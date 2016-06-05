/***************************************************************************
 *   This file is part of the nQTrucks project                             *
 *   Copyright (C) 2015 by Efraím Pérez                                    *
 *   newsages2014@gmail.com                                                *
 *                                                                         *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
/*
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QStringListModel>
*/

#include <QDebug>
#include <QDesktopWidget>

#include <QDir>
#include <QUrl>

#include "alpr.h"
#include "prewarp.h"


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
    , m_running(false)
{
    ui->setupUi(this);

    engine  = new nQTrucks::nQTrucksEngine(this);
    m_matricularesults.resize(2);

    connect(ui->runningCheckBox,SIGNAL(clicked(bool)),this,SLOT(isRunning(bool)));

    /** CAMARAS **/
    connect(engine,SIGNAL(CamaraIPFotoCV1(cv::Mat)),this,SLOT(onGetFotoCV1(cv::Mat)));
    connect(engine,SIGNAL(CamaraIPFotoCV2(cv::Mat)),this,SLOT(onGetFotoCV2(cv::Mat)));

    /** IO **/
    connect(engine,SIGNAL(IODevicesStatusChanged(bool)),this,SLOT(on_ioDeviceSTATUS(bool)));
    connect(engine,SIGNAL(IODevicesSemaforoChanged(int)),this,SLOT(on_ioDeviceSemaforoChanged(int)));

    /** BASCULAS **/
    connect(engine ,SIGNAL(BasculaStatus(bool)),this,SLOT(on_BasculaConectada(bool)));
    connect(engine ,SIGNAL(BasculaChanged(t_Bascula)),this,SLOT(onBascula(t_Bascula)));

    /** ALPR 1 **/
    connect(engine, SIGNAL(ReplyMatriculaResults1(t_MatriculaResults)),this,SLOT(onReplyMatriculaResults1(t_MatriculaResults)));
    connect(engine ,SIGNAL(ReplyOriginalFoto1(cv::Mat)),this,SLOT(onGetOriginalMatricula1(cv::Mat)));
    connect(engine ,SIGNAL(ReplyOriginalFotoRoja1(cv::Mat)),this,SLOT(onGetOriginalMatriculaRoja1(cv::Mat)));
    connect(engine ,SIGNAL(ReplyOriginalFotoBlanca1(cv::Mat)),this,SLOT(onGetOriginalMatriculaBlanca1(cv::Mat)));

    /** ALPR 2 **/
    connect(engine, SIGNAL(ReplyMatriculaResults2(t_MatriculaResults)),this,SLOT(onReplyMatriculaResults2(t_MatriculaResults)));
    connect(engine ,SIGNAL(ReplyOriginalFoto2(cv::Mat)),this,SLOT(onGetOriginalMatricula2(cv::Mat)));
    connect(engine ,SIGNAL(ReplyOriginalFotoRoja2(cv::Mat)),this,SLOT(onGetOriginalMatriculaRoja2(cv::Mat)));
    connect(engine ,SIGNAL(ReplyOriginalFotoBlanca2(cv::Mat)),this,SLOT(onGetOriginalMatriculaBlanca2(cv::Mat)));

    loadconfig();
//    setFixedSize(1024,768);
//    this->showFullScreen();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateGui(){
    switch (ui->CamaraSelect->currentIndex()) {
    case 0:
        ui->camaraLabel->setPixmap(QPixmap::fromImage(convertMat2QImage(m_matricularesults[0].OrigenFoto.clone())));
        break;
    case 1:
        ui->camaraLabel->setPixmap(QPixmap::fromImage(convertMat2QImage(m_matricularesults[1].OrigenFoto.clone())));
        break;
    }
}

void MainWindow::isRunning(bool clicked)
{
    m_running=clicked;
    engine->setInitDaemon(m_running);
    //DEBUG NO BLOQUEAR ui->configTabWidget->setEnabled(!m_running);
    if (m_running){
        ui->runningCheckBox->setText("Running...");
    }else{
        ui->runningCheckBox->setText("Stoped...");
    }
}

