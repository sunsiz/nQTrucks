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
#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <QMainWindow>

#include "nQTrucksEngine.h"
#include "nqtglobal.h"


namespace Ui {
class Configuracion;
}

namespace nQTrucks{
class Configuracion : public QWidget
{
    Q_OBJECT

public:
    explicit Configuracion(nQTrucksEngine *_engine=nullptr, QWidget *parent = 0);
    ~Configuracion();

private:
    Ui::Configuracion *ui;
//    bool m_running=false;
    void updateGui();

private slots:
//    void isRunning(bool clicked);

    /** nQTrucks Lib **/
private:
    nQTrucksEngine *engine;
    /** END nQTrucks Lib **/

    /** CAMARAS **/
private slots:
    void on_CamaraSelect_currentIndexChanged(int index);
    void on_GuardarCamara_clicked();
    void onGetFoto1(const MatriculaResults &_camara);
    void onGetFoto2(const MatriculaResults &_camara);
    /** END CAMARAS **/


    /** NEWSAGES I/O  **/
private slots:
    void on_actualizarSemaforos_clicked();
    void on_conectarSemaforo_clicked();
    void on_desconectarSemaforo_clicked();
    void on_guardarSemaforo_clicked();

    void on_semaforoVerde_clicked();
    void on_semaforoAmarillo_clicked();
    void on_semaforoRojo_clicked();

    void on_SemaforoConnect(bool status);
    void on_SemaforoEstadoChanged(int _color);
    /** END NEWSAGES I/O  **/

    /** BASCULAS **/
private slots:
    void on_actualizarBasculas_clicked();
    void on_conectarBascula_clicked();
    void on_desconectarBascula_clicked();
    void on_guardarBascula_clicked();
    void on_BasculaConectada(bool conectada);
    void onBascula(const Bascula &_bascula);
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
    QVector<MatriculaResults*> m_matricularesults;
    //QVector<MatriculaResults>::iterator m_matricularesults_iterator;
    void updateCalibracionGui();

    /** ALPR1 **/
private slots:
    void onGetCalibrationResult1(const MatriculaResults &_calibration_result);
    void onGetOriginalMatricula1(const MatriculaResults &_camara);
    void onReplyMatriculaResults1(const MatriculaResults &_result);
    /** END ALPR1 **/

    /** ALPR2 **/
private slots:
    void onGetCalibrationResult2(const MatriculaResults &_calibration_result);
    void onGetOriginalMatricula2(const MatriculaResults &_camara);
    void onReplyMatriculaResults2(const MatriculaResults &_result);
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

    void on_reportsTreeWidget_doubleClicked(const QModelIndex &index);
    void on_guardarEmpresa_clicked();
    void reloadEmpresa();

private:
    void loadconfig();
    /** END CONFIG **/
    // Tools *m_tools;

};

}
#endif // CONFIGURACION_H
