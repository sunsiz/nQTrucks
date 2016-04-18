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

#include "opencv2/opencv.hpp"
//#include <stdio.h>

#include <QByteArray>
#include <QBuffer>

namespace nQTrucks {
    namespace Devices {
    using namespace alpr;
NewsagesAlpr::NewsagesAlpr(QObject *parent)
    : QObject(parent)
    //, m_fotocamara(new QImage(_fotocamara))
{
    setlocale(LC_NUMERIC, "C");
}

void NewsagesAlpr::ProcessFoto()
{

    /** Crear hilos por matriculas **/
    hilo1 = new QThread();
    hilo2 = new QThread();

    /** Crear tareas **/
    tarea1 = new NewsagesAlprTask;
    tarea2 = new NewsagesAlprTask;

    /** Asignar tareas a hilos **/
    tarea1->moveToThread(hilo1);
    tarea2->moveToThread(hilo2);

    /** conectar hilos con proceso **/
    connect( hilo1, SIGNAL(started()), tarea1, SLOT(procesarBlancas()) );
    connect( tarea1,SIGNAL(workFinished()), hilo1, SLOT(quit()) );
    connect( hilo1, SIGNAL(finished()), tarea1, SLOT(deleteLater()) );
    connect( hilo1, SIGNAL(finished()), hilo1, SLOT(deleteLater()) );

    connect( hilo2, SIGNAL(started()), tarea2, SLOT(procesarRojas()) );
    connect( tarea2,SIGNAL(workFinished()), hilo2, SLOT(quit()) );
    connect( hilo2, SIGNAL(finished()), tarea2, SLOT(deleteLater()) );
    connect( hilo2, SIGNAL(finished()), hilo2, SLOT(deleteLater()) );

    /** conectar respuestas con padre **/
    connect( tarea1, SIGNAL(ReplyMatriculaFoto(QImage)),this,SIGNAL(ReplyMatriculaFoto(QImage)));
    connect( tarea2, SIGNAL(ReplyMatriculaRemolqueFoto(QImage)),this,SIGNAL(ReplyMatriculaRemolqueFoto(QImage)));


    //Configurar hilos
    //DEBUG
    QString filename ="matriculas/r1.jpg";
    m_fotocamara = new QImage(filename);

    /** buscar blancos **/


    tarea1->setFotoCamara(*m_fotocamara);
    tarea2->setFotoCamara(*m_fotocamara);

    /** Iniciar Hilos **/
    hilo1->start(QThread::TimeCriticalPriority);
    hilo2->start(QThread::TimeCriticalPriority);

    emit ReplyOriginalFoto(*m_fotocamara);

}


NewsagesAlprTask::NewsagesAlprTask(QObject *parent)
    : QObject(parent)
{
    setlocale(LC_NUMERIC, "C");
}

NewsagesAlprTask::~NewsagesAlprTask()
{
    this->deleteLater();
}

cv::Mat NewsagesAlprTask::QImage2cvMat(QImage image)
{
    /** CONVERSOR QImage to std::vector<char>) **/
    QByteArray baScene; // byte array with data
    QBuffer buffer(&baScene);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer,"JPG");

    const char* begin = reinterpret_cast<char*>(baScene.data());
    const char* end = begin + baScene.size();
    std::vector<char> pic(begin, end);
    cv::Mat img = cv::imdecode(pic,CV_LOAD_IMAGE_COLOR);
    return img;

}

void NewsagesAlprTask::procesarBlancas()
{
    //Alpr matricula("eu", "config/openalpr.conf");
    matricula = new Alpr("truck",  "config/openalpr.conf");
    matricula->setDefaultRegion("truck");
    matricula->setTopN(1);

    //matricula->setPrewarp("");

    if (matricula->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        //Implementado:
        cv::Mat img = QImage2cvMat(*m_fotocamara);
        QImage image_matricula = QImage(*m_fotocamara);

        //CONVERSION de BLANCOS
         // CALIBRACION —---»» PARAMETRIZAR :::::
            cv::add(img,cv::Scalar(40,20,0),img);
        cv::Mat channel[3];
        cv::split(img, channel);
        img = channel[2] - channel[1] -   channel[2] + channel[0];

        // RECONOCER
        std::vector<AlprRegionOfInterest> regionsOfInterest;
        AlprResults results = matricula->recognize(img.data, img.elemSize(), img.cols, img.rows,regionsOfInterest);
        for (int i = 0; i < results.plates.size(); i++)
        {
            AlprPlateResult plate = results.plates[i];
            for (int k = 0; k < plate.topNPlates.size(); k++)
            {
                AlprPlate candidate = plate.topNPlates[k];
                float confianza = candidate.overall_confidence;
                qDebug() << "  blanca  - " << QString::fromStdString(candidate.characters) << "\t precision: " << confianza << "% " <<
                            candidate.matches_template << endl;
                image_matricula = image_matricula.copy(QRect(QPoint(plate.plate_points[0].x,plate.plate_points[0].y),
                                                     QPoint(plate.plate_points[2].x,plate.plate_points[2].y)));

                emit ReplyMatriculaFoto(image_matricula);
            }
        }
    }
}

void NewsagesAlprTask::procesarRojas()
{
    //Alpr remolque("eur", "config/openalpr.conf");
    remolque  = new Alpr("eur", "config/openalpr.conf");
    remolque->setTopN(1);
    remolque->setDefaultRegion("eur");
    //openalpr.setPrewarp("");

    if (remolque->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        //Implementado:
        cv::Mat img = QImage2cvMat(*m_fotocamara);
        QImage image_matricula = QImage(*m_fotocamara);

        //CONVERSION de ROJA
        // CALIBRACION —---»» PARAMETRIZAR :::::
            cv::add(img,cv::Scalar(0,20,40),img);
        cv::Mat channel[3];
        cv::split(img, channel);
        cv::add(channel[0], channel[1], img);
        cv::subtract(channel[2], channel[1], img);

        //cv::imshow("ROJOS",img);

        // RECONOCER
        std::vector<AlprRegionOfInterest> regionsOfInterest;
        AlprResults results = remolque->recognize(img.data, img.elemSize(), img.cols, img.rows,regionsOfInterest);
        for (int i = 0; i < results.plates.size(); i++)
        {
            AlprPlateResult plate = results.plates[i];
            for (int k = 0; k < plate.topNPlates.size(); k++)
            {
                AlprPlate candidate = plate.topNPlates[k];
                float confianza = candidate.overall_confidence;
                qDebug() << "  remolque  - " << QString::fromStdString(candidate.characters) << "\t precision: " << confianza << "% " <<
                            candidate.matches_template << endl;
                image_matricula = image_matricula.copy(QRect(QPoint(plate.plate_points[0].x,plate.plate_points[0].y),
                                                     QPoint(plate.plate_points[2].x,plate.plate_points[2].y)));
                emit ReplyMatriculaRemolqueFoto(image_matricula);
            }
        }
    }
}


} /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/