/** SETTINGS **/
void MainWindow::loadconfig()
{

    /** DEFAULTS UI **/
    for(m_matricularesults_iterator = m_matricularesults.begin(); m_matricularesults_iterator != m_matricularesults.end(); m_matricularesults_iterator++)
    {
        m_matricularesults_iterator->OrigenFoto=cv::Mat::zeros( 720, 1280, CV_8UC3 );
        m_matricularesults_iterator->OrigenFotoPrewarp=cv::Mat::zeros( 720, 1280, CV_8UC3 );
        m_matricularesults_iterator->OrigenFotoRoja=cv::Mat::zeros( 720, 1280, CV_8UC3 );
        m_matricularesults_iterator->OrigenFotoBlanca=cv::Mat::zeros( 720, 1280, CV_8UC3 );
        m_matricularesults_iterator->MatriculaFotoA=cv::Mat::zeros( 110, 520, CV_8UC3 );
        m_matricularesults_iterator->MatriculaFotoB=cv::Mat::zeros( 110, 520, CV_8UC3 );
    }
    /** END DEFAULT UIS **/


    /** INTERFACE **/
    ui->configTabWidget->setEnabled(!m_running);

    /** CAMARAS **/
    ui->CamaraSelect->addItem(CAMARA1);
    ui->CamaraSelect->addItem(CAMARA2);
    /** END CAMARAS **/

    /** IO DEVICES **/
    QStringList l_IODevices = engine->getIODevices();
    ui->ioDevicesComboBox1->addItems(l_IODevices);
    ui->ioDeviceSTATUSLabel->setAutoFillBackground(true);
    ui->ioDeviceSTATUSLabel->setText("Disc");
    /** IO DEVICES **/

    /** BASCULAS **/
    ui->BasculaLcd->display("-------");
    ui->BasculaLcd2->display("-------");
    ui->BasculaLcd3->display("-------");
    ui->BasculaEstable->setChecked(false);
    ui->BasculaEstable->setAutoExclusive(false);

    QStringList l_BasculaTipo = engine->getTiposBasculas();
    ui->BasculaTipoComboBox->addItems(l_BasculaTipo);

    QStringList l_BasculaDevices = engine->getSerialDevices();
    ui->BasculaDevicesComboBox->addItems(l_BasculaDevices);

    engine->appConfig()->beginGroup(BASCULA);
    ui->BasculaTipoComboBox->setCurrentIndex(engine->appConfig()->value("tipo","0").toInt());
    engine->appConfig()->endGroup();
    /** END BASCULAS **/

    /** ALPR  **/
    ui->calibracionSelect->addItem(ALPR1);
    ui->calibracionSelect->addItem(ALPR2);   

    updateGui();
    updateCalibracionGui();


}
/** END SETTINGS **/


/** CAMARAS *************************************************************************/
    /** CAMARA1 **/
void MainWindow::onGetFotoCV1(const cv::Mat &fotocv){
    cv::resize(fotocv,m_matricularesults[0].OrigenFoto,m_matricularesults[0].OrigenFoto.size());
    updateGui();
    updateCalibracionGui();
}
    /** CAMARA2 **/
void MainWindow::onGetFotoCV2(const cv::Mat &fotocv){
    cv::resize(fotocv,m_matricularesults[1].OrigenFoto,m_matricularesults[1].OrigenFoto.size());
    updateGui();
    updateCalibracionGui();
}

