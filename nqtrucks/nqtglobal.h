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
#include <QBuffer>
#include <QDateTime>

#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>


#if defined(NQTRUCKS_LIBRARY)
#  define NQTRUCKSLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NQTRUCKSLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

/** SETTINGS **/
namespace nQTrucks
{
    static const int matriculaNewWidth = 520;
    static const int matriculaNewHeight = 110;
    static const cv::Size matriculaSize(matriculaNewWidth,matriculaNewHeight);

    static const int fotoWidth = 1280;
    static const int fotoHeight = 720;
    static const cv::Size fotoSize(fotoWidth,fotoHeight);

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

    /** CONVERSORES ********************************************************/
    #ifndef NQTRUCKS_TOOLS_H
    #define NQTRUCKS_TOOLS_H
    class Tools: public QObject{
        Q_OBJECT
    public:
        explicit Tools(QObject *parent=nullptr):QObject(parent){}

        inline QImage convertMat2QImage(const cv::Mat &_cvimage){
            QImage qtImg= QImage();
            if( !_cvimage.empty() && _cvimage.depth() == CV_8U ){
                if(_cvimage.channels() == 1){
                    qtImg = QImage( (const unsigned char *)(_cvimage.data),
                                    _cvimage.cols,
                                    _cvimage.rows,
                                    //_cvimage.step,
                                    QImage::Format_Indexed8 ).copy();
                }
                else{
                    cv::cvtColor( _cvimage, _cvimage, CV_BGR2RGB );
                    qtImg = QImage( (const unsigned char *)(_cvimage.data),
                                    _cvimage.cols,
                                    _cvimage.rows,
                                    //_cvimage.step,
                                    QImage::Format_RGB888 ).copy();
                }
            }
            return qtImg;
        }
        inline cv::Mat  convertQImage2Mat(   const QImage     &_qimage){
            QByteArray baScene; // byte array with data
            QBuffer buffer(&baScene);
            buffer.open(QIODevice::WriteOnly);
            _qimage.save(&buffer,"JPG");

            const char* begin = reinterpret_cast<char*>(baScene.data());
            const char* end = begin + baScene.size();
            std::vector<char> pic(begin, end);
            buffer.close();
            baScene.clear();
            return cv::imdecode(pic,CV_LOAD_IMAGE_COLOR).clone();
        }

        inline QByteArray convertMat2ByteArray(const cv::Mat    &_cvimage){
            QImage qtImg = convertMat2QImage(_cvimage.clone());
            QByteArray baScene; // byte array with data
            QBuffer buffer(&baScene);
            buffer.open(QIODevice::WriteOnly);
            qtImg.save(&buffer,"JPG");
            buffer.close();
            return baScene;
        }

        //static cv::Mat    convertByteArray2Mat(QByteArray _Bytearray);
        //static QByteArray resizeByteArray2ByteArray(QByteArray _ByteArray, const int &_w, const int &_h);
    };
    #endif  //TOOLS
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

    /** ALPR  **/
    class Planck{
    public:
        int A=0;
        int B=0;
        int C=0;
    };
    /** END ALRP **/


    namespace Registros{              /** REPORTS **/
        /** BASCULAS **************************************************************************************/
        #ifndef NQTRUCKS_BASCULA_H
        #define NQTRUCKS_BASCULA_H
        class Bascula : public QObject {
            Q_OBJECT
        public:
            explicit Bascula(QObject *parent=nullptr);
        private:
            bool  m_bEstado;
            bool  m_bEstadoAnterior;
            float m_iBruto;
            float m_iTara;
            float m_iNeto;
        public:
            void setBascula(const Bascula &value);
            void clearBascula();

            bool getBEstado() const{ return this->m_bEstado;}
            void setBEstado(bool value){    this->m_bEstado = value;}

            bool getBEstadoAnterior() const{ return this->m_bEstadoAnterior;}
            void setBEstadoAnterior(bool value){    this->m_bEstadoAnterior = value;}

            float getIBruto() const{return this->m_iBruto;}
            void  setIBruto(float value){  this->m_iBruto = value;}

            float getITara() const{return this->m_iTara;}
            void  setITara(float value){  this->m_iTara = value; }

            float getINeto() const{return this->m_iNeto;}
            void  setINeto(float value){  this->m_iNeto = value; }

        };
        #endif
        /** END BASCULAS *****************************************************************************************************/

        /** CAMARA **/
        #ifndef NQTRUCKS_CAMARA_H
        #define NQTRUCKS_CAMARA_H
        class Camara : public QObject{
            Q_OBJECT
        public:
            explicit Camara(QObject *parent=nullptr);
            void  setCamara(const Camara &value);
            cv::Mat *getOrigenFoto() const;
            void setOrigenFoto(cv::Mat *value);

            QByteArray getOrigenFotoByte() const;

            QImage getOrigenFotoQ() const;

        private:
            cv::Mat    *m_OrigenFoto;//              =cv::Mat::zeros(fotoSize, CV_8UC3 );                 //Imagen Original
            QByteArray  m_OrigenFotoByte;//          =nQTrucks::Tools::convertMat2ByteArray(OrigenFoto);
            QImage      m_OrigenFotoQ;
        public:
            inline void convertirFotos(){
                Tools *m_tools  = new Tools; /** MEMORY LEAK **/
                m_OrigenFotoByte.clear();
                m_OrigenFotoQ.detach();
                m_OrigenFotoByte  = m_tools->convertMat2ByteArray(m_OrigenFoto->clone()); /** MEMORY LEAK **/
                m_OrigenFotoQ     = m_tools->convertMat2QImage(   m_OrigenFoto->clone());
                delete m_tools;
            }
        };
        #endif
        /** END CAMARA **/

        class MatriculaResults{
        public:
            MatriculaResults();
            int        tipo;//                 =0;                                                     //0 para calibracion, 1 para procesado
            int        id ;//                  =0;                                                     //id fuente de captura de foto
            Camara    *camara;

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

            inline void convertirFotos(){
                Tools *m_tools        =  new Tools;
                OrigenFotoPrewarpQ    =  m_tools->convertMat2QImage(OrigenFotoPrewarp.clone());
                OrigenFotoBlancaQ     =  m_tools->convertMat2QImage(OrigenFotoBlanca.clone());
                OrigenFotoRojaQ       =  m_tools->convertMat2QImage(OrigenFotoRoja.clone());

                MatriculaFotoAByte    =  m_tools->convertMat2ByteArray(MatriculaFotoA.clone());
                MatriculaFotoAQ       =  m_tools->convertMat2QImage(MatriculaFotoA.clone());

                MatriculaFotoBByte    =  m_tools->convertMat2ByteArray(MatriculaFotoB.clone());
                MatriculaFotoBQ       =  m_tools->convertMat2QImage(MatriculaFotoB.clone());
                delete m_tools;
            }
        };

        #ifndef NQTRUCKS_REGISTROMATRICULAS_H
        #define NQTRUCKS_REGISTROMATRICULAS_H
        class RegistroMatriculas{
        public:
            //RegistroMatriculas(QObject *parent=nullptr):QObject(parent){}
            RegistroMatriculas():m_bascula(new Bascula()){;}
            long long   id=0 ;//                  =0;                                                     //id fuente de captura de foto
            QDateTime   FechaRegistro;
            Bascula     *m_bascula;
            QVector<Registros::MatriculaResults> results = QVector<Registros::MatriculaResults>(2);
        };
        #endif
    } // namespace Registros /** END REPORTS **/
    /** END SETTINGS **/


    namespace Const{
    } //end namespace Const
} // namespace nQTrucks
#endif // NQTRUCKS_H
