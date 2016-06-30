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
    m_results[0] = new Registros::MatriculaResults; //emitir
    m_results[0]->setId(m_nDevice);

    m_results[1] = new Registros::MatriculaResults; //calibrar
    m_results[1]->setId(m_nDevice);

    m_results[2] = new Registros::MatriculaResults; //procesar
    m_results[2]->setId(m_nDevice);


}

NewsagesAlpr::~NewsagesAlpr(){
    m_results[0]->clear();
    m_results[1]->clear();
    m_results[2]->clear();
    delete m_results[0];
    delete m_results[1];
    delete m_results[2];
    m_results.detach();
 //m_results.detach();
}

void NewsagesAlpr::setFotoCamara(const Registros::Camara &_camara) {
    m_results[0]->camara->setCamara(_camara);
    m_results[1]->camara->setCamara(_camara);
    m_results[2]->camara->setCamara(_camara);
    emit ReplyOriginalFoto(*m_results[0]->camara);
}

/** SOLO CALIBRACION ***************************************************************************************/
void NewsagesAlpr::calibrarFoto(const Registros::Camara &_camara){

    if(!bhiloCalibrar1 && !bhiloCalibrar2){
        bhiloCalibrar1=true;
        bhiloCalibrar2=true;

        //this->setFotoCamara(Foto);

        /** Crear hilos por Tipos de matriculas **/
        hiloCalibrar1 = new QThread();
        hiloCalibrar2 = new QThread();

        /** Crear tareas **/
        m_results[1]->camara->setCamara(_camara);
        tareaCalibrar1 = new NewsagesAlprTask(m_nDevice, ALPR_PLANCK_BLANCO, m_results[1], m_settings);
        tareaCalibrar2 = new NewsagesAlprTask(m_nDevice, ALPR_PLANCK_ROJO,   m_results[1], m_settings);

        /** Asignar tareas a hilos **/
        tareaCalibrar1->moveToThread(hiloCalibrar1);
        tareaCalibrar2->moveToThread(hiloCalibrar2);


        /** conectar hilos con proceso BLANCOS **/
        connect( tareaCalibrar1, &NewsagesAlprTask::ReplyOriginalFotoBlanca, [=](const cv::Mat &_PlanckBlancos){
            Q_UNUSED(_PlanckBlancos);
            tareaCalibrar1->workFinished();
            });

        connect( hiloCalibrar1,  &QThread::started,               tareaCalibrar1, &NewsagesAlprTask::calibrar );
        connect( tareaCalibrar1, &NewsagesAlprTask::workFinished, hiloCalibrar1,  &QThread::quit );
        connect( tareaCalibrar1, &NewsagesAlprTask::workFinished, tareaCalibrar1, &QObject::deleteLater );
        connect( hiloCalibrar1,  &QThread::finished,              hiloCalibrar1,  &QObject::deleteLater );
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
            Q_UNUSED(_PlanckRojos);
            tareaCalibrar2->workFinished();
            });

        connect( hiloCalibrar2,  &QThread::started,               tareaCalibrar2, &NewsagesAlprTask::calibrar );
        connect( tareaCalibrar2, &NewsagesAlprTask::workFinished, hiloCalibrar2,  &QThread::quit );
        connect( tareaCalibrar2, &NewsagesAlprTask::workFinished, tareaCalibrar2, &QObject::deleteLater );
        connect( hiloCalibrar2,  &QThread::finished,              hiloCalibrar2,  &QObject::deleteLater );
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
    m_results[1]->convertirFotos();
    this->ReplyMatriculaCalibrationResults(*m_results[1]);
}
/** END SOLO CALIBRACION *************************************************************************************/


/** PROCESAR ************************************************************************************************/
void NewsagesAlpr::processFoto(const Registros::Camara &_camara)
{

    if(!bhilo1 && !bhilo2){
        bhilo1=true;
        bhilo2=true;

        //this->setFotoCamara(Foto);

        /** Crear hilos por Tipos de matriculas **/
        hilo1 = new QThread;
        hilo2 = new QThread;

        /** Crear tareas **/
        m_results[2]->camara->setCamara(_camara);
        tarea1 = new NewsagesAlprTask(m_nDevice, ALPR_PLANCK_BLANCO, m_results[2], m_settings);
        tarea2 = new NewsagesAlprTask(m_nDevice, ALPR_PLANCK_ROJO,   m_results[2], m_settings);

        /** Asignar tareas a hilos **/
        tarea1->moveToThread(hilo1);
        tarea2->moveToThread(hilo2);

        /** conectar hilos con proceso **/
        connect( hilo1,  &QThread::started,               tarea1, &NewsagesAlprTask::procesar );
        connect( tarea1, &NewsagesAlprTask::workFinished, hilo1,  &QThread::quit );
        connect( tarea1, &NewsagesAlprTask::workFinished, tarea1, &QObject::deleteLater );
        connect( hilo1,  &QThread::finished,              hilo1,  &QObject::deleteLater );

        std::unique_ptr<QMetaObject::Connection> pconn1{new QMetaObject::Connection};
        QMetaObject::Connection &conn1 = *pconn1;
        conn1=connect(tarea1,  &NewsagesAlprTask::ReplyMatriculaFoto, [=](){
            QObject::disconnect(conn1);
            bhilo1=false;
            if (!bhilo1 && !bhilo2){
                onProcesarFotoFinished();
            }
            tarea1->workFinished();
        });


        connect( hilo2, &QThread::started,               tarea2,&NewsagesAlprTask::procesar );
        connect( tarea2,&NewsagesAlprTask::workFinished, hilo2, &QThread::quit );
        connect( tarea2,&NewsagesAlprTask::workFinished, tarea2,&QObject::deleteLater );
        connect( hilo2, &QThread::finished,              hilo2, &QObject::deleteLater );

        std::unique_ptr<QMetaObject::Connection> pconn2{new QMetaObject::Connection};
        QMetaObject::Connection &conn2 = *pconn2;
        conn2 = connect(tarea2,  &NewsagesAlprTask::ReplyMatriculaFoto, [=](){
            QObject::disconnect(conn2);
            bhilo2=false;
            if (!bhilo1 && !bhilo2){
                onProcesarFotoFinished();
            }
            tarea2->workFinished();
        });

        /** conectar respuestas con padre **/
        /** Ejecutar Procesos **/
        hilo1->start(); /** MEMORY LEAK **/
        hilo2->start(); /** MEMORY LEAK **/
    }

}

void NewsagesAlpr::onProcesarFotoFinished(){
    m_results[2]->convertirFotos(); /** MEMORY LEAK **/
    emit ReplyMatriculaResults(*m_results[2]);
    qDebug() << "Device:"           << m_results[2]->getId() << endl
             << "\t Matricula1:"    << m_results[2]->getMatriculaA()
             << "precision:"        << m_results[2]->getMatriculaPrecisionAs()<<"%"<< "patron:"    << m_results[2]->getMatriculaDetectedA() << endl
             << "\t Matricula2:"    << m_results[2]->getMatriculaB()
             << "precision:"        << m_results[2]->getMatriculaPrecisionBs() << "%" << "patron:" << m_results[2]->getMatriculaDetectedB() << endl;
}

/** END PROCESAR ********************************************************************************************************/


    } /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/