void MainWindow::on_CamaraSelect_currentIndexChanged(int index)
{
    QStringList l_Camaras = engine->getTiposCamaras();
    ui->camaraTipo->clear();
    ui->camaraTipo->addItems(l_Camaras);

    switch (index) {
    case 0:
        engine->appConfig()->beginGroup(CAMARA1);
        ui->camaraTipo->setCurrentIndex(engine->appConfig()->value("tipo","0").toInt());
        ui->hostLineEdit->setText(engine->appConfig()->value("host").toString());
        ui->puertoLineEdit->setText(engine->appConfig()->value("port").toString());
        ui->usuarioLineEdit->setText(engine->appConfig()->value("user").toString());
        ui->passwdLineEdit->setText(engine->appConfig()->value("pass").toString());
        engine->appConfig()->endGroup();
        break;
    case 1:
        engine->appConfig()->beginGroup(CAMARA2);
        ui->camaraTipo->setCurrentIndex(engine->appConfig()->value("tipo","0").toInt());
        ui->hostLineEdit->setText(engine->appConfig()->value("host").toString());
        ui->puertoLineEdit->setText(engine->appConfig()->value("port").toString());
        ui->usuarioLineEdit->setText(engine->appConfig()->value("user").toString());
        ui->passwdLineEdit->setText(engine->appConfig()->value("pass").toString());
        engine->appConfig()->endGroup();
        break;
    }
    updateGui();
}



void MainWindow::on_GuardarCamara_clicked()
{
    switch (ui->CamaraSelect->currentIndex()) {
    case 0:
        engine->appConfig()->beginGroup(CAMARA1);
        engine->appConfig()->setValue("tipo",QString::number(ui->camaraTipo->currentIndex()));
        engine->appConfig()->setValue("host",ui->hostLineEdit->text());
        engine->appConfig()->setValue("port",ui->puertoLineEdit->text());
        engine->appConfig()->setValue("user",ui->usuarioLineEdit->text());
        engine->appConfig()->setValue("pass",ui->passwdLineEdit->text());
        engine->appConfig()->endGroup();
        engine->getCamaraFoto(0);
        break;
    case 1:
        engine->appConfig()->beginGroup(CAMARA2);
        engine->appConfig()->setValue("tipo",QString::number(ui->camaraTipo->currentIndex()));
        engine->appConfig()->setValue("host",ui->hostLineEdit->text());
        engine->appConfig()->setValue("port",ui->puertoLineEdit->text());
        engine->appConfig()->setValue("user",ui->usuarioLineEdit->text());
        engine->appConfig()->setValue("pass",ui->passwdLineEdit->text());
        engine->appConfig()->endGroup();
        engine->getCamaraFoto(1);
        break;
    }

}
/** END CAMARAS ************************************************************************/



/** NEWSAGES I/O  **/
void MainWindow::on_actualizarIODEvicestoolButton_clicked()
{
    ui->ioDevicesComboBox1->clear();
    QStringList l_IODevices = engine->getIODevices();
    ui->ioDevicesComboBox1->addItems(l_IODevices);
}

void MainWindow::on_guardarIODevicesPushButton1_clicked()
{
    engine->appConfig()->beginGroup(NEWSAGESIO);
    engine->appConfig()->setValue("device",ui->ioDevicesComboBox1->currentText());
    engine->appConfig()->endGroup();
    //engine->setIODevicesConfig();
}

void MainWindow::on_ioDeviceONpushButton_clicked()
{
    engine->setSemaforoStatus(SEMAFORO_VERDE);
}

void MainWindow::on_ioDeviceOFFpushButton_clicked()
{
        engine->setSemaforoStatus(SEMAFORO_ROJO);
}

void MainWindow::on_ioDeviceSTATUS(bool status)
{
  if(status){
      ui->ioDeviceSTATUSLabel->setPalette(m_deviceReady_palette);
      ui->ioDeviceSTATUSLabel->setText("READY");
  }else{
      ui->ioDeviceSTATUSLabel->setPalette(m_deviceNotReady_palette);
      ui->ioDeviceSTATUSLabel->setText("Disc");
  }
}

