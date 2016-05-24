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

#include <QObject>
#include <QSettings>
//#include <QImage>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>

#if defined(NQTRUCKS_LIBRARY)
#  define NQTRUCKSLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NQTRUCKSLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

/** BASCULAS **/
struct Bascula{
    bool bEstado=false;
    bool bEstadoAnterior=false;
    float iBruto=0;
    float iTara=0;
    float iNeto=0;
}; typedef Bascula t_Bascula;
/** END BASCULAS **/

/** ALPR  **/
struct PlanK{
    int A;
    int B;
    int C;
}; typedef PlanK t_Plank;

struct s_RegistroSimple{
    Bascula bascula={};
    QByteArray camara1={};
    QByteArray camara2={};
}; typedef s_RegistroSimple Registro_Simple;


struct s_RegistroSimpleMatriculas{
    Registro_Simple registrosimple={};

    QByteArray  fotomatriculaA1={};
    QString     matriculaA1={};
    long        precisionA1={};

    QByteArray  fotomatriculaA2={};
    QString     matriculaA2={};
    long        precisionA2={};

    QByteArray  fotomatriculaB1={};
    QString     matriculaB1={};
    long        precisionB1={};

    QByteArray  fotomatriculaB2={};
    QString     matriculaB2={};
    long        precisionB2={};

}; typedef s_RegistroSimpleMatriculas Registro_Simple_Matriculas;

struct MatriculaResults{
    int     tipo=0;  //0 para calibracion, 1 para procesado
    int     id=0;    //id fuente de captura de foto
    cv::Mat  OrigenFoto; //Imagen Original
    cv::Mat  OrigenFotoPrewarp; // Imagen con calibracion prewarp
    cv::Mat  OrigenFotoBlanca;  //  Imagen con calibracion de Blancos
    cv::Mat  OrigenFotoRoja;    // Imagen con calibracion de Rojos

    bool    MatriculaDetectedA=false;  // Coincide con un patron de busqueda?
    QString MatriculaA="";             // STring de la matricula
    cv::Mat MatriculaFotoA;            // Imagen recortada de la Matricula
    long    MatriculaPrecisionA=0;     // Precision del OCR

    bool    MatriculaDetectedB=false;  // Coincide con un patron de busqueda?
    QString MatriculaB="";             // STring de la matricula
    cv::Mat MatriculaFotoB;            // Imagen recortada de la Matricula
    long    MatriculaPrecisionB=0;     // Precision del OCR

}; typedef MatriculaResults t_MatriculaResults;
/** END ALRP **/


/** SETTINGS **/
namespace nQTrucks
{

    #define CAMARA1 "Camara1"
    #define CAMARA2 "Camara2"

    #define NEWSAGESIO "NEWSAGESIO"
    #define BASCULA "BASCULA"

    #define ALPR1  "Alpr1"
    #define ALPR2  "Alpr2"

struct Prewarp{
    QString type="planar";  
    float w=1208;
    float h=1024;
    float panX = 0;
    float panY = 0;
    float rotationx = 0;
    float rotationy = 0;
    float rotationz = 0;
    float stretchX = 1.0;
    float dist = 1.0;

}; typedef Prewarp t_Prewarp;


    QString extractClassName(QString className);
    class nQTrucksError : public std::runtime_error{
    public:
        nQTrucksError(const QString& message):std::runtime_error(message.toStdString()){}
    };


    namespace Const{
    }

} // namespace nQTrucks

/** END SETTINGS **/

#endif // NQTRUCKS_H
