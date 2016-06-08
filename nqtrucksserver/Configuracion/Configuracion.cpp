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
#include "Configuracion.h"
#include "ui_Configuracion.h"

#include <QDebug>
#include <QDesktopWidget>

//#include <QDir>
//#include <QUrl>

#include "alpr.h"
#include "prewarp.h"


Configuracion::Configuracion(nQTrucks::nQTrucksEngine *_engine, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Configuracion)
    , m_running(false)
    , engine(_engine)
{
    ui->setupUi(this);

    //engine  = new nQTrucks::nQTrucksEngine(this);
    m_matricularesults.resize(2);

    connect(ui->runningCheckBox,SIGNAL(clicked(bool)),this,SLOT(isRunning(bool)));

    /** CAMARAS **/
    connect(engine,SIGNAL(CamaraIPFotoCV1(cv::Mat)),this,SLOT(onGetFotoCV1(cv::Mat)));
    connect(engine,SIGNAL(CamaraIPFotoCV2(cv::Mat)),this,SLOT(onGetFotoCV2(cv::Mat)));

    /** IO **/
    connect(engine,SIGNAL(SemaforoConnectChanged(bool)),this,SLOT(on_SemaforoConnect(bool)));
    connect(engine,SIGNAL(SemaforoEstadoChanged(int)),this,SLOT(on_SemaforoEstadoChanged(int)));

    /** BASCULAS **/
    connect(engine ,SIGNAL(BasculaStatus(bool)),this,SLOT(on_BasculaConectada(bool)));
    connect(engine ,SIGNAL(BasculaChanged(t_Bascula)),this,SLOT(onBascula(t_Bascula)));

    /** ALPR 1 **/
    connect(engine, SIGNAL(ReplyMatriculaResults1(Registros::t_MatriculaResults)),this,SLOT(onReplyMatriculaResults1(Registros::t_MatriculaResults)));
    connect(engine ,SIGNAL(ReplyOriginalFoto1(cv::Mat)),this,SLOT(onGetOriginalMatricula1(cv::Mat)));
    connect(engine ,SIGNAL(ReplyOriginalFotoRoja1(cv::Mat)),this,SLOT(onGetOriginalMatriculaRoja1(cv::Mat)));
    connect(engine ,SIGNAL(ReplyOriginalFotoBlanca1(cv::Mat)),this,SLOT(onGetOriginalMatriculaBlanca1(cv::Mat)));

    /** ALPR 2 **/
    connect(engine, SIGNAL(ReplyMatriculaResults2(Registros::t_MatriculaResults)),this,SLOT(onReplyMatriculaResults2(Registros::t_MatriculaResults)));
    connect(engine ,SIGNAL(ReplyOriginalFoto2(cv::Mat)),this,SLOT(onGetOriginalMatricula2(cv::Mat)));
    connect(engine ,SIGNAL(ReplyOriginalFotoRoja2(cv::Mat)),this,SLOT(onGetOriginalMatriculaRoja2(cv::Mat)));
    connect(engine ,SIGNAL(ReplyOriginalFotoBlanca2(cv::Mat)),this,SLOT(onGetOriginalMatriculaBlanca2(cv::Mat)));

    loadconfig();
    //setFixedSize(1024,768);
    //this->showFullScreen();
}

Configuracion::~Configuracion()
{
    delete ui;
}


void Configuracion::updateGui(){
    switch (ui->CamaraSelect->currentIndex()) {
    case 0:
        ui->camaraLabel->setPixmap(QPixmap::fromImage(convertMat2QImage(m_matricularesults[0].OrigenFoto.clone())));
        break;
    case 1:
        ui->camaraLabel->setPixmap(QPixmap::fromImage(convertMat2QImage(m_matricularesults[1].OrigenFoto.clone())));
        break;
    }
}

void Configuracion::isRunning(bool clicked)
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
void Configuracion::loadconfig()
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

    /** REPORTS **/
    engine->report_buildReportsTree(ui->reportsTreeWidget);
    /** END REPORTS **/


    updateGui();
    updateCalibracionGui();


}
/** END SETTINGS **/


/** CAMARAS *************************************************************************/
    /** CAMARA1 **/
void Configuracion::onGetFotoCV1(const cv::Mat &fotocv){
    cv::resize(fotocv,m_matricularesults[0].OrigenFoto,m_matricularesults[0].OrigenFoto.size());
    updateGui();
    updateCalibracionGui();
}
    /** CAMARA2 **/