void MainWindow::on_ioDeviceSemaforoChanged(int _color)
{
    switch (_color) {
    case SEMAFORO_VERDE:
        qDebug() << "SEMAFORO VERDE";
        break;
    case SEMAFORO_AMARILLO:
        //qDebug() << "SEMAFORO AMARILLO";
        break;
    case SEMAFORO_ROJO:
        qDebug() << "SEMAFORO ROJO";
        break;
    default:
        qDebug() << "SEMAFORO VERDE INDETERMINADO";
        break;
    }
}

void MainWindow::on_conectarIODevicesPushButton_clicked()
{
    engine->setSemaforoDevicesConnect(true);
}

void MainWindow::on_desconectarIODevicesPushButton_clicked()
{
    engine->setSemaforoDevicesConnect(false);
}
/** END NEWSAGES I/O **/




/** BASCULAS **/
void MainWindow::on_conectarBasculaPushButton_clicked()
{
    engine->setBasculaConnect(true);
}

void MainWindow::on_desconectarBasculaPushButton_clicked()
{
    engine->setBasculaConnect(false);
}

void MainWindow::onBascula(t_Bascula _bascula)
{          
    ui->BasculaLcd->display(_bascula.iBruto);
    ui->BasculaLcd2->display(_bascula.iTara);
    ui->BasculaLcd3->display(_bascula.iNeto);

    if(_bascula.bEstado){// == _bascula.bEstado){
        ui->BasculaEstable->setChecked(_bascula.bEstado);
    }
}

void MainWindow::on_guardarBasculaPushButton_clicked()
{
    engine->appConfig()->beginGroup(BASCULA);
    engine->appConfig()->setValue("device",ui->BasculaDevicesComboBox->currentText());
    engine->appConfig()->setValue("tipo",QString::number(ui->BasculaTipoComboBox->currentIndex()));
    engine->appConfig()->endGroup();
    //engine->setIODevicesConfig();
}

void MainWindow::on_actualizarIBasculasButton_clicked()
{
    ui->BasculaDevicesComboBox->clear();
    QStringList l_Devices = engine->getSerialDevices();
    ui->BasculaDevicesComboBox->addItems(l_Devices);
}

void MainWindow::on_BasculaConectada(bool conectada)
{
    ui->BasculaStatus->setChecked(conectada);
    if (!conectada){
        ui->BasculaLcd->display("-------");
        ui->BasculaLcd2->display("-------");
        ui->BasculaLcd3->display("-------");
        ui->BasculaEstable->setChecked(false);
    }
}

/** END BASCULAS **/



/** CALIBRACION ***********************************/


QImage MainWindow::convertMat2QImage(const cv::Mat& src)
{
    QImage qtImg;
    if( !src.empty() && src.depth() == CV_8U ){
        if(src.channels() == 1){
            qtImg = QImage( (const unsigned char *)(src.data),
                            src.cols,
                            src.rows,
                            QImage::Format_Indexed8 );
        }
        else{
            cv::cvtColor( src, src, CV_BGR2RGB );
            qtImg = QImage( (const unsigned char *)(src.data),
                            src.cols,
                            src.rows,
                            src.step,
                            QImage::Format_RGB888 );
        }
    }
    return qtImg.copy();
}



