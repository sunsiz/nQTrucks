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
//#include "prewarp.h"

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
    bool m_running=false;
private slots:
    void isRunning(bool clicked);

    /** nQTrucks Lib **/
private:
    nQTrucks::nQTrucksEngine *engine;
    /** END nQTrucks Lib **/

    /** CAMARAS **/
private slots:
    void on_GuardarCamara_clicked();
    void onGetFotoCV1(cv::Mat fotocv,cv::Mat fotorgbcv, QImage foto);
    void onGetFotoCV2(cv::Mat fotocv,cv::Mat fotorgbcv, QImage foto);
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

    /** BASCULAS **/
private slots:
    void on_conectarBasculaPushButton_clicked();
    void on_desconectarBasculaPushButton_clicked();
    void onBascula(t_Bascula _bascula);
    void on_guardarBasculaPushButton_clicked();
    void on_actualizarIBasculasButton_clicked();
    void on_BasculaConectada(bool conectada);
    /** END BASCULAS **/

    /** NEWSAGES ALPR A **/
private slots:
    void onGetOriginalMatriculaA1(cv::Mat foto);
    void onGetOriginalMatriculaRojaA(cv::Mat foto);
    void onGetOriginalMatriculaBlancaA(cv::Mat foto);
    void onGetMatriculaFotoA1(QString matricula, QString confianza, bool detectada, cv::Mat foto);
    void onGetMatriculaFotoA2(QString matricula, QString confianza, bool detectada, cv::Mat foto);
    /** END NEWSAGES ALPR A **/

    /** NEWSAGES ALPR B **/
private slots:
    void onGetOriginalMatriculaB1(cv::Mat foto);
    void onGetOriginalMatriculaRojaB(cv::Mat foto);
    void onGetOriginalMatriculaBlancaB(cv::Mat foto);
    void onGetMatriculaFotoB1(QString matricula, QString confianza, bool detectada, cv::Mat foto);
    void onGetMatriculaFotoB2(QString matricula, QString confianza, bool detectada, cv::Mat foto);
    /** END NEWSAGES ALPR B **/

    /** PREWARP **/
private:

    cv::Mat m_fotoprewarpCVA;
    QString m_prewarp;
    QString get_prewarp_config();
    nQTrucks::t_Prewarp prewarp;




    /** END  PREWARP **/









    /** CONFIG **/
private slots:
    void on_vPlankA_valueChanged(int value);
    void on_vPlankA_sliderMoved(int position);
    void on_vPlankB_sliderMoved(int position);
    void on_vPlankB_valueChanged(int value);
    void on_vPlankC_valueChanged(int value);
    void on_vPlankC_sliderMoved(int position);

    void on_TestMatriculaA1_clicked();
    void on_onCalibrarA_clicked();
    void on_CamaraSelect_currentIndexChanged(const QString &arg1);
    void on_calibracionSelect_currentIndexChanged(const QString &arg1);

private:
    cv::Mat m_fotocamaraCVA;
    void loadconfig();
    void updateOriginal(); //*<< DEBUG
    /** END CONFIG **/
    static QImage convertMat2QImage(const cv::Mat &src);
};

#endif // MAINWINDOW_H
