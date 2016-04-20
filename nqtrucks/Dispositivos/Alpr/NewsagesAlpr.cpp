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

namespace nQTrucks {
    namespace Devices {
    using namespace alpr;

NewsagesAlpr::NewsagesAlpr(int nDevice, QSettings *_appsettings, QObject *parent)
    : QObject(parent)
    , m_nDevice(nDevice)
    , m_settings(_appsettings)
    , m_matricularesults(new t_MatriculaResults)
{

}





void NewsagesAlpr::calibrarFoto(){

    QString filename ="matriculas/r1.jpg";
    QImage temp = QImage(filename);
    this->setFotoCamara(temp);

    m_matricularesults->id=m_nDevice;
    m_matricularesults->OrigenFoto=FotoCamara();
    m_matricularesults->OrigenFotoPrewarp=FotoCamara();
    emit ReplyOriginalFoto(m_matricularesults->OrigenFoto);



    /** Crear hilos por Tipos de matriculas **/
    hilo1 = new QThread;
    hilo2 = new QThread;

    /** Crear tareas **/
    tarea1 = new NewsagesAlprTask(m_nDevice,m_settings);
    tarea1->setFotoCamara(m_matricularesults->OrigenFotoPrewarp);

    tarea2 = new NewsagesAlprTask(m_nDevice,m_settings);
    tarea2->setFotoCamara(m_matricularesults->OrigenFotoPrewarp);

    /** Asignar tareas a hilos **/
    tarea1->moveToThread(hilo1);
    tarea2->moveToThread(hilo2);

    /** conectar hilos con proceso **/
    connect( hilo1, SIGNAL(started()), tarea1, SLOT(procesarBlancas()) );
    connect( tarea1,SIGNAL(workFinished()), hilo1, SLOT(quit()) );
    connect( tarea1, SIGNAL(workFinished()), tarea1, SLOT(deleteLater()) );
    connect( hilo1, SIGNAL(finished()), hilo1, SLOT(deleteLater()) );

    connect( hilo2, SIGNAL(started()), tarea2, SLOT(procesarRojas()) );
    connect( tarea2,SIGNAL(workFinished()), hilo2, SLOT(quit()) );
    connect( tarea2, SIGNAL(workFinished()), tarea2, SLOT(deleteLater()) );
    connect( hilo2, SIGNAL(finished()), hilo2, SLOT(deleteLater()) );

    /** conectar respuestas con padre **/
    connect( tarea1, SIGNAL(ReplyMatriculaFoto(QImage)),this,SIGNAL(ReplyMatriculaFoto(QImage)));
    connect( tarea2, SIGNAL(ReplyMatriculaFotoRemolque(QImage)),this,SIGNAL(ReplyMatriculaFotoRemolque(QImage)));

    hilo1->start();
    hilo2->start();
}


void NewsagesAlpr::processFoto()
{



}

//cv::Mat NewsagesAlpr::QImage2cvMat(QImage image)
//{
//    /** CONVERSOR QImage to std::vector<char>) **/
//    QByteArray baScene; // byte array with data
//    QBuffer buffer(&baScene);
//    buffer.open(QIODevice::WriteOnly);
//    image.save(&buffer,"PNG");

//    const char* begin = reinterpret_cast<char*>(baScene.data());
//    const char* end = begin + baScene.size();
//    std::vector<char> pic(begin, end);
//    cv::Mat img = cv::imdecode(pic,CV_LOAD_IMAGE_COLOR);
//    return img;

//}

//QImage NewsagesAlpr::cvMat2QImage(const cv::Mat &image)
//{
//    QImage qtImg;
//    if( !image.empty() && image.depth() == CV_8U ){
//        if(image.channels() == 1){
//            qtImg = QImage( (const unsigned char *)(image.data),
//                            image.cols,
//                            image.rows,
//                            QImage::Format_Indexed8 );
//        }
//        else{
//            cvtColor( image, image, CV_BGR2RGB );
//            qtImg = QImage( (const unsigned char *)(image.data),
//                            image.cols,
//                            image.rows,
//                            QImage::Format_RGB888 );
//        }
//    }
//    //emit ReplyMatriculaFoto(qtImg);
//    return qtImg;
//    /** CONVERSOR QImage to std::vector<char>) **/
//}





    } /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/

























