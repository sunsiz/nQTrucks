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
    connect(engine,SIGNAL(CamaraIPFoto1(QImage)),this,SLOT(onGetFoto1(QImage)));
    connect(engine,SIGNAL(CamaraIPFoto2(QImage)),this,SLOT(onGetFoto2(QImage)));

    connect(engine,SIGNAL(IODevicesStatusChanged(bool)),this,SLOT(on_ioDeviceSTATUS(bool)));
    connect(engine,SIGNAL(IODevicesPIN10Changed(bool)),this,SLOT(on_ioDevicePIN10(bool)));

    connect(engine,SIGNAL(BasculaStatus(bool)),this,SLOT(on_BasculaConectada(bool)));
    connect(engine,SIGNAL(BasculaChanged(t_Bascula)),this,SLOT(onBascula(t_Bascula)));

    connect(engine,SIGNAL(ReplyOriginalFotoA(QImage)),this,SLOT(onGetOriginalMatriculaA1(QImage)));

    connect(engine,SIGNAL(ReplyOriginalFotoRojaA(QImage)),this,SLOT(onGetOriginalMatriculaRojaA(QImage)));
    connect(engine,SIGNAL(ReplyOriginalFotoBlancaA(QImage)),this,SLOT(onGetOriginalMatriculaBlancaA(QImage)));

    connect(engine, SIGNAL(ReplyMatriculaFotoA1(QString,QString,bool,QImage)),
            this  , SLOT(onGetMatriculaFotoA1(  QString,QString,bool,QImage)));

    connect(engine, SIGNAL(ReplyMatriculaFotoA2(QString,QString,bool,QImage)),
            this  , SLOT(onGetMatriculaFotoA2(  QString,QString,bool,QImage)));

    loadconfig();

    /** DEBUG **/
    QString filename ="matriculas/r1.jpg";
    m_fotocamara = QImage(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(filename));
    ui->FotoOriginalA->setPixmap(QPixmap::fromImage(m_fotocamara));

}

MainWindow::~MainWindow()
{
    delete ui;
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
    QStringList l_Camaras = engine->getTiposCamaras();
    ui->camaraComboBox1->addItems(l_Camaras);
    ui->camaraComboBox2->addItems(l_Camaras);

    engine->appConfig()->beginGroup(CAMARA1);
    ui->camaraComboBox1->setCurrentIndex(engine->appConfig()->value("tipo","0").toInt());
    ui->hostLineEdit1->setText(engine->appConfig()->value("host").toString());
    ui->puertoLineEdit1->setText(engine->appConfig()->value("port").toString());
    ui->usuarioLineEdit1->setText(engine->appConfig()->value("user").toString());
    ui->passwdLineEdit1->setText(engine->appConfig()->value("pass").toString());
    engine->appConfig()->endGroup();
    engine->appConfig()->sync();

    engine->appConfig()->beginGroup(CAMARA2);
    ui->camaraComboBox2->setCurrentIndex(engine->appConfig()->value("tipo","0").toInt());
    ui->hostLineEdit2->setText(engine->appConfig()->value("host").toString());
    ui->puertoLineEdit2->setText(engine->appConfig()->value("port").toString());
    ui->usuarioLineEdit2->setText(engine->appConfig()->value("user").toString());
    ui->passwdLineEdit2->setText(engine->appConfig()->value("pass").toString());
    engine->appConfig()->endGroup();
    engine->appConfig()->sync();
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
    engine->appConfig()->beginGroup(CAMARA1);
    ui->vPlankA1->setValue(engine->appConfig()->value("plankA","0").toInt());
    ui->vPlankB1->setValue(engine->appConfig()->value("plankB","20").toInt());
    ui->vPlankC1->setValue(engine->appConfig()->value("plankC","40").toInt());
    engine->appConfig()->endGroup();
    engine->appConfig()->sync();

    /** END ALPR **/


}

