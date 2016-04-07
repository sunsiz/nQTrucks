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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
/*
#include <QProgressDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QtWebKitWidgets/QWebView>
*/

#include "nQTrucksEngine.h"
#include "nqtglobal.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    /** nQTrucks Lib **/
private:
    nQTrucks::nQTrucksEngine *engine;
    /** END nQTrucks Lib **/

    /** CAMARAS **/
private slots:
    void on_GuardarCamara1_clicked();
    void on_GuardarCamara2_clicked();
    void onGetFoto1(QImage foto);
    void onGetFoto2(QImage foto);
    /** END CAMARAS **/


    /** NEWSAGES I/O  **/
private:
    QPalette m_deviceNotReady_palette;
    QPalette m_deviceReady_palette;
    QPalette m_semaforoOFF_palette;
    QPalette m_semaforoRED_palette;
    QPalette m_semaforoGREEN_palette;

private slots:
    void on_actualizarIODEvicestoolButton_clicked();
    void on_guardarIODevicesPushButton1_clicked();
    void on_ioDeviceONpushButton_clicked();
    void on_ioDeviceOFFpushButton_clicked();
    void on_ioDeviceSTATUS(bool status);
    void on_ioDevicePIN10(bool value);
    void on_conectarIODevicesPushButton_clicked();
    void on_desconectarIODevicesPushButton_clicked();
    /** END NEWSAGES I/O  **/

    /** CONFIG **/
private:
    void loadconfig();
    /** END CONFIG **/
};

#endif // MAINWINDOW_H
