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
#include <stdio.h>

#include <QImage>
#include <QPixmap>
#include <QRgb>
#include <QByteArray>
#include <QBuffer>

namespace nQTrucks {
    namespace Devices {
    using namespace alpr;
NewsagesAlpr::NewsagesAlpr(QObject *parent) : QObject(parent)
{
    setlocale(LC_NUMERIC, "C");
}


void NewsagesAlpr::ProcessFoto(){

    // Initialize the library using United States style license plates.
    // You can use other countries/regions as well (for example: "eu", "au", or "kr")
    Alpr openalpr("eur", "config/openalpr.conf");
    //openalpr.setCountry();
    //openalpr.setPrewarp("planar,640.000000,480.000000,0.000400,0.000600,0.060000,0.675000,0.865000,-122.000000,-5.000000");

    // Optionally specify the top N possible plates to return (with confidences).  Default is 10
    openalpr.setTopN(1);

    // Optionally, provide the library with a region for pattern matching.  This improves accuracy by
    // comparing the plate text with the regional pattern.
    //openalpr.setDefaultRegion("eu");


    //openalpr.setPrewarp("");
    // Make sure the library loaded before continuing.
    // For example, it could fail if the config/runtime_data is not found
    if (openalpr.isLoaded() == false)
    {
        qDebug() << "Error loading OpenALPR" << endl;
    }else{


        QString filename ="matriculas/r1.jpg";
        QImage image = QImage(filename);
        QImage image_matricula = QImage(filename);

        /** CONVERSOR QImage to std::vector<char>) **/
        QByteArray baScene; // byte array with data
        QBuffer buffer(&baScene);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer,"JPG");

        const char* begin = reinterpret_cast<char*>(baScene.data());
        const char* end = begin + baScene.size();
        std::vector<char> pic(begin, end);
        /** END CONVERSOR QImage to std::vector<char>) **/


        /** CONVERSION QImage to cv::MAT
            TODO:  Implementar parametros y configuracion **/
            cv::Mat img = cv::imdecode(pic,CV_LOAD_IMAGE_COLOR);

            // CALIBRACION —---»» PARAMETRIZAR :::::
            cv::add(img,cv::Scalar(0,40,60),img);

            //CONVERSION de ROJA
            cv::Mat channel[3];
            cv::split(img, channel);
            cv::add(channel[0], channel[1], img);
            cv::subtract(channel[2], channel[1], img);

        // Recognize an image file.  You could alternatively provide the image bytes in-memory.
            //else regionsOfInterest.push_back(AlprRegionOfInterest(0, 0, frame.cols, frame.rows));

            std::vector<AlprRegionOfInterest> regionsOfInterest;
            //regionsOfInterest.push_back(AlprRegionOfInterest(0, 0, img.cols, img.rows));
            AlprResults results = openalpr.recognize(img.data, img.elemSize(), img.cols, img.rows,regionsOfInterest);

        //AlprResults results = openalpr.recognize("matriculas/image2.jpg");

        // Iterate through the results.  There may be multiple plates in an image,
        // and each plate return sthe top N candidates.
        for (int i = 0; i < results.plates.size(); i++)
        {
            AlprPlateResult plate = results.plates[i];
                //qDebug() << " country " << QString::fromStdString(plate.country)
                  //       << " region " << QString::fromStdString(plate.region) << " " << endl;
                //Image_matricula = Image_matricula.copy(QRect(QPoint(plate.plate_points[0].x,plate.plate_points[0].y),
                //                                     QPoint(plate.plate_points[2].x,plate.plate_points[2].y)));

                //emit ReplyMatriculaFoto(Image_matricula);


          //std::cout << "plate" << i << ": " << plate.topNPlates.size() << " results" << std::endl;
            for (int k = 0; k < plate.topNPlates.size(); k++)
            {

                AlprPlate candidate = plate.topNPlates[k];                
                float confianza = candidate.overall_confidence;

                qDebug() << "    - " << QString::fromStdString(candidate.characters) << "\t precision: " << confianza << "% " <<
                          candidate.matches_template << endl;
                qDebug() << "coordenadas de la imagen:"
                         << plate.plate_points[0].x << ","
                         << plate.plate_points[0].y << ","
                         << plate.plate_points[1].x << ","
                         << plate.plate_points[1].y << ","
                         << plate.plate_points[2].x << ","
                         << plate.plate_points[2].y << ","
                         << plate.plate_points[3].x << ","
                         << plate.plate_points[4].y << ","
                         << endl;

                //Image_matricula = Image_matricula.copy(QRect(QPoint(plate.plate_points[0].x,plate.plate_points[0].y),
                  //                                   QPoint(plate.plate_points[2].x,plate.plate_points[2].y)));

                //emit ReplyMatriculaFoto(Image_matricula);
          //std::cout << "\t pattern_match: " << candidate.matches_template << std::endl;
            }
        }

        emit ReplyOriginalFoto(image);
        emit ReplyMatriculaFoto(image_matricula);
    }



}

    } /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/




