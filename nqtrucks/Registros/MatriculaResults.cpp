#include "MatriculaResults.h"
namespace nQTrucks{


        MatriculaResults::MatriculaResults()
            //:Camara(parent)
        { /** MEMORY LEAK **/
            qRegisterMetaType<Planck>          ("Planck");
            qRegisterMetaType<t_Prewarp>       ("t_Prewarp");
            qRegisterMetaType<MatriculaResults>("MatriculaResults");
            convertirFotos();
        }

        MatriculaResults::~MatriculaResults(){
            OrigenFotoPrewarp.release();    // Imagen con calibracion prewarp
            OrigenFotoPrewarpQ.detach();

            OrigenFotoBlanca.release();     //  Imagen con calibracion de Blancos
            OrigenFotoBlancaQ.detach();
            OrigenFotoRoja.release();       // Imagen con calibracion de Rojos
            OrigenFotoRojaQ.detach();
            MatriculaFotoA.release();       // Imagen recortada de la Matricula
            MatriculaFotoAByte.clear();
            MatriculaFotoAQ.detach();
            MatriculaFotoB.release();       // Imagen recortada de la Matricula
            MatriculaFotoBByte.clear();
            MatriculaFotoBQ.detach();

        }

        void MatriculaResults::setMatriculaResults(const MatriculaResults &value){
            setTipo(value.getTipo());                                  // 0 para calibracion, 1 para procesado
            setId(value.getId());                                      // id fuente de captura de foto

            setOrigenFotoPrewarp(  value.getOrigenFotoPrewarp());
            setOrigenFotoBlanca(   value.getOrigenFotoBlanca());          // Imagen con calibracion de Blancos
            setOrigenFotoRoja(     value.getOrigenFotoRoja());              // Imagen con calibracion de Rojos

            setMatriculaDetectedA( value.getMatriculaDetectedA());      // Coincide con un patron de busqueda?
            setMatriculaDetectedB( value.getMatriculaDetectedB());      // Coincide con un patron de busqueda?
            setMatriculaA(         value.getMatriculaA());                      // STring de la matricula
            setMatriculaB(         value.getMatriculaB());                      // STring de la matricula
            setMatriculaFotoA(     value.getMatriculaFotoA());              // Imagen recortada de la Matricula
            setMatriculaFotoB(     value.getMatriculaFotoB());              // Imagen recortada de la Matricula
            setMatriculaPrecisionA(value.getMatriculaPrecisionA());    // Precision del OCR
            setMatriculaPrecisionB(value.getMatriculaPrecisionB());    // Precision del OCR

        }

        void MatriculaResults::setPlanckBlanco(const Planck &value){
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

        void MatriculaResults::setPlanckRojo(const Planck &value){
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


        void MatriculaResults::setOrigenFotoPrewarp(const cv::Mat &value){
            OrigenFotoPrewarp.release();
            OrigenFotoPrewarp = value.clone();
            convertirFotoPrewarp();
        }
        void MatriculaResults::setOrigenFotoBlanca(const cv::Mat &value){
            OrigenFotoBlanca.release();
            OrigenFotoBlanca = value.clone();
            convertirFotoBlanca();
        }
        void MatriculaResults::setOrigenFotoRoja(const cv::Mat &value){
            OrigenFotoRoja.release();
            OrigenFotoRoja = value.clone();
            convertirFotoRoja();
        }
        void MatriculaResults::setMatriculaFotoA(const cv::Mat &value){
            MatriculaFotoA.release();
            MatriculaFotoA = value.clone();
            convertirMatriculaFotoA();
        }

        void MatriculaResults::setMatriculaFotoB(const cv::Mat &value){
            MatriculaFotoB.release();
            MatriculaFotoB = value.clone();
            convertirMatriculaFotoB();
        }

        void MatriculaResults::convertirFotos(){
            convertirFotoPrewarp();
            convertirFotoBlanca();
            convertirFotoRoja();
            convertirMatriculaFotoA();
            convertirMatriculaFotoB();
        }
}


