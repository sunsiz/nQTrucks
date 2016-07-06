#include "MatriculaResults.h"
namespace nQTrucks{
        MatriculaResults::MatriculaResults(QObject *parent)
            : QObject(parent)
        {
            qRegisterMetaType<Planck>          ("Planck");
            qRegisterMetaType<t_Prewarp>       ("t_Prewarp");
            qRegisterMetaType<MatriculaResults>("MatriculaResults");
            qRegisterMetaType<cv::Mat>("cv::Mat");
        }

        MatriculaResults::~MatriculaResults(){
            OrigenFotoPrewarp.release();    // Imagen con calibracion prewarp
            OrigenFotoBlanca.release();     //  Imagen con calibracion de Blancos
            OrigenFotoRoja.release();       // Imagen con calibracion de Rojos
            MatriculaFotoA.release();       // Imagen recortada de la Matricula
            MatriculaFotoB.release();       // Imagen recortada de la Matricula
            camara->deleteLater();
        }

        MatriculaResults::MatriculaResults(const MatriculaResults &other)
        {
           this->setMatriculaResults(other);
        }

        void MatriculaResults::setMatriculaResults(const MatriculaResults &other){
            setTipo(                  other.getTipo()            );    // 0 para calibracion, 1 para procesado
            setId(                    other.getId()              );    // id fuente de captura de foto
            setOrigenFotoPrewarp(     other.OrigenFotoPrewarp    );
            setOrigenFotoBlanca(      other.OrigenFotoBlanca     );
            setOrigenFotoRoja(        other.OrigenFotoRoja       );
            setMatriculaFotoA(        other.MatriculaFotoA       );    // Imagen recortada de la Matricula
            setMatriculaFotoB(        other.MatriculaFotoB       );    // Imagen recortada de la Matricula
            setMatriculaPrecisionA(   other.MatriculaPrecisionA  );    // Precision del OCR
            setMatriculaPrecisionB(   other.MatriculaPrecisionB  );    // Precision del OCR
            MatriculaDetectedA      = other.MatriculaDetectedA    ;    // Coincide con un patron de busqueda?
            MatriculaDetectedB      = other.MatriculaDetectedB    ;    // Coincide con un patron de busqueda?
            MatriculaA              = other.MatriculaA            ;    // STring de la matricula
            MatriculaB              = other.MatriculaB            ;    // STring de la matricula

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