/** PREWARP **/
void MainWindow::loadprewarp(){
    /** DEBUG **/
    //updateOriginal();
//    int max_width;
//    int max_height;
//    QStringList valores;
//    /** PREWARP **/
//    switch (ui->calibracionSelect->currentIndex()) {
//    case 0:
//        m_fotoprewarpCVA1 = m_fotocamaraCVA1.clone();
//        prewarp.w = m_fotoprewarpCVA1.cols;
//        prewarp.h = m_fotoprewarpCVA1.rows;
//        max_width = m_fotoprewarpCVA1.cols;
//        max_height = m_fotoprewarpCVA1.rows;

//        if (m_fotoprewarpCVA1.cols > max_width){
//          float aspect = max_width / ((float)m_fotoprewarpCVA1.cols);
//          float y = ((float)m_fotoprewarpCVA1.rows) * aspect;
//          cv::resize(m_fotoprewarpCVA1, m_fotoprewarpCVA1, cv::Size((int) max_width, (int) y));
//        }
//        if (m_fotoprewarpCVA1.rows > max_height){
//          float aspect = max_height / ((float)m_fotoprewarpCVA1.rows);
//          float x = ((float)m_fotoprewarpCVA1.cols) * aspect;
//          cv::resize(m_fotoprewarpCVA1, m_fotoprewarpCVA1, cv::Size((int) x, (int) max_height));
//        }
//        valores = m_prewarp.split(",");
//        if (valores.count()==10){
//            prewarp.w = m_fotoprewarpCVA1.cols;
//            prewarp.h = m_fotoprewarpCVA1.rows;
//            prewarp.rotationx   = QString(valores.value(3)).toFloat();
//            prewarp.rotationy   = QString(valores.value(4)).toFloat();
//            prewarp.rotationz   = QString(valores.value(5)).toFloat();
//            prewarp.stretchX    = QString(valores.value(6)).toFloat();
//            prewarp.dist        = QString(valores.value(7)).toFloat();
//            prewarp.panX        = QString(valores.value(8)).toFloat();
//            prewarp.panY        = QString(valores.value(9)).toFloat();

//            //set bars
//            ui->valueXA1->setValue(-(prewarp.rotationx*20000.0)+100);
//            ui->valueYA1->setValue((prewarp.rotationy*20000.0)+100);
//            ui->valueZA1->setValue(-(prewarp.rotationz*100.0)+100);
//            ui->valueWA1->setValue(-(1-prewarp.stretchX)*-200+100);
//            ui->valueDA1->setValue((1-prewarp.dist)*200+100);
//        }
//        else{
//            //DEFAULT
//            ui->valueXA1->setValue(100);
//            ui->valueYA1->setValue(100);
//            ui->valueZA1->setValue(100);
//            ui->valueWA1->setValue(100);
//            ui->valueDA1->setValue(100);
//        }
//        m_fotoprewarpCVA1=updateprewarp(m_fotoprewarpCVA1);
//        break;

//    case 1:
//        m_fotoprewarpCVA2 = m_fotocamaraCVA2.clone();
//        prewarp.w = m_fotoprewarpCVA2.cols;
//        prewarp.h = m_fotoprewarpCVA2.rows;
//        max_width = m_fotoprewarpCVA2.cols;
//        max_height = m_fotoprewarpCVA2.rows;
//        break;
//        if (m_fotoprewarpCVA2.cols > max_width){
//          float aspect = max_width / ((float)m_fotoprewarpCVA2.cols);
//          float y = ((float)m_fotoprewarpCVA2.rows) * aspect;
//          cv::resize(m_fotoprewarpCVA2, m_fotoprewarpCVA2, cv::Size((int) max_width, (int) y));
//        }
//        if (m_fotoprewarpCVA2.rows > max_height){
//          float aspect = max_height / ((float)m_fotoprewarpCVA2.rows);
//          float x = ((float)m_fotoprewarpCVA2.cols) * aspect;
//          cv::resize(m_fotoprewarpCVA2, m_fotoprewarpCVA2, cv::Size((int) x, (int) max_height));
//        }
//        valores = m_prewarp.split(",");
//        if (valores.count()==10){
//            prewarp.w = m_fotoprewarpCVA2.cols;
//            prewarp.h = m_fotoprewarpCVA2.rows;
//            prewarp.rotationx   = QString(valores.value(3)).toFloat();
//            prewarp.rotationy   = QString(valores.value(4)).toFloat();
//            prewarp.rotationz   = QString(valores.value(5)).toFloat();
//            prewarp.stretchX    = QString(valores.value(6)).toFloat();
//            prewarp.dist        = QString(valores.value(7)).toFloat();
//            prewarp.panX        = QString(valores.value(8)).toFloat();
//            prewarp.panY        = QString(valores.value(9)).toFloat();

//            //set bars
//            ui->valueXA1->setValue(-(prewarp.rotationx*20000.0)+100);
//            ui->valueYA1->setValue((prewarp.rotationy*20000.0)+100);
//            ui->valueZA1->setValue(-(prewarp.rotationz*100.0)+100);
//            ui->valueWA1->setValue(-(1-prewarp.stretchX)*-200+100);
//            ui->valueDA1->setValue((1-prewarp.dist)*200+100);
//        }
//        else{
//            //DEFAULT
//            ui->valueXA1->setValue(100);
//            ui->valueYA1->setValue(100);
//            ui->valueZA1->setValue(100);
//            ui->valueWA1->setValue(100);
//            ui->valueDA1->setValue(100);
//        }
//        m_fotoprewarpCVA2=updateprewarp(m_fotoprewarpCVA2);
//        break;
//    }
//    updateGui();
}

