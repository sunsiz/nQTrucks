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
private slots:
    void on_CamaraSelect_currentIndexChanged(int index);
    void on_GuardarCamara_clicked();
    void onGetFotoCV1(const cv::Mat &fotocv);
    void onGetFotoCV2(const cv::Mat &fotocv);
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
    void on_ioDeviceSemaforoChanged(int _color);
    void on_conectarIODevicesPushButton_clicked();
    void on_desconectarIODevicesPushButton_clicked();
    /** END NEWSAGES I/O  **/

    /** BASCULAS **/
private slots:
    void on_conectarBasculaPushButton_clicked();
    void on_desconectarBasculaPushButton_clicked();
    void onBascula(Bascula _bascula);
    void on_guardarBasculaPushButton_clicked();
    void on_actualizarIBasculasButton_clicked();
    void on_BasculaConectada(bool conectada);
    /** END BASCULAS **/


    /** CALIBRACION ***************************************/
    /** GUI **/
private slots:
    void on_calibracionSelect_currentIndexChanged(int index);
    void loadPlanks(const int &index);
    void on_TestMatricula_clicked();
    void on_ActualizarCamara_clicked();
    /** END GUI **/
    /** ALPR **/
public:
    int getAlprIndex()const {return m_AlprIndex;}
    void setAlprIndex(int _AlprIndex){ m_AlprIndex = _AlprIndex;}
private:
    int m_AlprIndex=0;
    QVector<Registros::MatriculaResults> m_matricularesults;
    QVector<Registros::MatriculaResults>::iterator m_matricularesults_iterator;
    void updateCalibracionGui();

    /** ALPR1 **/
private slots:
    void onReplyMatriculaResults1(const Registros::MatriculaResults &_result);
    void onGetOriginalMatricula1(const cv::Mat &foto);
    void onGetOriginalMatriculaRoja1(const cv::Mat &foto);
    void onGetOriginalMatriculaBlanca1(const cv::Mat &foto);
    /** END ALPR1 **/

    /** ALPR2 **/
private slots:
    void onReplyMatriculaResults2(const Registros::MatriculaResults &_result);
    void onGetOriginalMatricula2(const cv::Mat &foto);
    void onGetOriginalMatriculaRoja2(const cv::Mat &foto);
    void onGetOriginalMatriculaBlanca2(const cv::Mat &foto);
    /** END NEWSAGES ALPR 2 **/
    /** PLANKs **/
private slots:
    void on_vPlankA_valueChanged(int value);
    void on_vPlankB_valueChanged(int value);
    void on_vPlankC_valueChanged(int value);
    void on_guardarPlanK_clicked();
    /** END PLANKS **/

/** END CALIBRACION ************************************************/

    /** CONFIG **/
private slots:
    void on_resetPlanK_clicked();

private:
    void loadconfig();
    /** END CONFIG **/
    nQTrucks::Tools * m_tools;
    //static QImage convertMat2QImage(const cv::Mat &src);
};

#endif // MAINWINDOW_H
