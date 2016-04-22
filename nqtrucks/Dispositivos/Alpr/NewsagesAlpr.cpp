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
    , m_matricularesults(new t_MatriculaResults)
    , bhilo1(false)
    , bhilo2(false)
    , bhiloCalibrar1(false)
    , bhiloCalibrar2(false)
{       
    m_matricularesults->id=m_nDevice;
}


void NewsagesAlpr::setFotoCamara(const QImage &Foto) {
    m_FotoCamara = Foto;
    m_matricularesults->OrigenFoto=FotoCamara();
    m_matricularesults->OrigenFotoPrewarp=FotoCamara(); /** <<<<<< TODO CHANGE TO FUNCTION load and set prewarp **/
    emit ReplyOriginalFoto(m_matricularesults->OrigenFoto.copy());
}

/** SOLO CALIBRACION ***************************************************************************************/
void NewsagesAlpr::calibrarFoto(const QImage &Foto){

    if(!bhiloCalibrar1 && !bhiloCalibrar2){
        bhiloCalibrar1=true;
        bhiloCalibrar2=true;

        this->setFotoCamara(Foto);

        /** Crear hilos por Tipos de matriculas **/
        hiloCalibrar1 = new QThread;
        hiloCalibrar2 = new QThread;

        /** Crear tareas **/
        tareaCalibrar1 = new NewsagesAlprTask(m_nDevice,m_matricularesults->OrigenFotoPrewarp,m_settings);
        tareaCalibrar2 = new NewsagesAlprTask(m_nDevice,m_matricularesults->OrigenFotoPrewarp,m_settings);

        /** Asignar tareas a hilos **/
        tareaCalibrar1->moveToThread(hiloCalibrar1);
        tareaCalibrar2->moveToThread(hiloCalibrar2);


        /** conectar hilos con proceso BLANCOS **/
        connect( tareaCalibrar1, SIGNAL(ReplyOriginalFotoBlanca(QImage)),this,SIGNAL(ReplyOriginalFotoBlanca(QImage)));
        connect( hiloCalibrar1,  SIGNAL(started()),      tareaCalibrar1, SLOT(calibrarBlanco()) );
        connect( tareaCalibrar1, SIGNAL(workFinished()), hiloCalibrar1,  SLOT(quit()) );
        connect( tareaCalibrar1, SIGNAL(workFinished()), tareaCalibrar1, SLOT(deleteLater()) );
        connect( hiloCalibrar1,  SIGNAL(finished()),     hiloCalibrar1,  SLOT(deleteLater()) );

        std::unique_ptr<QMetaObject::Connection> pconnCalibrar1{new QMetaObject::Connection};
        QMetaObject::Connection &connCalibrar1 = *pconnCalibrar1;
        connCalibrar1=connect( hiloCalibrar1, &QThread::finished, [=](){ QObject::disconnect(connCalibrar1); bhiloCalibrar1=false; });

        /** conectar hilos con proceso ROJOS **/
        connect( tareaCalibrar2, SIGNAL(ReplyOriginalFotoRoja(QImage)),this,SIGNAL(ReplyOriginalFotoRoja(QImage)));
        connect( hiloCalibrar2,  SIGNAL(started()),      tareaCalibrar2, SLOT(calibrarRojo()) );
        connect( tareaCalibrar2, SIGNAL(workFinished()), hiloCalibrar2,  SLOT(quit()) );
        connect( tareaCalibrar2, SIGNAL(workFinished()), tareaCalibrar2, SLOT(deleteLater()) );
        connect( hiloCalibrar2,  SIGNAL(finished()),     hiloCalibrar2,  SLOT(deleteLater()) );
        std::unique_ptr<QMetaObject::Connection> pconnCalibrar2{new QMetaObject::Connection};
        QMetaObject::Connection &connCalibrar2 = *pconnCalibrar2;
        connCalibrar2=connect( hiloCalibrar2, &QThread::finished, [=](){ QObject::disconnect(connCalibrar2); bhiloCalibrar2=false; });

        /** Ejecutar Procesos **/
        hiloCalibrar1->start();
        hiloCalibrar2->start();
    }
}
/** END SOLO CALIBRACION *************************************************************************************/


