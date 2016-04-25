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
#include "nQTrucksEngine.h"

#include <QDir>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_running(false)
    //, m_config(new QSettings)
{
    ui->setupUi(this);

    connect(ui->runningCheckBox,SIGNAL(clicked(bool)),this,SLOT(isRunning(bool)));
    engine  = new nQTrucks::nQTrucksEngine(this);
    /** CAMARAS **/
    connect(engine,SIGNAL(CamaraIPFotoCV1(cv::Mat,cv::Mat,QImage)),
            this  ,SLOT(onGetFotoCV1(cv::Mat,cv::Mat,QImage)));
    connect(engine,SIGNAL(CamaraIPFotoCV2(cv::Mat,cv::Mat,QImage)),
            this  ,SLOT(onGetFotoCV2(cv::Mat,cv::Mat,QImage)));

    /** IO **/
    connect(engine,SIGNAL(IODevicesStatusChanged(bool)),this,SLOT(on_ioDeviceSTATUS(bool)));
    connect(engine,SIGNAL(IODevicesPIN10Changed(bool)),this,SLOT(on_ioDevicePIN10(bool)));

    /** BASCULAS **/
    connect(engine,SIGNAL(BasculaStatus(bool)),this,SLOT(on_BasculaConectada(bool)));
    connect(engine,SIGNAL(BasculaChanged(t_Bascula)),this,SLOT(onBascula(t_Bascula)));

    /** ALPR A **/
    connect(engine,SIGNAL(ReplyOriginalFotoA(cv::Mat)),this,SLOT(onGetOriginalMatriculaA1(cv::Mat)));
    connect(engine,SIGNAL(ReplyOriginalFotoRojaA(cv::Mat)),this,SLOT(onGetOriginalMatriculaRojaA(cv::Mat)));
    connect(engine,SIGNAL(ReplyOriginalFotoBlancaA(cv::Mat)),this,SLOT(onGetOriginalMatriculaBlancaA(cv::Mat)));
    connect(engine, SIGNAL(ReplyMatriculaFotoA1(QString,QString,bool,cv::Mat)),
            this  , SLOT(onGetMatriculaFotoA1(  QString,QString,bool,cv::Mat)));
    connect(engine, SIGNAL(ReplyMatriculaFotoA2(QString,QString,bool,cv::Mat)),
            this  , SLOT(onGetMatriculaFotoA2(  QString,QString,bool,cv::Mat)));

    /** ALPR B **/
    connect(engine,SIGNAL(ReplyOriginalFotoB(cv::Mat)),this,SLOT(onGetOriginalMatriculaB1(cv::Mat)));
    connect(engine,SIGNAL(ReplyOriginalFotoRojaB(cv::Mat)),this,SLOT(onGetOriginalMatriculaRojaB(cv::Mat)));
    connect(engine,SIGNAL(ReplyOriginalFotoBlancaB(cv::Mat)),this,SLOT(onGetOriginalMatriculaBlancaB(cv::Mat)));
    connect(engine, SIGNAL(ReplyMatriculaFotoB1(QString,QString,bool,cv::Mat)),
            this  , SLOT(onGetMatriculaFotoB1(  QString,QString,bool,cv::Mat)));
    connect(engine, SIGNAL(ReplyMatriculaFotoB2(QString,QString,bool,cv::Mat)),
            this  , SLOT(onGetMatriculaFotoB2(  QString,QString,bool,cv::Mat)));


    loadconfig();

    /** DEBUG **/
    updateOriginal();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateOriginal(){
    QString filename = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("matriculas/r1.jpg");
    m_fotocamaraCVA = cv::imread(filename.toStdString(), CV_LOAD_IMAGE_COLOR);
    ui->FotoOriginalA->setPixmap(QPixmap::fromImage(convertMat2QImage(m_fotocamaraCVA.clone())));
    ui->FotoPrewarpA->setPixmap(QPixmap::fromImage(convertMat2QImage(m_fotocamaraCVA.clone())));
    //cv::imshow(this->windowTitle().toStdString(),m_fotocamaraCVA);
}

void MainWindow::isRunning(bool clicked)
{
    m_running=clicked;
    ui->configTabWidget->setEnabled(!m_running);
    if (m_running){
        ui->runningCheckBox->setText("Running...");
    }else{
        ui->runningCheckBox->setText("Stoped...");
    }
}

/** SETTINGS **/
void MainWindow::loadconfig()
{
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

    /** colores por defecto solo para configuracion **/
    m_semaforoOFF_palette.setColor(QPalette::Window, Qt::black);
    m_semaforoOFF_palette.setColor(QPalette::WindowText, Qt::black);
    m_semaforoRED_palette.setColor(QPalette::Window, Qt::red);
    m_semaforoRED_palette.setColor(QPalette::WindowText, Qt::red);
    m_semaforoGREEN_palette.setColor(QPalette::Window, Qt::green);
    m_semaforoGREEN_palette.setColor(QPalette::WindowText, Qt::green);

    ui->ioDeviceREDLabel->setPalette(m_semaforoOFF_palette);
    ui->ioDeviceGREENLabel->setPalette(m_semaforoOFF_palette);

    m_deviceNotReady_palette.setColor(QPalette::Window,Qt::yellow);
    m_deviceNotReady_palette.setColor(QPalette::WindowText,Qt::red);
    m_deviceReady_palette.setColor(QPalette::Window,Qt::blue);
    m_deviceReady_palette.setColor(QPalette::WindowText,Qt::black);

    ui->ioDeviceSTATUSLabel->setPalette(m_deviceNotReady_palette);
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
    //ui->BasculaDevicesComboBox->setCurrentIndex(engine->appConfig()->value("device","0").toInt());
    ui->BasculaTipoComboBox->setCurrentIndex(engine->appConfig()->value("tipo","0").toInt());
    engine->appConfig()->endGroup();
    engine->appConfig()->sync();

    /** END BASCULAS **/

    /** ALPR  **/
    ui->calibracionSelect->addItem(ALPR1);
    ui->calibracionSelect->addItem(ALPR2);
    /** END ALPR **/


}
/** END SETTINGS **/


/** CAMARAS *************************************************************************/
    /** CAMARA1 **/
void MainWindow::onGetFotoCV1(cv::Mat fotocv, cv::Mat fotorgbcv, QImage foto)
{
    m_fotocamaraCVA=fotocv.clone();
    ui->camaraLabel->setPixmap(QPixmap::fromImage(convertMat2QImage(m_fotocamaraCVA.clone())));
}
    /** CAMARA2 **/
void MainWindow::onGetFotoCV2(cv::Mat fotocv, cv::Mat fotorgbcv, QImage foto)
{
    m_fotocamaraCVA=fotocv.clone();
    ui->camaraLabel->setPixmap(QPixmap::fromImage(convertMat2QImage(m_fotocamaraCVA.clone())));
}

void MainWindow::on_CamaraSelect_currentIndexChanged(const QString &arg1)
{
    QStringList l_Camaras = engine->getTiposCamaras();
    ui->camaraTipo->addItems(l_Camaras);
    engine->appConfig()->beginGroup(arg1);
    ui->camaraTipo->setCurrentIndex(engine->appConfig()->value("tipo","0").toInt());
    ui->hostLineEdit->setText(engine->appConfig()->value("host").toString());
    ui->puertoLineEdit->setText(engine->appConfig()->value("port").toString());
    ui->usuarioLineEdit->setText(engine->appConfig()->value("user").toString());
    ui->passwdLineEdit->setText(engine->appConfig()->value("pass").toString());
    engine->appConfig()->endGroup();
    engine->appConfig()->sync();
}


void MainWindow::on_GuardarCamara_clicked()
{
    engine->appConfig()->beginGroup(ui->CamaraSelect->currentText());
    engine->appConfig()->setValue("tipo",QString::number(ui->camaraTipo->currentIndex()));
    engine->appConfig()->setValue("host",ui->hostLineEdit->text());
    engine->appConfig()->setValue("port",ui->puertoLineEdit->text());
    engine->appConfig()->setValue("user",ui->usuarioLineEdit->text());
    engine->appConfig()->setValue("pass",ui->passwdLineEdit->text());
    engine->appConfig()->endGroup();
    engine->appConfig()->sync();
    engine->getCamaraFoto(ui->CamaraSelect->currentIndex());
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
    engine->appConfig()->sync();
    engine->setIODevicesConfig();
}

void MainWindow::on_ioDeviceONpushButton_clicked()
{
    engine->setIODevicesPin10(false);
}

void MainWindow::on_ioDeviceOFFpushButton_clicked()
{
    engine->setIODevicesPin10(true);
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

void MainWindow::on_ioDevicePIN10(bool value)
{
    if (value){
        ui->ioDeviceGREENLabel->setPalette(m_semaforoOFF_palette);
        ui->ioDeviceREDLabel->setPalette(m_semaforoRED_palette);
    }else{
        ui->ioDeviceGREENLabel->setPalette(m_semaforoGREEN_palette);
        ui->ioDeviceREDLabel->setPalette(m_semaforoOFF_palette);
    }
}

void MainWindow::on_conectarIODevicesPushButton_clicked()
{
    engine->setIODevicesConnect(true);
}

void MainWindow::on_desconectarIODevicesPushButton_clicked()
{
    engine->setIODevicesConnect(false);
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

    if(_bascula.bEstado == _bascula.bEstado){
        ui->BasculaEstable->setChecked(_bascula.bEstado);
    }
}

void MainWindow::on_guardarBasculaPushButton_clicked()
{
    engine->appConfig()->beginGroup(BASCULA);
    engine->appConfig()->setValue("device",ui->BasculaDevicesComboBox->currentText());
    engine->appConfig()->setValue("tipo",QString::number(ui->BasculaTipoComboBox->currentIndex()));
    engine->appConfig()->endGroup();
    engine->appConfig()->sync();
    engine->setIODevicesConfig();
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

/** ALRP A **/
void MainWindow::onGetOriginalMatriculaA1(cv::Mat foto)
{
    ui->FotoOriginalA->setPixmap(QPixmap::fromImage(convertMat2QImage(foto)));
}

void MainWindow::onGetOriginalMatriculaRojaA(cv::Mat foto)
{
    ui->FotoRojosA->setPixmap(QPixmap::fromImage(convertMat2QImage(foto)));
}

void MainWindow::onGetOriginalMatriculaBlancaA(cv::Mat foto)
{
    ui->FotoBlancosA->setPixmap(QPixmap::fromImage(convertMat2QImage(foto)));
}

void MainWindow::onGetMatriculaFotoA1(QString matricula, QString confianza, bool detectada, cv::Mat foto)
{
    ui->LongMatriculaA1->setText(confianza+ "%");
    ui->MatriculaA1->setText(matricula);
    ui->FotoMatriculaA1->setPixmap(QPixmap::fromImage(convertMat2QImage(foto)));
}
void MainWindow::onGetMatriculaFotoA2(QString matricula, QString confianza, bool detectada, cv::Mat foto)
{
    ui->LongMatriculaA2->setText(confianza + "%");
    ui->MatriculaA2->setText(matricula);
    ui->FotoMatriculaA2->setPixmap(QPixmap::fromImage(convertMat2QImage(foto)));
}
/** ALPR B **/
void MainWindow::onGetOriginalMatriculaB1(cv::Mat foto)
{
    ui->FotoOriginalA->setPixmap(QPixmap::fromImage(convertMat2QImage(foto)));
}

void MainWindow::onGetOriginalMatriculaRojaB(cv::Mat foto)
{
    ui->FotoRojosA->setPixmap(QPixmap::fromImage(convertMat2QImage(foto)));
}

void MainWindow::onGetOriginalMatriculaBlancaB(cv::Mat foto)
{
    ui->FotoBlancosA->setPixmap(QPixmap::fromImage(convertMat2QImage(foto)));
}

void MainWindow::onGetMatriculaFotoB1(QString matricula, QString confianza, bool detectada, cv::Mat foto)
{
    ui->LongMatriculaA1->setText(confianza+ "%");
    ui->MatriculaA1->setText(matricula);
    ui->FotoMatriculaA1->setPixmap(QPixmap::fromImage(convertMat2QImage(foto)));
}
void MainWindow::onGetMatriculaFotoB2(QString matricula, QString confianza, bool detectada, cv::Mat foto)
{
    ui->LongMatriculaA2->setText(confianza + "%");
    ui->MatriculaA2->setText(matricula);
    ui->FotoMatriculaA2->setPixmap(QPixmap::fromImage(convertMat2QImage(foto)));
}

void MainWindow::on_calibracionSelect_currentIndexChanged(const QString &arg1)
{
    engine->appConfig()->beginGroup(arg1);
    ui->vPlankA->setValue(engine->appConfig()->value("plankA","0").toInt());
    ui->vPlankB->setValue(engine->appConfig()->value("plankB","20").toInt());
    ui->vPlankC->setValue(engine->appConfig()->value("plankC","40").toInt());

    engine->appConfig()->endGroup();
    engine->appConfig()->sync();
}

void MainWindow::on_TestMatriculaA1_clicked()
{
    /** DEBUG **/
    updateOriginal();

    engine->getFotoMatricula(ui->calibracionSelect->currentIndex(),m_fotocamaraCVA.clone());
}

void MainWindow::on_onCalibrarA_clicked()
{
    /** DEBUG **/
    updateOriginal();
    engine->appConfig()->beginGroup(ui->calibracionSelect->currentText());
    engine->appConfig()->setValue("plankA",QString::number(ui->vPlankA->value()));
    engine->appConfig()->setValue("plankB",QString::number(ui->vPlankB->value()));
    engine->appConfig()->setValue("plankC",QString::number(ui->vPlankC->value()));
    engine->appConfig()->endGroup();
    engine->appConfig()->sync();
    engine->calibrarFoto(ui->calibracionSelect->currentIndex(),m_fotocamaraCVA.clone());
}
/** END ALRP **/

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
QString MainWindow::get_prewarp_config()
{

    /*
     *  prewarp =
     *  planar,
     * 1280.000000,
     * 720.000000,
     * 0.000000,
     * 0.000000,
     * 0.000000,
     * 1.000000,
     * 1.000000,
     * 0.000000,
     * 0.000000
    */
    /*
    prewarp.m_w = QString(m_fotocamaraCVA.cols);
    prewarp.m_h = QString(m_fotocamaraCVA.rows);
    prewarp.m_rotationx ="0";
    prewarp.m_rotationy="0";
    prewarp.m_rotationz="0";
    prewarp.m_stretchX="1";
    prewarp.m_dist="1";
    prewarp.m_panX="0";
    prewarp.m_panY="0";
*/
    //qDebug() << prewarp;
 return "";

}

/** END PREWARP **/















/** CHECK PARES PLANK **/
void MainWindow::on_vPlankA_valueChanged(int value)
{
    if (value!=0){
        if (value % 2){
            ui->vPlankA->setValue(value+1);
        }
    }
}

void MainWindow::on_vPlankA_sliderMoved(int position)
{
   if (position % 2){
            ui->vPlankA->setValue(position+1);
        }
}
void MainWindow::on_vPlankB_valueChanged(int value)
{
    if (value!=0){
        if (value % 2){
            ui->vPlankB->setValue(value+1);
        }
    }
}
void MainWindow::on_vPlankB_sliderMoved(int position)
{
    if (position % 2){
             ui->vPlankB->setValue(position+1);
         }
}

void MainWindow::on_vPlankC_valueChanged(int value)
{
    if (value!=0){
        if (value % 2){
            ui->vPlankC->setValue(value+1);
        }
    }
}

void MainWindow::on_vPlankC_sliderMoved(int position)
{
    if (position % 2){
             ui->vPlankC->setValue(position+1);
         }
}
