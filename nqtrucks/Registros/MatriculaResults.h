#ifndef NQTRUCKS_MARICULARESULTS_H
#define NQTRUCKS_MARICULARESULTS_H

#include "Camara.h"
#include "nqtglobal.h"

namespace nQTrucks{
    namespace Registros{
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
            inline void convertirFotoPrewarp(){
                Tools *m_tools        =  new Tools;
                setOrigenFotoPrewarpQ(m_tools->convertMat2QImage(getOrigenFotoPrewarp().clone()));
                delete m_tools;
            }
            inline void convertirFotoBlanca(){
                Tools *m_tools        =  new Tools;
                setOrigenFotoBlancaQ( m_tools->convertMat2QImage(getOrigenFotoBlanca().clone()));
                delete m_tools;
            }
            inline void convertirFotoRoja(){
                Tools *m_tools        =  new Tools;
                setOrigenFotoRojaQ(   m_tools->convertMat2QImage(getOrigenFotoRoja().clone()));
                delete m_tools;
            }
            inline void convertirMatriculaFotoA(){
                Tools *m_tools        =  new Tools;
                setMatriculaFotoAByte(m_tools->convertMat2ByteArray(getMatriculaFotoA().clone()));
                setMatriculaFotoAQ(   m_tools->convertMat2QImage(getMatriculaFotoA().clone()));
                delete m_tools;
            }
            inline void convertirMatriculaFotoB(){
                Tools *m_tools        =  new Tools;
                setMatriculaFotoBByte(m_tools->convertMat2ByteArray(getMatriculaFotoB().clone()));
                setMatriculaFotoBQ(   m_tools->convertMat2QImage(getMatriculaFotoB().clone()));
                delete m_tools;
            }
        };
    }
}
#endif // MATRICULARESULTS_H
