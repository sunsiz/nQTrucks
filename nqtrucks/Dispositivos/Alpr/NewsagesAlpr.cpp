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

#include "NewsagesAlpr.h"
#include <QDebug>

#include <QByteArray>
#include <QBuffer>

#include <QDir>
#include <QCoreApplication>

#include <memory>

namespace nQTrucks {
    namespace Devices {

NewsagesAlpr::NewsagesAlpr(int nDevice, QSettings *_appsettings, QObject *parent)
    : QObject(parent)
    , m_nDevice(nDevice)
    , m_settings(_appsettings)
    , bhiloCalibrar1(false)
    , bhiloCalibrar2(false)
    , bhilo1(false)
    , bhilo2(false)
{
    m_results.resize(3);
    m_results[0] = new t_MatriculaResults;
    m_results[1] = new t_MatriculaResults;
    m_results[2] = new t_MatriculaResults;

    m_results[0]->id=m_nDevice;
    m_results[1]->id=m_nDevice;
    m_results[2]->id=m_nDevice;

}

NewsagesAlpr::~NewsagesAlpr()
{
 m_FotoCamara.release();
}

void NewsagesAlpr::setFotoCamara(const cv::Mat &Foto) {
    m_FotoCamara = Foto.clone();
    m_results[0]->OrigenFoto=Foto.clone();
    m_results[0]->OrigenFotoPrewarp=FotoCamara(); /** <<<<<< TODO CHANGE TO FUNCTION load and set prewarp **/
    emit ReplyOriginalFoto(FotoCamara());
}

/** SOLO CALIBRACION ***************************************************************************************/
void NewsagesAlpr::calibrarFoto(const cv::Mat &Foto){

    if(!bhiloCalibrar1 && !bhiloCalibrar2){
        bhiloCalibrar1=true;
        bhiloCalibrar2=true;

        //this->setFotoCamara(Foto);

        /** Crear hilos por Tipos de matriculas **/
        hiloCalibrar1 = new QThread;
        hiloCalibrar2 = new QThread;

        /** Crear tareas **/
        tareaCalibrar1 = new NewsagesAlprTask(m_nDevice, ALPR_PLANCK_BLANCO, Foto, m_results[1], m_settings);
        tareaCalibrar2 = new NewsagesAlprTask(m_nDevice, ALPR_PLANCK_ROJO,   Foto, m_results[1],  m_settings);

        /** Asignar tareas a hilos **/
        tareaCalibrar1->moveToThread(hiloCalibrar1);
        tareaCalibrar2->moveToThread(hiloCalibrar2);


        /** conectar hilos con proceso BLANCOS **/
        connect( tareaCalibrar1, &NewsagesAlprTask::ReplyOriginalFotoBlanca, [=](const cv::Mat &_PlanckBlancos){
            tareaCalibrar1->workFinished();
            });

        connect( hiloCalibrar1,  SIGNAL(started()),      tareaCalibrar1, SLOT(calibrar()) );
        connect( tareaCalibrar1, SIGNAL(workFinished()), hiloCalibrar1,  SLOT(quit()) );
        connect( tareaCalibrar1, SIGNAL(workFinished()), tareaCalibrar1, SLOT(deleteLater()) );
        connect( hiloCalibrar1,  SIGNAL(finished()),     hiloCalibrar1,  SLOT(deleteLater()) );

        std::unique_ptr<QMetaObject::Connection> pconnCalibrar1{new QMetaObject::Connection};
        QMetaObject::Connection &connCalibrar1 = *pconnCalibrar1;
        connCalibrar1=connect( hiloCalibrar1, &QThread::finished, [=](){
            QObject::disconnect(connCalibrar1);
            bhiloCalibrar1=false;
            if (!bhiloCalibrar1 && !bhiloCalibrar2){
                onCalibrarFotoFinished();
            }

        });

        /** conectar hilos con proceso ROJOS **/
        connect( tareaCalibrar2, &NewsagesAlprTask::ReplyOriginalFotoRoja, [=](const cv::Mat &_PlanckRojos){
            tareaCalibrar2->workFinished();
            });

        connect( hiloCalibrar2,  SIGNAL(started()),      tareaCalibrar2, SLOT(calibrar()) );
        connect( tareaCalibrar2, SIGNAL(workFinished()), hiloCalibrar2,  SLOT(quit()) );
        connect( tareaCalibrar2, SIGNAL(workFinished()), tareaCalibrar2, SLOT(deleteLater()) );
        connect( hiloCalibrar2,  SIGNAL(finished()),     hiloCalibrar2,  SLOT(deleteLater()) );
        std::unique_ptr<QMetaObject::Connection> pconnCalibrar2{new QMetaObject::Connection};
        QMetaObject::Connection &connCalibrar2 = *pconnCalibrar2;
        connCalibrar2=connect( hiloCalibrar2, &QThread::finished, [=](){
            QObject::disconnect(connCalibrar2);
            bhiloCalibrar2=false;
            if (!bhiloCalibrar1 && !bhiloCalibrar2){
                onCalibrarFotoFinished();
            }            
        });

        /** Ejecutar Procesos **/
        hiloCalibrar1->start();
        hiloCalibrar2->start();
    }
}

void NewsagesAlpr::onCalibrarFotoFinished(){
    this->ReplyOriginalFotoBlanca(m_results[1]->OrigenFotoBlanca);
    this->ReplyOriginalFotoRoja(  m_results[1]->OrigenFotoRoja);
}
/** END SOLO CALIBRACION *************************************************************************************/


/** PROCESAR ************************************************************************************************/
void NewsagesAlpr::processFoto(const cv::Mat &Foto)
{

    if(!bhilo1 && !bhilo2){
        bhilo1=true;
        bhilo2=true;

        //this->setFotoCamara(Foto);

        /** Crear hilos por Tipos de matriculas **/
        hilo1 = new QThread;
        hilo2 = new QThread;

        /** Crear tareas **/
        tarea1 = new NewsagesAlprTask(m_nDevice, ALPR_PLANCK_BLANCO, Foto, m_results[2], m_settings);
        tarea2 = new NewsagesAlprTask(m_nDevice, ALPR_PLANCK_ROJO,   Foto, m_results[2], m_settings);

        /** Asignar tareas a hilos **/
        tarea1->moveToThread(hilo1);
        tarea2->moveToThread(hilo2);

        /** conectar hilos con proceso **/
        connect( hilo1,  SIGNAL(started()),      tarea1, SLOT(procesar()) );
        connect( tarea1, SIGNAL(workFinished()), hilo1,  SLOT(quit()) );
        connect( tarea1, SIGNAL(workFinished()), tarea1, SLOT(deleteLater()) );
        connect( hilo1,  SIGNAL(finished()),     hilo1,  SLOT(deleteLater()) );

        std::unique_ptr<QMetaObject::Connection> pconn1{new QMetaObject::Connection};
        QMetaObject::Connection &conn1 = *pconn1;
        conn1 =connect( hilo1, &QThread::finished, [=](){
            QObject::disconnect(conn1);
                bhilo1=false;
                if (!bhilo1 && !bhilo2){
                    onProcesarFotoFinished();
                }
             });
        connect( hilo2, SIGNAL(started()), tarea2, SLOT(procesar()) );
        connect( tarea2,SIGNAL(workFinished()), hilo2, SLOT(quit()) );
        connect( tarea2, SIGNAL(workFinished()), tarea2, SLOT(deleteLater()) );
        connect( hilo2, SIGNAL(finished()), hilo2, SLOT(deleteLater()) );

        std::unique_ptr<QMetaObject::Connection> pconn2{new QMetaObject::Connection};
        QMetaObject::Connection &conn2 = *pconn2;
        conn2 = connect( hilo2, &QThread::finished, [=](){
            QObject::disconnect(conn2);
                bhilo2=false;
                if (!bhilo1 && !bhilo2){
                    onProcesarFotoFinished();
                }
            });

        /** conectar respuestas con padre **/
        connect(tarea1, SIGNAL(ReplyMatriculaFoto()),tarea1,SIGNAL(workFinished()));
        connect(tarea2, SIGNAL(ReplyMatriculaFoto()),tarea2,SIGNAL(workFinished()));
        /** Ejecutar Procesos **/
        hilo1->start();
        hilo2->start();
    }

}

void NewsagesAlpr::onProcesarFotoFinished(){

    m_results[0]->OrigenFoto              = m_results[2]->OrigenFoto;
    m_results[0]->OrigenFotoRoja          = m_results[2]->OrigenFotoRoja;
    m_results[0]->OrigenFotoBlanca        = m_results[2]->OrigenFotoBlanca;
    m_results[0]->OrigenFotoPrewarp       = m_results[2]->OrigenFotoPrewarp;
    m_results[0]->OrigenFotoResize        = m_results[2]->OrigenFotoResize;
    m_results[0]->OrigenFotoResizeByte    = m_results[2]->OrigenFotoResizeByte;
    m_results[0]->MatriculaA              = m_results[2]->MatriculaA;
    m_results[0]->MatriculaDetectedA      = m_results[2]->MatriculaDetectedA;
    m_results[0]->MatriculaFotoA          = m_results[2]->MatriculaFotoA;
    m_results[0]->MatriculaFotoAByte      = m_results[2]->MatriculaFotoAByte;
    m_results[0]->MatriculaPrecisionA     = m_results[2]->MatriculaPrecisionA;
    m_results[0]->MatriculaPrecisionAs    = m_results[2]->MatriculaPrecisionAs;
    m_results[0]->MatriculaB              = m_results[2]->MatriculaB;
    m_results[0]->MatriculaDetectedB      = m_results[2]->MatriculaDetectedB;
    m_results[0]->MatriculaFotoB          = m_results[2]->MatriculaFotoB;
    m_results[0]->MatriculaFotoBByte      = m_results[2]->MatriculaFotoBByte;
    m_results[0]->MatriculaPrecisionB     = m_results[2]->MatriculaPrecisionB;
    m_results[0]->MatriculaPrecisionBs    = m_results[2]->MatriculaPrecisionBs;

    emit ReplyMatriculaResults(*m_results[0]);
    qDebug() << "Device:" << m_results[0]->id << endl
             << "\t Matricula1:"   << m_results[0]->MatriculaA
             << "precision:"<<m_results[0]->MatriculaPrecisionAs<<"%"<< "patron:"    << m_results[0]->MatriculaDetectedA << endl
             << "\t Matricula2:"<<m_results[0]->MatriculaB
             << "precision:" << m_results[0]->MatriculaPrecisionBs << "%" << "patron:" << m_results[0]->MatriculaDetectedB << endl;

}

/** END PROCESAR ********************************************************************************************************/


    } /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/
