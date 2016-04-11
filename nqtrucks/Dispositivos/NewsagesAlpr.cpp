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
#include <QImage>
#include <QPixmap>
#include <QByteArray>
#include <QBuffer>

namespace nQTrucks {
    namespace Devices {
    using namespace alpr;
NewsagesAlpr::NewsagesAlpr(QObject *parent) : QObject(parent)
{
    setlocale(LC_NUMERIC, "C");
    // Initialize the library using United States style license plates.
    // You can use other countries/regions as well (for example: "eu", "au", or "kr")
    Alpr openalpr("eu", "config/openalpr.conf");

    // Optionally specify the top N possible plates to return (with confidences).  Default is 10
    openalpr.setTopN(1);

    // Optionally, provide the library with a region for pattern matching.  This improves accuracy by
    // comparing the plate text with the regional pattern.
    openalpr.setDefaultRegion("es");

    openalpr.setPrewarp("");
    // Make sure the library loaded before continuing.
    // For example, it could fail if the config/runtime_data is not found
    if (openalpr.isLoaded() == false)
    {
        qDebug() << "Error loading OpenALPR" << endl;
    }else{


        /** CONVERSOR QImage to std::vector<char>) **/
        QString filename ="matriculas/image2.jpg";
        QImage Image = QImage(filename);
        QByteArray baScene; // byte array with data
        QBuffer buffer(&baScene);
        buffer.open(QIODevice::WriteOnly);
        Image.save(&buffer,"JPG");

        const char* begin = reinterpret_cast<char*>(baScene.data());
        const char* end = begin + baScene.size();
        std::vector<char> pic(begin, end);


        // Recognize an image file.  You could alternatively provide the image bytes in-memory.
        AlprResults results = openalpr.recognize(pic);
        //AlprResults results = openalpr.recognize("matriculas/image2.jpg");

        // Iterate through the results.  There may be multiple plates in an image,
        // and each plate return sthe top N candidates.
        for (int i = 0; i < results.plates.size(); i++)
        {
            AlprPlateResult plate = results.plates[i];
          //std::cout << "plate" << i << ": " << plate.topNPlates.size() << " results" << std::endl;
            for (int k = 0; k < plate.topNPlates.size(); k++)
            {
                AlprPlate candidate = plate.topNPlates[k];
                float confianza = candidate.overall_confidence;
                qDebug() << "    - " << QString::fromStdString(candidate.characters) << "\t precision: " << confianza << "%" << endl;
          //std::cout << "\t pattern_match: " << candidate.matches_template << std::endl;
            }
        }
    }
}
    } /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/