/** PROCESAR ************************************************************************************************/
void NewsagesAlpr::processFoto(const QImage &Foto)
{

    if(!bhilo1 && !bhilo2){
        bhilo1=true;
        bhilo2=true;

        this->setFotoCamara(Foto);


        /** Crear hilos por Tipos de matriculas **/
        hilo1 = new QThread;
        hilo2 = new QThread;

        /** Crear tareas **/
        tarea1 = new NewsagesAlprTask(m_nDevice,m_matricularesults->OrigenFotoPrewarp,m_settings);
        tarea2 = new NewsagesAlprTask(m_nDevice,m_matricularesults->OrigenFotoPrewarp,m_settings);

        /** Asignar tareas a hilos **/
        tarea1->moveToThread(hilo1);
        tarea2->moveToThread(hilo2);


        //connect( tarea1, SIGNAL(ReplyOriginalFotoBlanca(QImage)),this,SIGNAL(ReplyOriginalFotoBlanca(QImage)));
        /** conectar hilos con proceso **/
        connect( hilo1, SIGNAL(started()), tarea1, SLOT(procesarBlancas()) );
        connect( tarea1,SIGNAL(workFinished()), hilo1, SLOT(quit()) );
        connect( tarea1, SIGNAL(workFinished()), tarea1, SLOT(deleteLater()) );
        connect( hilo1, SIGNAL(finished()), hilo1, SLOT(deleteLater()) );


        std::unique_ptr<QMetaObject::Connection> pconn1{new QMetaObject::Connection};
        QMetaObject::Connection &conn1 = *pconn1;
        conn1 =connect( hilo1, &QThread::finished, [=](){
            QObject::disconnect(conn1);
                bhilo1=false;
                if (!bhilo1 && !bhilo2){
                    emit ReplyMatriculaResults(*m_matricularesults);
                    qDebug() << m_matricularesults->MatriculaA << " y " << m_matricularesults->MatriculaB;
                }
             });

        //connect( tarea2, SIGNAL(ReplyOriginalFotoRoja(QImage)),this,SIGNAL(ReplyOriginalFotoRoja(QImage)));

        connect( hilo2, SIGNAL(started()), tarea2, SLOT(procesarRojas()) );
        connect( tarea2,SIGNAL(workFinished()), hilo2, SLOT(quit()) );
        connect( tarea2, SIGNAL(workFinished()), tarea2, SLOT(deleteLater()) );
        connect( hilo2, SIGNAL(finished()), hilo2, SLOT(deleteLater()) );

        std::unique_ptr<QMetaObject::Connection> pconn2{new QMetaObject::Connection};
        QMetaObject::Connection &conn2 = *pconn2;
        conn2 = connect( hilo2, &QThread::finished, [=](){
            QObject::disconnect(conn2);
                bhilo2=false;
                if (!bhilo1 && !bhilo2){
                    emit ReplyMatriculaResults(*m_matricularesults);
                    qDebug() << m_matricularesults->MatriculaA << " y " << m_matricularesults->MatriculaB;
                }

            });

        /** conectar respuestas con padre **/
        connect( tarea1, SIGNAL(ReplyMatriculaFoto(QString,QString,bool,QImage)),
                 this  , SIGNAL(ReplyMatriculaFoto(QString,QString,bool,QImage)));

        std::unique_ptr<QMetaObject::Connection> pconn3{new QMetaObject::Connection};
        QMetaObject::Connection &conn3 = *pconn3;
        conn3=connect( tarea1, &NewsagesAlprTask::ReplyMatriculaFoto, [=](const QString &matricula, const QString &precision ,const bool &detectada,const QImage &foto){
            QObject::disconnect(conn3);
           m_matricularesults->MatriculaA=matricula;
           m_matricularesults->MatriculaDetectedA=detectada;
           m_matricularesults->MatriculaFotoA=foto;
           m_matricularesults->MatriculaPrecisionA=precision.toDouble();

        });


        connect( tarea2, SIGNAL(ReplyMatriculaFotoRemolque(QString,QString,bool,QImage)),
                 this  , SIGNAL(ReplyMatriculaFotoRemolque(QString,QString,bool,QImage)));

        std::unique_ptr<QMetaObject::Connection> pconn4{new QMetaObject::Connection};
        QMetaObject::Connection &conn4 = *pconn4;
        conn4=connect( tarea2, &NewsagesAlprTask::ReplyMatriculaFotoRemolque, [=](const QString &matricula, const QString &precision ,const bool &detectada,const QImage &foto){
           QObject::disconnect(conn4);
           m_matricularesults->MatriculaB=matricula;
           m_matricularesults->MatriculaDetectedB=detectada;
           m_matricularesults->MatriculaFotoB=foto;
           m_matricularesults->MatriculaPrecisionB=precision.toDouble();

        });

        /** Ejecutar Procesos **/
        hilo1->start();
        hilo2->start();
    }

}
/** END PROCESAR ********************************************************************************************************/


    } /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/

























