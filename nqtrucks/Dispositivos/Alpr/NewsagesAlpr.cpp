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
    m_matricularesults = new t_MatriculaResults;
    m_matricularesults->id=m_nDevice;
}

NewsagesAlpr::~NewsagesAlpr()
{
 m_FotoCamara.release();
}

void NewsagesAlpr::setFotoCamara(const cv::Mat &Foto) {
    m_FotoCamara = Foto.clone();
    m_matricularesults->OrigenFoto=Foto.clone();
    m_matricularesults->OrigenFotoPrewarp=FotoCamara(); /** <<<<<< TODO CHANGE TO FUNCTION load and set prewarp **/
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
        tareaCalibrar1 = new NewsagesAlprTask(m_nDevice, ALPR_PLANCK_BLANCO, Foto, m_settings);
        tareaCalibrar2 = new NewsagesAlprTask(m_nDevice, ALPR_PLANCK_ROJO,   Foto, m_settings);

        /** Asignar tareas a hilos **/
        tareaCalibrar1->moveToThread(hiloCalibrar1);
        tareaCalibrar2->moveToThread(hiloCalibrar2);


        /** conectar hilos con proceso BLANCOS **/
        connect( tareaCalibrar1, &NewsagesAlprTask::ReplyOriginalFotoBlanca, [=](const cv::Mat &_PlanckBlancos){
            this->ReplyOriginalFotoBlanca(_PlanckBlancos);
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
//        connect( tareaCalibrar2, SIGNAL(ReplyOriginalFotoRoja(cv::Mat)),this,SIGNAL(ReplyOriginalFotoRoja(cv::Mat)));
        connect( tareaCalibrar2, &NewsagesAlprTask::ReplyOriginalFotoRoja, [=](const cv::Mat &_PlanckRojos){
            this->ReplyOriginalFotoRoja(_PlanckRojos);
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
        tarea1 = new NewsagesAlprTask(m_nDevice, ALPR_PLANCK_BLANCO, Foto, m_settings);
        tarea2 = new NewsagesAlprTask(m_nDevice, ALPR_PLANCK_ROJO,   Foto, m_settings);

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
//        connect( tarea1, SIGNAL(ReplyMatriculaFoto(t_MatriculaResults)),
//                 this  , SIGNAL(ReplyMatriculaFoto(t_MatriculaResults)));

        std::unique_ptr<QMetaObject::Connection> pconn3{new QMetaObject::Connection};
        QMetaObject::Connection &conn3 = *pconn3;
        conn3=connect( tarea1, &NewsagesAlprTask::ReplyMatriculaFoto, [=](const t_MatriculaResults &_tempResults){
            QObject::disconnect(conn3);
            m_matricularesults->MatriculaA              = _tempResults.MatriculaA;
            m_matricularesults->MatriculaDetectedA      = _tempResults.MatriculaDetectedA;
            m_matricularesults->MatriculaFotoA          = _tempResults.MatriculaFotoA;
            m_matricularesults->MatriculaFotoAByte      = _tempResults.MatriculaFotoAByte;
            m_matricularesults->MatriculaPrecisionA     = _tempResults.MatriculaPrecisionA;
            m_matricularesults->MatriculaPrecisionAs    = _tempResults.MatriculaPrecisionAs;
            m_matricularesults->OrigenFoto              = _tempResults.OrigenFoto;
            m_matricularesults->OrigenFotoBlanca        = _tempResults.OrigenFotoBlanca;
            m_matricularesults->OrigenFotoPrewarp       = _tempResults.OrigenFotoPrewarp;
            m_matricularesults->OrigenFotoResize        = _tempResults.OrigenFotoResize;
            m_matricularesults->OrigenFotoResizeByte    = _tempResults.OrigenFotoResizeByte;
            tarea1->workFinished();
        });


//        connect( tarea2, SIGNAL(ReplyMatriculaFoto(t_MatriculaResults)),
//                 this  , SIGNAL(ReplyMatriculaFoto(t_MatriculaResults)));

        std::unique_ptr<QMetaObject::Connection> pconn4{new QMetaObject::Connection};
        QMetaObject::Connection &conn4 = *pconn4;
        conn4=connect( tarea2, &NewsagesAlprTask::ReplyMatriculaFoto, [=](const t_MatriculaResults &_tempResults){
           QObject::disconnect(conn4);           
           m_matricularesults->MatriculaB              = _tempResults.MatriculaB;
           m_matricularesults->MatriculaDetectedB      = _tempResults.MatriculaDetectedB;
           m_matricularesults->MatriculaFotoB          = _tempResults.MatriculaFotoB;
           m_matricularesults->MatriculaFotoBByte      = _tempResults.MatriculaFotoBByte;
           m_matricularesults->MatriculaPrecisionB     = _tempResults.MatriculaPrecisionB;
           m_matricularesults->MatriculaPrecisionBs    = _tempResults.MatriculaPrecisionBs;
           m_matricularesults->OrigenFoto              = _tempResults.OrigenFoto;
           m_matricularesults->OrigenFotoRoja          = _tempResults.OrigenFotoRoja;
           m_matricularesults->OrigenFotoPrewarp       = _tempResults.OrigenFotoPrewarp;
           m_matricularesults->OrigenFotoResize        = _tempResults.OrigenFotoResize;
           m_matricularesults->OrigenFotoResizeByte    = _tempResults.OrigenFotoResizeByte;
           tarea2->workFinished();
        });

        /** Ejecutar Procesos **/
        hilo1->start();
        hilo2->start();
    }

}

void NewsagesAlpr::onProcesarFotoFinished(){

    emit ReplyMatriculaResults(*m_matricularesults);
    qDebug() << "Device:" << m_nDevice << endl
             << "\t Matricula1:"   << m_matricularesults->MatriculaA
             << "precision:"<<m_matricularesults->MatriculaPrecisionAs<<"%"<< "patron:"    << m_matricularesults->MatriculaDetectedA << endl
             << "\t Matricula2:"<<m_matricularesults->MatriculaB
             << "precision:" << m_matricularesults->MatriculaPrecisionBs << "%" << "patron:" << m_matricularesults->MatriculaDetectedB << endl;

//    m_matricularesults.MatriculaA.detach();
//    m_matricularesults.MatriculaDetectedA=false;
//    m_matricularesults.MatriculaFotoA.release();
//    m_matricularesults.MatriculaFotoAByte.detach();
//    m_matricularesults.MatriculaPrecisionA=0;
//    m_matricularesults.MatriculaPrecisionAs.detach();
//    m_matricularesults.OrigenFoto.release();
//    m_matricularesults.OrigenFotoBlanca.release();
//    m_matricularesults.OrigenFotoPrewarp.release();
//    m_matricularesults.OrigenFotoResize.release();
//    m_matricularesults.OrigenFotoResizeByte.detach();

//    m_matricularesults.MatriculaB.detach();
//    m_matricularesults.MatriculaDetectedB=false;
//    m_matricularesults.MatriculaFotoB.release();
//    m_matricularesults.MatriculaFotoBByte.detach();
//    m_matricularesults.MatriculaPrecisionB=0;
//    m_matricularesults.MatriculaPrecisionBs.detach();
//    m_matricularesults.OrigenFoto.release();
//    m_matricularesults.OrigenFotoRoja.release();
//    m_matricularesults.OrigenFotoPrewarp.release();
//    m_matricularesults.OrigenFotoResize.release();
//    m_matricularesults.OrigenFotoResizeByte.detach();



}

/** END PROCESAR ********************************************************************************************************/


    } /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/