QString MainWindow::get_prewarp_config()
{
//    float width_ratio;
//    float height_ratio;

//    switch (ui->calibracionSelect->currentIndex()) {
//    case 0:
////        width_ratio = prewarp.w / ((float)m_fotoprewarpCVA1.cols);
////        height_ratio = prewarp.h / ((float)m_fotoprewarpCVA1.rows);
//        break;
//    case 1:
////        width_ratio = prewarp.w / ((float)m_fotoprewarpCVA2.cols);
////        height_ratio = prewarp.h / ((float)m_fotoprewarpCVA2.rows);
//        break;
//    }
//    updateGui();

//    prewarp.rotationx *=width_ratio;
//    prewarp.rotationy *=width_ratio;
//    prewarp.panX /= width_ratio;
//    prewarp.panY /= height_ratio;

//    std::stringstream output;
//    output << "planar," << std::fixed;
//    output << prewarp.w << "," << prewarp.h << ",";
//    output << prewarp.rotationx << "," << prewarp.rotationy << "," << prewarp.rotationz << ",";
//    output << prewarp.stretchX << "," << prewarp.dist << ",";
//    output << prewarp.panX << "," << prewarp.panY;
//    m_prewarp= QString::fromStdString(output.str());
    //qDebug() << m_prewarp;
    return m_prewarp;

}

cv::Mat MainWindow::updateprewarp(cv::Mat img){
    alpr::Config config("truck");
    config.prewarp = get_prewarp_config().toStdString();
    config.setDebug(false);
    alpr::PreWarp prewarp(&config);
    return prewarp.warpImage(img);
}

void MainWindow::on_valueXA1_valueChanged(int value)
{
//    prewarp.rotationx = -((float)value - 100) / 20000.0;
//    switch (ui->calibracionSelect->currentIndex()) {
//    case 0:
//        m_matricularesults1.OrigenFotoPrewarp=updateprewarp(m_matricularesults1.OrigenFotoPrewarp);
//        break;
//    case 1:
//        m_matricularesults2.OrigenFotoPrewarp=updateprewarp(m_matricularesults2.OrigenFotoPrewarp);
//        break;
//    }
}

void MainWindow::on_valueYA1_valueChanged(int value)
{
//      prewarp.rotationy = ((float)value - 100) / 20000.0;
//      switch (ui->calibracionSelect->currentIndex()) {
//      case 0:
//          m_matricularesults1.OrigenFotoPrewarp=updateprewarp(m_matricularesults1.OrigenFotoPrewarp);
//          break;
//      case 1:
//          m_matricularesults2.OrigenFotoPrewarp=updateprewarp(m_matricularesults2.OrigenFotoPrewarp);
//          break;
//      }
}