void Configuracion::onGetFotoCV2(const cv::Mat &fotocv){
    cv::resize(fotocv,m_matricularesults[1].OrigenFoto,m_matricularesults[1].OrigenFoto.size());
    updateGui();
    updateCalibracionGui();
}

void Configuracion::on_CamaraSelect_currentIndexChanged(int index)
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



void Configuracion::on_GuardarCamara_clicked()
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
void Configuracion::on_actualizarSemaforos_clicked(){
    ui->ioDevicesComboBox1->clear();
    QStringList l_IODevices = engine->getIODevices();
    ui->ioDevicesComboBox1->addItems(l_IODevices);
}

void Configuracion::on_conectarSemaforo_clicked(){
    on_guardarSemaforo_clicked();
    engine->setSemaforoDevicesConnect(true);
}

void Configuracion::on_desconectarSemaforo_clicked(){
    engine->setSemaforoDevicesConnect(false);
}

void Configuracion::on_guardarSemaforo_clicked(){
    engine->appConfig()->setValue(QString(NEWSAGESIO) + "/device",ui->ioDevicesComboBox1->currentText());
}

void Configuracion::on_semaforoVerde_clicked(){
    engine->setSemaforoStatus(SEMAFORO_VERDE);
}

void Configuracion::on_semaforoAmarillo_clicked(){
    engine->setSemaforoStatus(SEMAFORO_AMARILLO);
}

void Configuracion::on_semaforoRojo_clicked(){
    engine->setSemaforoStatus(SEMAFORO_ROJO);
}

void Configuracion::on_SemaforoConnect(bool status){
    ui->semaforoConectado->setChecked(status);
}

void Configuracion::on_SemaforoEstadoChanged(int _color){
    switch (_color) {
    case SEMAFORO_VERDE:
        qDebug() << "SEMAFORO VERDE";
        ui->semaforoVerde->setChecked(true);
        break;
    case SEMAFORO_AMARILLO:
        qDebug() << "SEMAFORO AMARILLO";
        ui->semaforoAmarillo->setChecked(true);
        break;
    case SEMAFORO_ROJO:
        qDebug() << "SEMAFORO ROJO";
        ui->semaforoRojo->setChecked(true);
        break;
    default:
        qDebug() << "SEMAFORO VERDE INDETERMINADO";
        ui->semaforoVerde->setChecked(true);
        break;
    }
}
/** END NEWSAGES I/O **/




/** BASCULAS **/
void Configuracion::on_actualizarBasculas_clicked()
{
    ui->BasculaDevicesComboBox->clear();
    QStringList l_Devices = engine->getSerialDevices();
    ui->BasculaDevicesComboBox->addItems(l_Devices);
}

void Configuracion::on_conectarBascula_clicked()
{
    on_guardarBascula_clicked();
    engine->setBasculaConnect(true);
}

void Configuracion::on_desconectarBascula_clicked()
{
    engine->setBasculaConnect(false);
}

void Configuracion::on_guardarBascula_clicked()
{
    engine->appConfig()->setValue(QString(BASCULA) + "/device",ui->BasculaDevicesComboBox->currentText());
    engine->appConfig()->setValue(QString(BASCULA) + "/tipo",QString::number(ui->BasculaTipoComboBox->currentIndex()));
}

void Configuracion::on_BasculaConectada(bool conectada)
{
    ui->BasculaStatus->setChecked(conectada);
    if (!conectada){
        ui->BasculaLcd->display("-------");
        ui->BasculaLcd2->display("-------");
        ui->BasculaLcd3->display("-------");
        ui->BasculaStatus->setChecked(false);
    }
}

void Configuracion::onBascula(t_Bascula _bascula)
{          
    ui->BasculaLcd->display(_bascula.iBruto);
    ui->BasculaLcd2->display(_bascula.iTara);
    ui->BasculaLcd3->display(_bascula.iNeto);
    ui->BasculaEstable->setChecked(_bascula.bEstado);
}

/** END BASCULAS **/

/** CONVERSORES ***********************************/
QImage Configuracion::convertMat2QImage(const cv::Mat& src)
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
/** END CONVERSORES ***********************************/



/** CALIBRACION ***************************************************************/
    /** GUI **/
void Configuracion::on_calibracionSelect_currentIndexChanged(int index){
    setAlprIndex(index);
    loadPlanks(index);
    updateCalibracionGui();
}

