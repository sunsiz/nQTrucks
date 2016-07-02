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
#ifndef NQTRUCKS_H
#define NQTRUCKS_H
#include <QtCore/qglobal.h>
#include <stdexcept>
#include <QString>

#include <QSettings>


#if defined(NQTRUCKS_LIBRARY)
#  define NQTRUCKSLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NQTRUCKSLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

/** SETTINGS **/
namespace nQTrucks
{
//    static const int matriculaNewWidth = 520;
//    static const int matriculaNewHeight = 110;
//    static const cv::Size matriculaSize(matriculaNewWidth,matriculaNewHeight);

//    static const int fotoWidth = 1280;
//    static const int fotoHeight = 720;
//    static const cv::Size fotoSize(fotoWidth,fotoHeight);

    static const QString default_prewarp ="";//  "planar,1280,720,0,0,0,1.0,1.0,0,0";

    #define ALPR_PLANCK_BLANCO 0
    #define ALPR_PLANCK_ROJO   1

    #define SEMAFORO_VERDE 0
    #define SEMAFORO_AMARILLO 1
    #define SEMAFORO_ROJO 2

    #define RELE_ON  false
    #define RELE_OFF true

    #define CAMARA1     "Camara1"
    #define CAMARA2     "Camara2"

    #define NEWSAGESIO "NEWSAGESIO"
    #define BASCULA    "BASCULA"

    #define ALPR       "Alpr"
    #define ALPR1      "Alpr1"
    #define ALPR2      "Alpr2"

    QString extractClassName(QString className);
    class nQTrucksError : public std::runtime_error{
    public:
        nQTrucksError(const QString& message):std::runtime_error(message.toStdString()){}
    };


    struct Prewarp{
        QString type="planar";
        float w=1280;
        float h=720;
        float panX = 0;
        float panY = 0;
        float rotationx = 0;
        float rotationy = 0;
        float rotationz = 0;
        float stretchX = 1.0;
        float dist = 1.0;
    }; typedef Prewarp t_Prewarp;

    /** ALPR  **/
    class Planck{
    public:
        int A=0;
        int B=0;
        int C=0;
    };
    /** END ALRP **/

    namespace Const{
    } //end namespace Const
} // namespace nQTrucks
#endif // NQTRUCKS_H
