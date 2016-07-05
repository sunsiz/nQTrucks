#ifndef NQTRUCKS_MARICULARESULTS_H
#define NQTRUCKS_MARICULARESULTS_H

#include "Camara.h"
namespace nQTrucks{
    static const QString default_prewarp ="";//  "planar,1280,720,0,0,0,1.0,1.0,0,0";

    #define ALPR_PLANCK_BLANCO 0
    #define ALPR_PLANCK_ROJO   1

    #define ALPR       "Alpr"
    #define ALPR1      "Alpr1"
    #define ALPR2      "Alpr2"

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

    class Planck{
    public:
        int A=0;
        int B=0;
        int C=0;
    };

        class MatriculaResults {
        public:
            MatriculaResults();
            ~MatriculaResults();
            Camara     *camara = new Camara;
            MatriculaResults&       getMatriculaResults()       { return *this; }
            const MatriculaResults& getMatriculaResults() const { return *this; }

            inline void setPlanckBlanco(const Planck &value){
                cv::Mat channel[3];
                cv::Mat _dest;
                _dest = this->camara->getOrigenFoto().clone();
                cv::add(_dest,cv::Scalar(value.C,value.B,value.A), _dest);
                cv::split(_dest, channel);
                this->setOrigenFotoBlanca(channel[2] - channel[1] -   channel[2] + channel[0]); /** MEMORY LEAK **/
                channel[0].release();
                channel[1].release();
                channel[2].release();
                _dest.release();
                //delete _dest;
            }

            inline void setPlanckRojo(  const Planck &value){
                cv::Mat channel[3];
                cv::Mat *_dest = new cv::Mat(this->camara->getOrigenFoto().clone());
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
            }

        private:
            int        tipo;//                 =0;                                                     //0 para calibracion, 1 para procesado
            int        id ;//                  =0;                                                     //id fuente de captura de foto

            cv::Mat    OrigenFotoPrewarp      ;//= cv::Mat::zeros(FotoSize, CV_8U );                    // Imagen con calibracion prewarp
            cv::Mat    OrigenFotoBlanca       ;//= cv::Mat::zeros(FotoSize, CV_8U );                    //  Imagen con calibracion de Blancos
            cv::Mat    OrigenFotoRoja         ;//= cv::Mat::zeros(FotoSize, CV_8U );                    // Imagen con calibracion de Rojos

            bool       MatriculaDetectedA     = false;                                                 // Coincide con un patron de busqueda?
            bool       MatriculaDetectedB     = false;                                                 // Coincide con un patron de busqueda?
            QString    MatriculaA             = QString("");                                                    // STring de la matricula
            QString    MatriculaB             = QString("");                                                    // STring de la matricula
            cv::Mat    MatriculaFotoA         ;//= cv::Mat::zeros(MatriculaSize, CV_8U );              // Imagen recortada de la Matricula
            cv::Mat    MatriculaFotoB         ;//= cv::Mat::zeros(MatriculaSize, CV_8U );              // Imagen recortada de la Matricula
            float      MatriculaPrecisionA    = 0;                                                     // Precision del OCR
            float      MatriculaPrecisionB    = 0;                                                     // Precision del OCR
            QString    MatriculaPrecisionAs   = QString("0%");
            QString    MatriculaPrecisionBs   = QString("0%");

        public:
            void     setMatriculaResults(const MatriculaResults &value);
            //void     clear();
            void setId(  int value){id   = value;}
            void setTipo(int value){tipo = value;}
            int  getId()   const{return id;}
            int  getTipo() const{return tipo;}


            void setOrigenFotoPrewarp(const cv::Mat &value);
            void setOrigenFotoBlanca( const cv::Mat &value);
            void setOrigenFotoRoja(   const cv::Mat &value);

            cv::Mat getOrigenFotoPrewarp() const{return OrigenFotoPrewarp; }
            cv::Mat getOrigenFotoBlanca()  const{return OrigenFotoBlanca;}
            cv::Mat getOrigenFotoRoja()    const{return OrigenFotoRoja;}

            void setMatriculaDetectedA( bool           value){ MatriculaDetectedA = value; }
            void setMatriculaDetectedB( bool           value){ MatriculaDetectedB = value; }
            void setMatriculaA(         const QString &value){ MatriculaA         = value; }
            void setMatriculaB(         const QString &value){ MatriculaB         = value; }
            void setMatriculaFotoA(     const cv::Mat &value);
            void setMatriculaFotoB(     const cv::Mat &value);
            void setMatriculaPrecisionA(float          value){ MatriculaPrecisionA = value; MatriculaPrecisionAs = QString::number(value,'g',6);}
            void setMatriculaPrecisionB(float          value){ MatriculaPrecisionB = value; MatriculaPrecisionBs = QString::number(value,'g',6);}

            bool       getMatriculaDetectedA()   const{return MatriculaDetectedA; }
            bool       getMatriculaDetectedB()   const{return MatriculaDetectedB; }
            QString    getMatriculaA()           const{return MatriculaA;}
            QString    getMatriculaB()           const{return MatriculaB;}
            cv::Mat    getMatriculaFotoA()       const{return MatriculaFotoA; }
            cv::Mat    getMatriculaFotoB()       const{return MatriculaFotoB; }
            float      getMatriculaPrecisionA()  const{return MatriculaPrecisionA;}
            float      getMatriculaPrecisionB()  const{return MatriculaPrecisionB;}
            QString    getMatriculaPrecisionAs() const{return MatriculaPrecisionAs; }
            QString    getMatriculaPrecisionBs() const{return MatriculaPrecisionBs; }
        };
}
#endif // MATRICULARESULTS_H
