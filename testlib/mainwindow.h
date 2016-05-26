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

#include "nQTrucksEngine.h"
#include "nqtglobal.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool m_running=false;
    void updateGui();

private slots:
    void isRunning(bool clicked);

    /** nQTrucks Lib **/
private:
    nQTrucks::nQTrucksEngine *engine;
    /** END nQTrucks Lib **/

    /** CAMARAS **/
private:
    cv::Mat m_fotocamaraCVA1;
    cv::Mat m_fotocamaraCVA2;
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

    /** NEWSAGES ALPR 1 **/
private:
    cv::Mat m_fotoMatriculaA1;
    cv::Mat m_fotoMatriculaB1;
    QString m_MatriculaA1;
    QString m_MatriculaB1;
    QString m_MatriculaConfianzaA1;
    QString m_MatriculaConfianzaB1;
private slots:
    void onGetOriginalMatriculaA1(cv::Mat foto);
    void onGetOriginalMatriculaRojaA(cv::Mat foto);
    void onGetOriginalMatriculaBlancaA(cv::Mat foto);
    void onGetMatriculaFotoA1(QString matricula, QString confianza, bool detectada, cv::Mat foto, QByteArray fotoByte);
    void onGetMatriculaFotoB1(QString matricula, QString confianza, bool detectada, cv::Mat foto, QByteArray fotoByte);
    /** END NEWSAGES ALPR 1 **/

    /** NEWSAGES ALPR 2 **/
private:
    cv::Mat m_fotoMatriculaA2;
    cv::Mat m_fotoMatriculaB2;
    QString m_MatriculaA2;
    QString m_MatriculaB2;
    QString m_MatriculaConfianzaA2;
    QString m_MatriculaConfianzaB2;
private slots:
    void onGetOriginalMatriculaB1(cv::Mat foto);
    void onGetOriginalMatriculaRojaB(cv::Mat foto);
    void onGetOriginalMatriculaBlancaB(cv::Mat foto);
    void onGetMatriculaFotoA2(QString matricula, QString confianza, bool detectada, cv::Mat foto, QByteArray fotoByte);
    void onGetMatriculaFotoB2(QString matricula, QString confianza, bool detectada, cv::Mat foto, QByteArray fotoByte);
    /** END NEWSAGES ALPR 2 **/

    /** PREWARP **/
private:
    cv::Mat updateprewarp(cv::Mat img);
    void loadprewarp();
    QString get_prewarp_config();
    //cv::Mat m_fotoprewarpCVA;
    cv::Mat m_fotoprewarpCVA1;
    cv::Mat m_fotoprewarpCVA2;
    cv::Mat curWarpedImage;
    QString m_prewarp;
    nQTrucks::t_Prewarp prewarp;
private slots:
    void on_valueXA1_valueChanged(int value);
    void on_valueYA1_valueChanged(int value);
    void on_valueZA1_valueChanged(int value);
    void on_valueWA1_valueChanged(int value);
    void on_valueDA1_valueChanged(int value);
    void on_guardarPrewarp_clicked();
    /** END  PREWARP **/



    /** PLANK **/
private:
    cv::Mat m_fotoRojos1;
    cv::Mat m_fotoBlancos1;
    cv::Mat m_fotoRojos2;
    cv::Mat m_fotoblancos2;
private slots:
    void on_vPlankA_valueChanged(int value);
    void on_vPlankA_sliderMoved(int position);
    void on_vPlankB_sliderMoved(int position);
    void on_vPlankB_valueChanged(int value);
    void on_vPlankC_valueChanged(int value);
    void on_vPlankC_sliderMoved(int position);
    void on_guardarPlanK_clicked();


    /** CONFIG **/
private slots:
    void on_TestMatriculaA1_clicked();
    void on_CamaraSelect_currentIndexChanged(const QString &arg1);
    void on_calibracionSelect_currentIndexChanged(const QString &arg1);



    void on_ActualizarCamara_clicked();

    void on_calibracionSelect_currentIndexChanged(int index);

private:
    //cv::Mat m_fotocamaraCVA;
    void loadconfig();
    //void updateOriginal(); //*<< DEBUG
    /** END CONFIG **/
    static QImage convertMat2QImage(const cv::Mat &src);
};

#endif // MAINWINDOW_H