void MainWindow::on_valueZA1_valueChanged(int value)
{
//  prewarp.rotationz = -((float)value - 100) / 100.0;
//  switch (ui->calibracionSelect->currentIndex()) {
//  case 0:
//      m_matricularesults1.OrigenFotoPrewarp=updateprewarp(m_matricularesults1.OrigenFotoPrewarp);
//      break;
//  case 1:
//      m_matricularesults2.OrigenFotoPrewarp=updateprewarp(m_matricularesults2.OrigenFotoPrewarp);
//      break;
//  }
}

void MainWindow::on_valueWA1_valueChanged(int value)
{
//    prewarp.stretchX = 1.0 + ((float)value - 100) / -200.0;
//    switch (ui->calibracionSelect->currentIndex()) {
//    case 0:
//        m_matricularesults1.OrigenFotoPrewarp=updateprewarp(m_matricularesults1.OrigenFotoPrewarp);
//        break;
//    case 1:
//        m_matricularesults2.OrigenFotoPrewarp=updateprewarp(m_matricularesults2.OrigenFotoPrewarp);
//        break;
//    }
}

void MainWindow::on_valueDA1_valueChanged(int value)
{
//    prewarp.dist = 1.0 - ((float)value - 100) / 200.0;
//    switch (ui->calibracionSelect->currentIndex()) {
//    case 0:
//        m_matricularesults1.OrigenFotoPrewarp=updateprewarp(m_matricularesults1.OrigenFotoPrewarp);
//        break;
//    case 1:
//        m_matricularesults2.OrigenFotoPrewarp=updateprewarp(m_matricularesults2.OrigenFotoPrewarp);
//        break;
//    }
}

void MainWindow::on_guardarPrewarp_clicked()
{
//    engine->appConfig()->beginGroup(ALPR);
//    engine->appConfig()->setValue(QString("prewarp"+(QString::number(ui->calibracionSelect->currentIndex()+1))),get_prewarp_config());
//    engine->appConfig()->endGroup();

}
/** END PREWARP **/















/** CALIBRACION ***************************************************************/
    /** GUI **/
void MainWindow::on_calibracionSelect_currentIndexChanged(int index){
    setAlprIndex(index);
    loadPlanks(index);
    updateCalibracionGui();
}

void MainWindow::loadPlanks(const int &index){
    switch (index) {
    case 0:
        engine->appConfig()->beginGroup(ALPR);
        ui->vPlankA->setValue(engine->appConfig()->value("planka1","0").toInt());
        ui->vPlankB->setValue(engine->appConfig()->value("plankb1","0").toInt());
        ui->vPlankC->setValue(engine->appConfig()->value("plankc1","0").toInt());
        engine->appConfig()->endGroup();
        break;
    case 1:
        engine->appConfig()->beginGroup(ALPR);
        ui->vPlankA->setValue(engine->appConfig()->value("planka2","0").toInt());
        ui->vPlankB->setValue(engine->appConfig()->value("plankb2","0").toInt());
        ui->vPlankC->setValue(engine->appConfig()->value("plankc2","0").toInt());
        engine->appConfig()->endGroup();
        break;
    }
}
void MainWindow::on_TestMatricula_clicked(){
    int index = getAlprIndex();
    on_guardarPlanK_clicked();
    engine->getFotoMatricula(index,m_matricularesults[index].OrigenFoto.clone());
}
void MainWindow::on_ActualizarCamara_clicked(){
    int index = getAlprIndex();
    engine->getCamaraFoto(index);
}
    /** END GUI **/
    /** ALPR **/
