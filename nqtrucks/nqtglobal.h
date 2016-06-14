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
#pragma once
#ifndef NQTRUCKS_H
#define NQTRUCKS_H
#include <QtCore/qglobal.h>
#include <stdexcept>
#include <QString>

#include <QObject>
#include <QSettings>
#include <QVector>
#include <QImage>
#include <QDateTime>

#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>


#if defined(NQTRUCKS_LIBRARY)
#  define NQTRUCKSLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NQTRUCKSLIBSHARED_EXPORT Q_DECL_IMPORT
#endif



const int matriculaNewWidth = 520;
const int matriculaNewHeight = 110;
const cv::Size matriculaSize(matriculaNewWidth,matriculaNewHeight);

const int fotoWidth = 1280;
const int fotoHeight = 720;
const cv::Size fotoSize(fotoWidth,fotoHeight);

const QString default_prewarp ="";//  "planar,1280,720,0,0,0,1.0,1.0,0,0";



/** SETTINGS **/
namespace nQTrucks
{
    #define ALPR_PLANCK_BLANCO 0
    #define ALPR_PLANCK_ROJO   1

    #define SEMAFORO_VERDE 0
    #define SEMAFORO_AMARILLO 1
    #define SEMAFORO_ROJO 2

    #define CAMARA1 "Camara1"
    #define CAMARA2 "Camara2"

    #define NEWSAGESIO "NEWSAGESIO"
    #define BASCULA    "BASCULA"

    #define ALPR   "Alpr"
    #define ALPR1  "Alpr1"
    #define ALPR2  "Alpr2"

    /** CONVERSORES ********************************************************/
    class Tools: public QObject{
        Q_OBJECT
    public:
        explicit Tools(QObject *parent=nullptr);
        /*static*/ QImage     convertMat2QImage(   const cv::Mat    &_cvimage);
        /*static*/ cv::Mat    convertQImage2Mat(   const QImage     &_qimage);
        /*static*/ QByteArray convertMat2ByteArray(const cv::Mat    &_cvimage);
        //static cv::Mat    convertByteArray2Mat(QByteArray _Bytearray);
        //static QByteArray resizeByteArray2ByteArray(QByteArray _ByteArray, const int &_w, const int &_h);
    };
    /** END CONVERSORES **********************************************************/




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


    QString extractClassName(QString className);
    class nQTrucksError : public std::runtime_error{
    public:
        nQTrucksError(const QString& message):std::runtime_error(message.toStdString()){}
    };


    namespace Const{
    }




/** BASCULAS **/
    class Bascula{
    public:
        bool bEstado=false;
        bool bEstadoAnterior=false;
        float iBruto=0;
        float iTara=0;
        float iNeto=0;
    };
/** END BASCULAS **/

/** ALPR  **/
    class Planck{
    public:
        int A=0;
        int B=0;
        int C=0;
    };
/** END ALRP **/


namespace Registros{              /** REPORTS **/
    class Camara{        
    public:
        Camara();
        ~Camara();
        cv::Mat    OrigenFoto;//              =cv::Mat::zeros(fotoSize, CV_8UC3 );                 //Imagen Original
        QByteArray OrigenFotoByte;//          =nQTrucks::Tools::convertMat2ByteArray(OrigenFoto);
        QImage     OrigenFotoQ;
        void convertirFotos();
        Tools *m_tools;
    };

    class Simple{
        public:
        Bascula bascula;
    };

    class MatriculaResults{
    public:
        MatriculaResults();
        ~MatriculaResults();
        int        tipo;//                 =0;                                                     //0 para calibracion, 1 para procesado
        int        id ;//                  =0;                                                     //id fuente de captura de foto
        Camara    camara;


        cv::Mat    OrigenFotoPrewarp;//    =cv::Mat::zeros(fotoSize, CV_8UC3 );                    // Imagen con calibracion prewarp
        QImage     OrigenFotoPrewarpQ;

        cv::Mat    OrigenFotoBlanca;//     =cv::Mat::zeros(fotoSize, CV_8UC3 );                    //  Imagen con calibracion de Blancos
        QImage     OrigenFotoBlancaQ;
        cv::Mat    OrigenFotoRoja;//       =cv::Mat::zeros(fotoSize, CV_8UC3 );                    // Imagen con calibracion de Rojos
        QImage     OrigenFotoRojaQ;

        bool       MatriculaDetectedA;//   =false;                                                 // Coincide con un patron de busqueda?
        QString    MatriculaA;//           ="";                                                    // STring de la matricula
        cv::Mat    MatriculaFotoA;//       =cv::Mat::zeros( matriculaSize, CV_8UC3 );              // Imagen recortada de la Matricula
        QByteArray MatriculaFotoAByte;//   =nQTrucks::Tools::convertMat2ByteArray(MatriculaFotoA);
        QImage     MatriculaFotoAQ;
        float      MatriculaPrecisionA;//  =0;                                                     // Precision del OCR
        QString    MatriculaPrecisionAs;// ="0%";

        bool       MatriculaDetectedB;//   =false;                                                 // Coincide con un patron de busqueda?
        QString    MatriculaB;//           ="";                                                    // STring de la matricula
        cv::Mat    MatriculaFotoB;//       =cv::Mat::zeros( matriculaSize, CV_8UC3 );              // Imagen recortada de la Matricula
        QByteArray MatriculaFotoBByte;//   =nQTrucks::Tools::convertMat2ByteArray(MatriculaFotoB);
        QImage     MatriculaFotoBQ;
        float      MatriculaPrecisionB;//  =0;                                                     // Precision del OCR        
        QString    MatriculaPrecisionBs;// ="0%";

        Tools *m_tools;
        void convertirFotos();
    };

        #define REGISTRO_ID                          0
        #define REGISTRO_FECHA                       1
        #define REGISTRO_PESO_BRUTO                  2
        #define REGISTRO_PESO_NETO                   3
        #define REGISTRO_PESO_TARA                   4
        #define REGISTRO_CAMARA1                     5
        #define REGISTRO_CAMARA2                     10

        struct sMatriculas{
            QString key[255]={0};
            QString v[255]={0};
            QString l[255]={0};
        };typedef sMatriculas Matriculas;

} // namespace Registros /** END REPORTS **/
/** END SETTINGS **/

class SimpleMatriculas{
public:
    long long   id=0 ;//                  =0;                                                     //id fuente de captura de foto
    QDateTime FechaRegistro;
    Bascula bascula;
    QVector<Registros::MatriculaResults> results = QVector<Registros::MatriculaResults>(2);
    void convertirFotos();
    };


} // namespace nQTrucks

#endif // NQTRUCKS_H
