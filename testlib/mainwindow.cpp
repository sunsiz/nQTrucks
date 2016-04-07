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
#include <QDebug>
#include <QStringListModel>
*/
#include "nQTrucksEngine.h"

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
    loadconfig();

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

