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

#ifndef NEWSAGESALPR_H
#define NEWSAGESALPR_H

#include <QObject>
#include <nqtglobal.h>

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "alpr.h"
#include "state_detector.h"


namespace nQTrucks {
namespace Devices {
    using namespace alpr;
class NewsagesAlpr : public QObject
{
    Q_OBJECT
    cv::Mat QImage2cvMat(QImage image);
public:
    explicit NewsagesAlpr(QObject *parent = 0);

signals:
    void ReplyOriginalFoto(const QImage &Foto);
    void ReplyMatriculaFoto(const QImage &Foto);
public slots:
    void ProcessFoto();
};

} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/
#endif // NEWSAGESALPR_H
