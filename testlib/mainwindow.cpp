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

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <QDebug>
#include "nQTrucksEngine.h"

//#include "alpr.h"

#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    //, m_config(new QSettings)
{
    ui->setupUi(this);

    engine  = new nQTrucks::nQTrucksEngine(this);

    connect(engine,SIGNAL(CamaraIPFoto1(QImage)),this,SLOT(onGetFoto1(QImage)));
    connect(engine,SIGNAL(CamaraIPFoto2(QImage)),this,SLOT(onGetFoto2(QImage)));
    connect(engine,SIGNAL(IODevicesStatusChanged(bool)),this,SLOT(on_ioDeviceSTATUS(bool)));
    connect(engine,SIGNAL(IODevicesPIN10Changed(bool)),this,SLOT(on_ioDevicePIN10(bool)));
    connect(engine,SIGNAL(BasculaStatus(bool)),this,SLOT(on_BasculaConectada(bool)));
    connect(engine,SIGNAL(BasculaChanged(t_Bascula)),this,SLOT(onBascula(t_Bascula)));
    loadconfig();

  //  testalpr();

}

MainWindow::~MainWindow()
{
    delete ui;
}


/** SETTINGS **/
void MainWindow::loadconfig()
{
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
void MainWindow::onGetFoto1(QImage foto)
{
    ui->camaraLabel1->setPixmap(QPixmap::fromImage(foto));
}

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



void MainWindow::testalpr()
{
//    // Initialize the library using United States style license plates.
//    // You can use other countries/regions as well (for example: "eu", "au", or "kr")
//    alpr::Alpr openalpr("eu", "config/openalpr.conf");

//    // Optionally specify the top N possible plates to return (with confidences).  Default is 10
//    openalpr.setTopN(1);

//    // Optionally, provide the library with a region for pattern matching.  This improves accuracy by
//    // comparing the plate text with the regional pattern.
//    openalpr.setDefaultRegion("es");

//    openalpr.setPrewarp("");
//    // Make sure the library loaded before continuing.
//    // For example, it could fail if the config/runtime_data is not found
//    if (openalpr.isLoaded() == false)
//    {
//        std::cerr << "Error loading OpenALPR" << std::endl;
//        //return;
//    }
//    QString file ="matriculas/image.jpg";
//    // Recognize an image file.  You could alternatively provide the image bytes in-memory.
//    alpr::AlprResults results = openalpr.recognize(file.toStdString()    );

//    // Iterate through the results.  There may be multiple plates in an image,
//    // and each plate return sthe top N candidates.
//    for (int i = 0; i < results.plates.size(); i++)
//    {
//      alpr::AlprPlateResult plate = results.plates[i];
//      //std::cout << "plate" << i << ": " << plate.topNPlates.size() << " results" << std::endl;

//        for (int k = 0; k < plate.topNPlates.size(); k++)
//        {
//          alpr::AlprPlate candidate = plate.topNPlates[k];
//          std::cout << "    - " << candidate.characters << "\t confidence: " << candidate.overall_confidence;
//          //std::cout << "\t pattern_match: " << candidate.matches_template << std::endl;
//        }
//    }

}

