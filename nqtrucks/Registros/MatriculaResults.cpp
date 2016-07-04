#include "MatriculaResults.h"
namespace nQTrucks{


        MatriculaResults::MatriculaResults(){
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

            value.OrigenFotoPrewarp.copyTo(OrigenFotoPrewarp);
            value.OrigenFotoBlanca.copyTo( OrigenFotoBlanca);          // Imagen con calibracion de Blancos
            value.OrigenFotoRoja.copyTo(   OrigenFotoRoja);              // Imagen con calibracion de Rojos

            value.MatriculaFotoA.copyTo(   MatriculaFotoA);              // Imagen recortada de la Matricula
            value.MatriculaFotoB.copyTo(   MatriculaFotoB);              // Imagen recortada de la Matricula
            MatriculaDetectedA      = value.MatriculaDetectedA;      // Coincide con un patron de busqueda?
            MatriculaDetectedB      = value.MatriculaDetectedB;      // Coincide con un patron de busqueda?
            MatriculaA              = value.MatriculaA;                      // STring de la matricula
            MatriculaB              = value.MatriculaB;                      // STring de la matricula
            setMatriculaPrecisionA(value.MatriculaPrecisionA);    // Precision del OCR
            setMatriculaPrecisionB(value.MatriculaPrecisionB);    // Precision del OCR
            convertirFotos();
        }






        void MatriculaResults::setOrigenFotoPrewarp(const cv::Mat &value){            
            value.copyTo(OrigenFotoPrewarp);
        }
        void MatriculaResults::setOrigenFotoBlanca(const cv::Mat &value){
            value.copyTo(OrigenFotoBlanca);
        }
        void MatriculaResults::setOrigenFotoRoja(const cv::Mat &value){
            value.copyTo(OrigenFotoRoja);
        }
        void MatriculaResults::setMatriculaFotoA(const cv::Mat &value){
            value.copyTo(MatriculaFotoA);
        }

        void MatriculaResults::setMatriculaFotoB(const cv::Mat &value){
            value.copyTo(MatriculaFotoB);
        }


}


