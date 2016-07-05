#include "MatriculaResults.h"
namespace nQTrucks{
        MatriculaResults::MatriculaResults(){
            qRegisterMetaType<Planck>          ("Planck");
            qRegisterMetaType<t_Prewarp>       ("t_Prewarp");
            qRegisterMetaType<MatriculaResults>("MatriculaResults");
        }

        MatriculaResults::~MatriculaResults(){
            OrigenFotoPrewarp.release();    // Imagen con calibracion prewarp
            OrigenFotoBlanca.release();     //  Imagen con calibracion de Blancos
            OrigenFotoRoja.release();       // Imagen con calibracion de Rojos
            MatriculaFotoA.release();       // Imagen recortada de la Matricula
            MatriculaFotoB.release();       // Imagen recortada de la Matricula
        }

        void MatriculaResults::setMatriculaResults(const MatriculaResults &value){
            setTipo(                  value.getTipo()            );    // 0 para calibracion, 1 para procesado
            setId(                    value.getId()              );    // id fuente de captura de foto
            setOrigenFotoPrewarp(     value.OrigenFotoPrewarp    );
            setOrigenFotoBlanca(      value.OrigenFotoBlanca     );
            setOrigenFotoRoja(        value.OrigenFotoRoja       );
            setMatriculaFotoA(        value.MatriculaFotoA       );    // Imagen recortada de la Matricula
            setMatriculaFotoB(        value.MatriculaFotoB       );    // Imagen recortada de la Matricula
            setMatriculaPrecisionA(   value.MatriculaPrecisionA  );    // Precision del OCR
            setMatriculaPrecisionB(   value.MatriculaPrecisionB  );    // Precision del OCR
            MatriculaDetectedA      = value.MatriculaDetectedA    ;    // Coincide con un patron de busqueda?
            MatriculaDetectedB      = value.MatriculaDetectedB    ;    // Coincide con un patron de busqueda?
            MatriculaA              = value.MatriculaA            ;    // STring de la matricula
            MatriculaB              = value.MatriculaB            ;    // STring de la matricula
        }

        void MatriculaResults::setOrigenFotoPrewarp(const cv::Mat &value){
            OrigenFotoPrewarp.release();
            value.copyTo(OrigenFotoPrewarp);
        }
        void MatriculaResults::setOrigenFotoBlanca(const cv::Mat &value){
            OrigenFotoBlanca.release();
            value.copyTo(OrigenFotoBlanca);
        }
        void MatriculaResults::setOrigenFotoRoja(const cv::Mat &value){
            OrigenFotoRoja.release();
            value.copyTo(OrigenFotoRoja);
        }
        void MatriculaResults::setMatriculaFotoA(const cv::Mat &value){
            MatriculaFotoA.release();
            value.copyTo(MatriculaFotoA);
        }

        void MatriculaResults::setMatriculaFotoB(const cv::Mat &value){
            MatriculaFotoB.release();
            value.copyTo(MatriculaFotoB);
        }
}