void Configuracion::loadPlanks(const int &index){
    switch (index) {
    case 0:
        ui->vPlankA->setValue(engine->appConfig()->value(QString(ALPR) + "/planka1","0").toInt());
        ui->vPlankB->setValue(engine->appConfig()->value(QString(ALPR) + "/plankb1","0").toInt());
        ui->vPlankC->setValue(engine->appConfig()->value(QString(ALPR) + "/plankc1","0").toInt());
        break;
    case 1:
        ui->vPlankA->setValue(engine->appConfig()->value(QString(ALPR) + "/planka2","0").toInt());
        ui->vPlankB->setValue(engine->appConfig()->value(QString(ALPR) + "/plankb2","0").toInt());
        ui->vPlankC->setValue(engine->appConfig()->value(QString(ALPR) + "/plankc2","0").toInt());
        break;
    }
}
void Configuracion::on_TestMatricula_clicked(){
    int index = getAlprIndex();
    on_guardarPlanK_clicked();
    engine->getFotoMatricula(index,m_matricularesults[index].OrigenFoto.clone());
}
void Configuracion::on_ActualizarCamara_clicked(){
    int index = getAlprIndex();
    engine->getCamaraFoto(index);
}
    /** END GUI **/
    /** ALPR **/
void Configuracion::updateCalibracionGui(){
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
void Configuracion::onReplyMatriculaResults1(const Registros::t_MatriculaResults &_result){
    m_matricularesults[0] = _result;
    loadPlanks(0);
    updateCalibracionGui();
}

void Configuracion::onGetOriginalMatricula1(const cv::Mat &foto){
    m_matricularesults[0].OrigenFoto = foto.clone();
    updateCalibracionGui();
}

void Configuracion::onGetOriginalMatriculaRoja1(const cv::Mat &foto){
    m_matricularesults[0].OrigenFotoRoja = foto.clone();
    updateCalibracionGui();
}

void Configuracion::onGetOriginalMatriculaBlanca1(const cv::Mat &foto){
    m_matricularesults[0].OrigenFotoBlanca = foto.clone();
    updateCalibracionGui();
}
    /** END ALPR1 **/
    /** ALPR2 **/
void Configuracion::onReplyMatriculaResults2(const Registros::t_MatriculaResults &_result){
    m_matricularesults[1] = _result;
    loadPlanks(1);
    updateCalibracionGui();
}

void Configuracion::onGetOriginalMatricula2(const cv::Mat &foto){
    m_matricularesults[1].OrigenFoto = foto.clone();
    updateCalibracionGui();
}

void Configuracion::onGetOriginalMatriculaRoja2(const cv::Mat &foto){
    m_matricularesults[1].OrigenFotoRoja = foto.clone();
    updateCalibracionGui();
}

void Configuracion::onGetOriginalMatriculaBlanca2(const cv::Mat &foto){
    m_matricularesults[1].OrigenFotoBlanca = foto.clone();
    updateCalibracionGui();
}
    /** END ALPR2 **/
    /** PLANKs **/
void Configuracion::on_vPlankA_valueChanged(int value){
    ui->lvPlankA->setText(QString::number( value));
}
void Configuracion::on_vPlankB_valueChanged(int value){
    ui->lvPlankB->setText(QString::number( value));
}
void Configuracion::on_vPlankC_valueChanged(int value){
    ui->lvPlankC->setText(QString::number( value));
}


void Configuracion::on_guardarPlanK_clicked()
{
    int index = getAlprIndex();
    switch (index) {
    case 0:
        engine->appConfig()->setValue(QString(ALPR) + "/planka1",QString::number(ui->vPlankA->value()));
        engine->appConfig()->setValue(QString(ALPR) + "/plankb1",QString::number(ui->vPlankB->value()));
        engine->appConfig()->setValue(QString(ALPR) + "/plankc1",QString::number(ui->vPlankC->value()));
        break;
    case 1:
        engine->appConfig()->setValue(QString(ALPR) + "/planka2",QString::number(ui->vPlankA->value()));
        engine->appConfig()->setValue(QString(ALPR) + "/plankb2",QString::number(ui->vPlankB->value()));
        engine->appConfig()->setValue(QString(ALPR) + "/plankc2",QString::number(ui->vPlankC->value()));
        break;
    }
    engine->calibrarFoto(index,m_matricularesults[index].OrigenFoto.clone());
}
    /** END PLANKs **/
/** END CALIBRACION *******************************************************************/

void Configuracion::on_reportsTreeWidget_doubleClicked(const QModelIndex &index)
{
    qDebug() << index.data(Qt::UserRole).toString();
    engine->report_loadFromFile(index.data(Qt::UserRole).toString());
    engine->report_desingReport();
}
