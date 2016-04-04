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
    , m_config(new QSettings)
{
    ui->setupUi(this);

    engine  = new nQTrucks::nQTrucksEngine(this);

    QStringList lcamaras = engine->getTiposCamaras();
    ui->camaraComboBox1->addItems(lcamaras);
    ui->camaraComboBox2->addItems(lcamaras);

    connect(engine,SIGNAL(CamaraIPFoto1(QImage)),this,SLOT(onGetFoto1(QImage)));
    connect(engine,SIGNAL(CamaraIPFoto2(QImage)),this,SLOT(onGetFoto2(QImage)));

    loadconfig();

}

MainWindow::~MainWindow()
{
    delete ui;
}

/** SETTINGS **/
void MainWindow::loadconfig()
{
    m_config = engine->appConfig();
    m_config->beginGroup(CAMARA1);

    ui->camaraComboBox1->setCurrentIndex(m_config->value("tipo").toInt());
    ui->hostLineEdit1->setText(m_config->value("host").toString());
    ui->puertoLineEdit1->setText(m_config->value("port").toString());
    ui->usuarioLineEdit1->setText(m_config->value("user").toString());
    ui->passwdLineEdit1->setText(m_config->value("pass").toString());

    m_config->endGroup();



    m_config->beginGroup(CAMARA2);

    ui->camaraComboBox2->setCurrentIndex(m_config->value("tipo").toInt());
    ui->hostLineEdit2->setText(m_config->value("host").toString());
    ui->puertoLineEdit2->setText(m_config->value("port").toString());
    ui->usuarioLineEdit2->setText(m_config->value("user").toString());
    ui->passwdLineEdit2->setText(m_config->value("pass").toString());

    m_config->endGroup();
}

void MainWindow::on_guardarPushButton1_clicked()
{
    m_config->beginGroup(CAMARA1);

    m_config->setValue("tipo",ui->camaraComboBox1->currentIndex());
    m_config->setValue("host",ui->hostLineEdit1->text());
    m_config->setValue("port",ui->puertoLineEdit1->text());
    m_config->setValue("user",ui->usuarioLineEdit1->text());
    m_config->setValue("pass",ui->passwdLineEdit1->text());
    m_config->endGroup();

    engine->getCamaraFoto(1);

}
void MainWindow::on_guardarPushButton2_clicked()
{
    m_config->beginGroup(CAMARA2);

    m_config->setValue("tipo",ui->camaraComboBox2->currentIndex());
    m_config->setValue("host",ui->hostLineEdit2->text());
    m_config->setValue("port",ui->puertoLineEdit2->text());
    m_config->setValue("user",ui->usuarioLineEdit2->text());
    m_config->setValue("pass",ui->passwdLineEdit2->text());
    m_config->endGroup();

    engine->getCamaraFoto(2);
}
/** END SETTINGS **/

void MainWindow::onGetFoto1(QImage foto)
{
    ui->camaraLabel1->setPixmap(QPixmap::fromImage(foto));
}

void MainWindow::onGetFoto2(QImage foto)
{
    ui->camaraLabel2->setPixmap(QPixmap::fromImage(foto));
}