void MainWindow::updateCalibracionGui(){
    int index = getAlprIndex();
    ui->FotoOriginalA->setPixmap(QPixmap::fromImage(convertMat2QImage(m_matricularesults[index].OrigenFoto.clone())));
    ui->FotoMatriculaA->setPixmap(QPixmap::fromImage(convertMat2QImage(m_matricularesults[index].MatriculaFotoA.clone())));
    ui->FotoMatriculaB->setPixmap(QPixmap::fromImage(convertMat2QImage(m_matricularesults[index].MatriculaFotoB.clone())));
    ui->MatriculaA->setText(m_matricularesults[index].MatriculaA);
    ui->MatriculaB->setText(m_matricularesults[index].MatriculaB);
    ui->FotoBlancosA->setPixmap(QPixmap::fromImage(convertMat2QImage(m_matricularesults[index].OrigenFotoBlanca.clone())));
    ui->FotoRojosA->setPixmap(QPixmap::fromImage(convertMat2QImage(m_matricularesults[index].OrigenFotoRoja.clone())));
    ui->LongMatriculaA->setText(m_matricularesults[index].MatriculaPrecisionAs);
    ui->LongMatriculaB->setText(m_matricularesults[index].MatriculaPrecisionBs);
}
    /** ALPR 1 **/
void MainWindow::onReplyMatriculaResults1(const t_MatriculaResults &_result){
    m_matricularesults[0] = _result;
    loadPlanks(0);
    updateCalibracionGui();
}

void MainWindow::onGetOriginalMatricula1(const cv::Mat &foto){
    m_matricularesults[0].OrigenFoto = foto.clone();
    updateCalibracionGui();
}

void MainWindow::onGetOriginalMatriculaRoja1(const cv::Mat &foto){
    m_matricularesults[0].OrigenFotoRoja = foto.clone();
    updateCalibracionGui();
}

void MainWindow::onGetOriginalMatriculaBlanca1(const cv::Mat &foto){
    m_matricularesults[0].OrigenFotoBlanca = foto.clone();
    updateCalibracionGui();
}
    /** END ALPR1 **/
    /** ALPR2 **/
void MainWindow::onReplyMatriculaResults2(const t_MatriculaResults &_result){
    m_matricularesults[1] = _result;
    loadPlanks(1);
    updateCalibracionGui();
}

void MainWindow::onGetOriginalMatricula2(const cv::Mat &foto){
    m_matricularesults[1].OrigenFoto = foto.clone();
    updateCalibracionGui();
}

void MainWindow::onGetOriginalMatriculaRoja2(const cv::Mat &foto){
    m_matricularesults[1].OrigenFotoRoja = foto.clone();
    updateCalibracionGui();
}

void MainWindow::onGetOriginalMatriculaBlanca2(const cv::Mat &foto){
    m_matricularesults[1].OrigenFotoBlanca = foto.clone();
    updateCalibracionGui();
}
    /** END ALPR2 **/
    /** PLANKs **/
void MainWindow::on_vPlankA_valueChanged(int value){
    ui->lvPlankA->setText(QString::number( value));
}
void MainWindow::on_vPlankB_valueChanged(int value){
    ui->lvPlankB->setText(QString::number( value));
}
void MainWindow::on_vPlankC_valueChanged(int value){
    ui->lvPlankC->setText(QString::number( value));
}


void MainWindow::on_guardarPlanK_clicked()
{
    int index = getAlprIndex();
    switch (index) {
    case 0:
        engine->appConfig()->beginGroup(ALPR);
        engine->appConfig()->setValue("planka1",QString::number(ui->vPlankA->value()));
        engine->appConfig()->setValue("plankb1",QString::number(ui->vPlankB->value()));
        engine->appConfig()->setValue("plankc1",QString::number(ui->vPlankC->value()));
        engine->appConfig()->endGroup();
        break;
    case 1:
        engine->appConfig()->beginGroup(ALPR);
        engine->appConfig()->setValue("planka2",QString::number(ui->vPlankA->value()));
        engine->appConfig()->setValue("plankb2",QString::number(ui->vPlankB->value()));
        engine->appConfig()->setValue("plankc2",QString::number(ui->vPlankC->value()));
        engine->appConfig()->endGroup();
        break;
    }
    engine->calibrarFoto(index,m_matricularesults[index].OrigenFoto.clone());
}
void MainWindow::on_resetPlanK_clicked()
{

}

    /** END PLANKs **/
/** END CALIBRACION *******************************************************************/