void MainWindow::on_GuardarCamara1_clicked()
{
    engine->appConfig()->beginGroup(CAMARA1);
    engine->appConfig()->setValue("tipo",QString::number(ui->camaraComboBox1->currentIndex()));
    engine->appConfig()->setValue("host",ui->hostLineEdit1->text());
    engine->appConfig()->setValue("port",ui->puertoLineEdit1->text());
    engine->appConfig()->setValue("user",ui->usuarioLineEdit1->text());
    engine->appConfig()->setValue("pass",ui->passwdLineEdit1->text());
    engine->appConfig()->endGroup();
    engine->appConfig()->sync();
    engine->getCamaraFoto(1);

}
void MainWindow::on_GuardarCamara2_clicked()
{
    engine->appConfig()->beginGroup(CAMARA2);
    engine->appConfig()->setValue("tipo",QString::number(ui->camaraComboBox2->currentIndex()));
    engine->appConfig()->setValue("host",ui->hostLineEdit2->text());
    engine->appConfig()->setValue("port",ui->puertoLineEdit2->text());
    engine->appConfig()->setValue("user",ui->usuarioLineEdit2->text());
    engine->appConfig()->setValue("pass",ui->passwdLineEdit2->text());
    engine->appConfig()->endGroup();
    engine->appConfig()->sync();
    engine->getCamaraFoto(2);
}
/** END SETTINGS **/


/** CAMARAS **/
    /** CAMARA1 **/
void MainWindow::onGetFoto1(QImage foto)
{
    ui->camaraLabel1->setPixmap(QPixmap::fromImage(foto));
}

    /** CAMARA2 **/
void MainWindow::onGetFoto2(QImage foto)
{
    ui->camaraLabel2->setPixmap(QPixmap::fromImage(foto));
}
/** END CAMARAS **/



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

    /*
    qDebug() <<  " Estado: "  << _bascula.bEstado << endl <<
                 " Bruto:  "  << _bascula.iBruto  << endl <<
                 " Tara:   "  << _bascula.iTara   << endl <<
                 " Neto:   "  << _bascula.iNeto   << endl ;
    */

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

/** ALRP **/
void MainWindow::onGetOriginalMatriculaA1(QImage foto)
{
    ui->FotoOriginalA->setPixmap(QPixmap::fromImage(foto));
}

void MainWindow::onGetOriginalMatriculaRojaA(QImage foto)
{
    ui->FotoRojosA->setPixmap(QPixmap::fromImage(foto));
}

void MainWindow::onGetOriginalMatriculaBlancaA(QImage foto)
{
    ui->FotoBlancosA->setPixmap(QPixmap::fromImage(foto));
}

void MainWindow::onGetMatriculaFotoA1(QString matricula, QString confianza, bool detectada, QImage foto)
{
    ui->LongMatriculaA1->setText(confianza+ "%");
    ui->MatriculaA1->setText(matricula);
    ui->FotoMatriculaA1->setPixmap(QPixmap::fromImage(foto));
}
void MainWindow::onGetMatriculaFotoA2(QString matricula, QString confianza, bool detectada, QImage foto)
{

    ui->LongMatriculaA2->setText(confianza + "%");
    ui->MatriculaA2->setText(matricula);
    ui->FotoMatriculaA2->setPixmap(QPixmap::fromImage(foto));
}


void MainWindow::on_TestMatriculaA1_clicked()
{
    /** DEBUG **/
    QString filename ="matriculas/r1.jpg";
    m_fotocamara = QImage(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(filename));
    ui->FotoOriginalA->setPixmap(QPixmap::fromImage(m_fotocamara));

    engine->getFotoMatricula(0,m_fotocamara);
}

void MainWindow::on_onCalibrarA_clicked()
{
    /** DEBUG **/
    QString filename ="matriculas/r1.jpg";
    m_fotocamara = QImage(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(filename));
    ui->FotoOriginalA->setPixmap(QPixmap::fromImage(m_fotocamara));

    engine->appConfig()->beginGroup(CAMARA1);
    engine->appConfig()->setValue("plankA",QString::number(ui->vPlankA1->value()));
    engine->appConfig()->setValue("plankB",QString::number(ui->vPlankB1->value()));
    engine->appConfig()->setValue("plankC",QString::number(ui->vPlankC1->value()));
    engine->appConfig()->endGroup();
    engine->appConfig()->sync();

    engine->calibrarFoto(0,m_fotocamara);
}
/** END ALRP **/
