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
            return cv::Mat(cv::imdecode(pic,CV_LOAD_IMAGE_COLOR));//.clone();
            //return _cvtemp;
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


    namespace Registros{
        /** BASCULAS **************************************************************************************/
        #ifndef NQTRUCKS_BASCULA_H
        #define NQTRUCKS_BASCULA_H
        class Bascula {
        public:
            Bascula();
            Bascula getBascula() const { return *this; }
        private:
            bool  m_bEstado;
            bool  m_bEstadoAnterior;
            float m_iBruto;
            float m_iTara;
            float m_iNeto;
        public:
            void setBascula(const Bascula &value);
            void clear();

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
        class Camara{
        public:
            Camara();
//            // Return by pointer needs const and non-const versions
//                  Camara*  getCamara()       { return this; }
//            const Camara*  getCamara() const { return this; }

//            // Return by reference needs const and non-const versions
//                  Camara& getCamara()       { return *this; }
//            const Camara& getCamara() const { return *this; }

            // Return by value only needs one version.
            Camara getCamara() const { return *this; }
            ~Camara();
        private:
            cv::Mat           m_OrigenFoto;//              =cv::Mat::zeros(fotoSize, CV_8UC3 );                 //Imagen Original
            QByteArray        m_OrigenFotoByte;//          =nQTrucks::Tools::convertMat2ByteArray(OrigenFoto);
            QImage            m_OrigenFotoQ;
        public:
            void clear();
            cv::Mat  getOrigenFoto() const;
            void     setOrigenFoto(const cv::Mat value);

            void       setOrigenFotoByte(  const QByteArray &OrigenFotoByte);
            QByteArray getOrigenFotoByte() const;

            QImage getOrigenFotoQ() const;
            void   setOrigenFotoQ(  const QImage &OrigenFotoQ);

            void setCamara(const Camara &value);

            inline void convertirFotos(){
                Tools *m_tools  = new Tools; /** MEMORY LEAK **/
                setOrigenFotoByte(m_tools->convertMat2ByteArray(getOrigenFoto().clone())); /** MEMORY LEAK **/
                setOrigenFotoQ(   m_tools->convertMat2QImage(getOrigenFoto().clone()));
                delete m_tools;
            }
        };
        #endif
        /** END CAMARA **/


        #ifndef NQTRUCKS_MARICULARESULTS_H
        #define NQTRUCKS_MARICULARESULTS_H
        class MatriculaResults{
        public:
            explicit MatriculaResults();
            MatriculaResults getMatriculaResults() const { return *this; }
            ~MatriculaResults();
            Camara     *camara;
            inline void setPlanckBlanco(const Planck &value){
                cv::Mat channel[3];
                cv::Mat *_dest = new cv::Mat(this->camara->getOrigenFoto().clone());
                cv::add(*_dest,cv::Scalar(value.C,value.B,value.A), *_dest);
                cv::split(*_dest, channel);
                this->setOrigenFotoBlanca(channel[2] - channel[1] -   channel[2] + channel[0]); /** MEMORY LEAK **/
                channel[0].release();
                channel[1].release();
                channel[2].release();
                _dest->release();
                delete _dest;
            }
            inline void setPlanckRojo(const Planck &value){
                cv::Mat channel[3];
                cv::Mat *_dest = new cv::Mat(this->camara->getOrigenFoto().clone());
                //m_matricularesult->setOrigenFotoRoja(m_matricularesult->camara->getOrigenFoto().clone());
                cv::add(*_dest,cv::Scalar(value.A,value.B,value.C),*_dest);
                cv::split(*_dest,channel);
                cv::add(channel[0], channel[1], *_dest); /** MEMORY LEAK **/
                cv::subtract(channel[2], channel[1], *_dest);
                this->setOrigenFotoRoja(*_dest);
                channel[0].release();
                channel[1].release();
                channel[2].release();
                _dest->release();
                delete _dest;
                //emit ReplyOriginalFotoRoja(m_matricularesult->getOrigenFotoRoja());
            }

        private:
            int        tipo;//                 =0;                                                     //0 para calibracion, 1 para procesado
            int        id ;//                  =0;                                                     //id fuente de captura de foto

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
        public:
            void     setMatriculaResults(const MatriculaResults &value);
            void     clear();


            int getTipo() const{return tipo; }
            void setTipo(int value){tipo = value;}

            int getId() const{return id;}
            void setId(int value){id = value;}

            cv::Mat getOrigenFotoPrewarp() const{return OrigenFotoPrewarp; }
            void setOrigenFotoPrewarp(const cv::Mat &value);

            QImage getOrigenFotoPrewarpQ() const{return OrigenFotoPrewarpQ;}

            cv::Mat getOrigenFotoBlanca() const{return OrigenFotoBlanca;}
            void setOrigenFotoBlanca(const cv::Mat &value);

            QImage getOrigenFotoBlancaQ() const{return OrigenFotoBlancaQ;}

            cv::Mat getOrigenFotoRoja() const{return OrigenFotoRoja;}
            void setOrigenFotoRoja(const cv::Mat &value);

            QImage getOrigenFotoRojaQ() const{ return OrigenFotoRojaQ;}

            bool getMatriculaDetectedA() const{ return MatriculaDetectedA; }
            void setMatriculaDetectedA(bool value){ MatriculaDetectedA = value; }

            QString getMatriculaA() const{return MatriculaA;}
            void setMatriculaA(const QString &value){ MatriculaA = value; }

            cv::Mat getMatriculaFotoA() const{return MatriculaFotoA; }
            void setMatriculaFotoA(const cv::Mat &value);

            QByteArray getMatriculaFotoAByte() const{return MatriculaFotoAByte;}

            QImage getMatriculaFotoAQ() const{return MatriculaFotoAQ;}

            float getMatriculaPrecisionA() const{return MatriculaPrecisionA;}
            void setMatriculaPrecisionA(float value){ MatriculaPrecisionA = value; }

            QString getMatriculaPrecisionAs() const{ return MatriculaPrecisionAs; }
            void setMatriculaPrecisionAs(const QString &value){ MatriculaPrecisionAs = value; }

            bool getMatriculaDetectedB() const{ return MatriculaDetectedB;}
            void setMatriculaDetectedB(bool value){ MatriculaDetectedB = value;}

            QString getMatriculaB() const{return MatriculaB;}
            void setMatriculaB(const QString &value){MatriculaB = value;}

            cv::Mat getMatriculaFotoB() const{return MatriculaFotoB; }
            void setMatriculaFotoB(const cv::Mat &value);

            QByteArray getMatriculaFotoBByte() const{ return MatriculaFotoBByte; }

            QImage getMatriculaFotoBQ() const{return MatriculaFotoBQ;}


            float getMatriculaPrecisionB() const{ return MatriculaPrecisionB; }
            void setMatriculaPrecisionB(float value){ MatriculaPrecisionB = value; }

            QString getMatriculaPrecisionBs() const{ return MatriculaPrecisionBs; }
            void setMatriculaPrecisionBs(const QString &value){ MatriculaPrecisionBs = value; }

            void setOrigenFotoPrewarpQ(const QImage &value);
            void setOrigenFotoBlancaQ( const QImage &value);
            void setOrigenFotoRojaQ(   const QImage &value);
            void setMatriculaFotoAByte(const QByteArray &value);
            void setMatriculaFotoAQ(   const QImage &value);
            void setMatriculaFotoBByte(const QByteArray &value);
            void setMatriculaFotoBQ(   const QImage &value);
        public:
            inline void convertirFotos(){
                Tools *m_tools        =  new Tools;
                setOrigenFotoPrewarpQ(m_tools->convertMat2QImage(getOrigenFotoPrewarp().clone()));
                setOrigenFotoBlancaQ( m_tools->convertMat2QImage(getOrigenFotoBlanca().clone()));
                setOrigenFotoRojaQ(   m_tools->convertMat2QImage(getOrigenFotoRoja().clone()));

                setMatriculaFotoAByte(m_tools->convertMat2ByteArray(getMatriculaFotoA().clone()));
                setMatriculaFotoAQ(   m_tools->convertMat2QImage(getMatriculaFotoA().clone()));

                setMatriculaFotoBByte(m_tools->convertMat2ByteArray(getMatriculaFotoB().clone()));
                setMatriculaFotoBQ(   m_tools->convertMat2QImage(getMatriculaFotoB().clone()));
                delete m_tools;
            }
        };
        #endif

        /** REPORTS **/
        #ifndef NQTRUCKS_REGISTROMATRICULAS_H
        #define NQTRUCKS_REGISTROMATRICULAS_H
        class RegistroMatriculas{
        public:
            //RegistroMatriculas(QObject *parent=nullptr):QObject(parent){}
            explicit RegistroMatriculas();
            void setRegistroMatriculas(const RegistroMatriculas &value);
            void clear();

            Bascula          *m_bascula;
            MatriculaResults *m_results0;// = QVector<Registros::MatriculaResults>(2);
            MatriculaResults *m_results1;// = QVector<Registros::MatriculaResults>(2);

            long long getId() const;
            void setId(long long value);

            QDateTime getFechaRegistro() const;
            void setFechaRegistro(const QDateTime &value);

        private:
            long long   id ;//                  =0;                                                     //id fuente de captura de foto
            QDateTime   FechaRegistro;

        };
        #endif
    } // namespace Registros /** END REPORTS **/
    /** END SETTINGS **/


    namespace Const{
    } //end namespace Const
} // namespace nQTrucks
#endif // NQTRUCKS_H
