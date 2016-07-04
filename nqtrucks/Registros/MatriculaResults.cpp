#include "MatriculaResults.h"
namespace nQTrucks{
        MatriculaResults::MatriculaResults(){ /** MEMORY LEAK **/
            qRegisterMetaType<Planck>("Planck");
            qRegisterMetaType<t_Prewarp>("t_Prewarp");

            camara = new Camara;
            tipo =0;                        // 0 para calibracion, 1 para procesado
            id   =0;                        // id fuente de captura de foto

            MatriculaDetectedA=false;        // Coincide con un patron de busqueda?
            MatriculaA="";                  // STring de la matricula
            MatriculaPrecisionA=0;          // Precision del OCR
            MatriculaPrecisionAs="0%";

            MatriculaDetectedB=false;        // Coincide con un patron de busqueda?
            MatriculaB="";                  // STring de la matricula
            MatriculaPrecisionB=0;          // Precision del OCR
            MatriculaPrecisionBs="0%";
        }

        MatriculaResults::~MatriculaResults(){
            clear();
        }

        void MatriculaResults::setMatriculaResults(const MatriculaResults &value){
            clear();
            setTipo(value.getTipo());                                  // 0 para calibracion, 1 para procesado
            setId(value.getId());                                      // id fuente de captura de foto
            camara = new Camara(value.camara);

            setOrigenFotoPrewarp(value.getOrigenFotoPrewarp());

            setOrigenFotoBlanca(value.getOrigenFotoBlanca());          // Imagen con calibracion de Blancos
            setOrigenFotoRoja(value.getOrigenFotoRoja());              // Imagen con calibracion de Rojos

            setMatriculaDetectedA(value.getMatriculaDetectedA());      // Coincide con un patron de busqueda?
            setMatriculaA(value.getMatriculaA());                      // STring de la matricula
            setMatriculaFotoA(value.getMatriculaFotoA());              // Imagen recortada de la Matricula
            setMatriculaPrecisionA(value.getMatriculaPrecisionA());    // Precision del OCR
            setMatriculaPrecisionAs(value.getMatriculaPrecisionAs());

            setMatriculaDetectedB(value.getMatriculaDetectedB());      // Coincide con un patron de busqueda?
            setMatriculaB(value.getMatriculaB());                      // STring de la matricula
            setMatriculaFotoB(value.getMatriculaFotoB());              // Imagen recortada de la Matricula
            setMatriculaPrecisionB(value.getMatriculaPrecisionB());    // Precision del OCR
            setMatriculaPrecisionBs(value.getMatriculaPrecisionBs());
            //convertirFotos();

        }

        void MatriculaResults::clear()
        {
            tipo =0;                        // 0 para calibracion, 1 para procesado
            id   =0;                        // id fuente de captura de foto
            //camara->clear();
            //delete camara;

            OrigenFotoPrewarp.release();    // Imagen con calibracion prewarp
            OrigenFotoPrewarpQ.detach();

            OrigenFotoBlanca.release();     //  Imagen con calibracion de Blancos
            OrigenFotoBlancaQ.detach();
            OrigenFotoRoja.release();       // Imagen con calibracion de Rojos
            OrigenFotoRojaQ.detach();

            MatriculaDetectedA=NULL;        // Coincide con un patron de busqueda?
            MatriculaA="";                  // STring de la matricula
            MatriculaFotoA.release();       // Imagen recortada de la Matricula
            MatriculaFotoAByte.clear();
            MatriculaFotoAQ.detach();
            MatriculaPrecisionA=0;          // Precision del OCR
            MatriculaPrecisionAs="0%";

            MatriculaDetectedB=NULL;        // Coincide con un patron de busqueda?
            MatriculaB="";                  // STring de la matricula
            MatriculaFotoB.release();       // Imagen recortada de la Matricula
            MatriculaFotoBByte.clear();
            MatriculaFotoBQ.detach();
            MatriculaPrecisionB=0;          // Precision del OCR
            MatriculaPrecisionBs="0%";
            //convertirFotos();
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
        void MatriculaResults::setOrigenFotoBlancaQ(const QImage &value){
            OrigenFotoBlancaQ.detach();
            OrigenFotoBlancaQ = value;

        }
        void MatriculaResults::setOrigenFotoRoja(const cv::Mat &value){
            OrigenFotoRoja.release();
            OrigenFotoRoja = value.clone();
            convertirFotoRoja();
        }
        void MatriculaResults::setOrigenFotoRojaQ(const QImage &value){
            OrigenFotoRojaQ.detach();
            OrigenFotoRojaQ = value;
        }
        void MatriculaResults::setMatriculaFotoA(const cv::Mat &value){
            MatriculaFotoA.release();
            //MatriculaFotoA=cv::Mat( matriculaSize, CV_8UC3 );
            MatriculaFotoA = value.clone();
            convertirMatriculaFotoA();
        }

        void MatriculaResults::setMatriculaFotoB(const cv::Mat &value){
            MatriculaFotoB.release();
            //MatriculaFotoA=cv::Mat( matriculaSize, CV_8UC3 );
            MatriculaFotoB = value.clone();
            convertirMatriculaFotoB();
        }

        void MatriculaResults::setOrigenFotoPrewarpQ(const QImage &value){
            OrigenFotoPrewarpQ.detach();
            OrigenFotoPrewarpQ = value;
        }
        void MatriculaResults::setMatriculaFotoAByte(const QByteArray &value){
            MatriculaFotoAByte.clear();
            MatriculaFotoAByte = value;
        }
        void MatriculaResults::setMatriculaFotoAQ(const QImage &value){
            MatriculaFotoAQ.detach();
            MatriculaFotoAQ = value;
        }

        void MatriculaResults::setMatriculaFotoBByte(const QByteArray &value){
            MatriculaFotoBByte.clear();
            MatriculaFotoBByte = value;
        }

        void MatriculaResults::setMatriculaFotoBQ(const QImage &value){
            MatriculaFotoBQ.detach();
            MatriculaFotoBQ = value;
        }
}


