#ifndef NQTRUCKS_CAMARA_H
#define NQTRUCKS_CAMARA_H

#include <QObject>
#include "nqtglobal.h"
#include "Tools.h"
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>


/** CAMARA **/
namespace nQTrucks{
    namespace Registros{

    static const int MatriculaNewWidth = 520;
    static const int MatriculaNewHeight = 110;
    static const cv::Size MatriculaSize(MatriculaNewWidth,MatriculaNewHeight);

    static const int FotoWidth = 1280;
    static const int FotoHeight = 720;
    static const cv::Size FotoSize(FotoWidth,FotoHeight);

        class Camara : public QObject
        {
            Q_OBJECT
        public:
            explicit Camara(QObject *parent=nullptr);
        //            // Return by pointer needs const and non-const versions
        //                  Camara*  getCamara()       { return this; }
        //            const Camara*  getCamara() const { return this; }

        //            // Return by reference needs const and non-const versions
        //                  Camara& getCamara()       { return *this; }
        //            const Camara& getCamara() const { return *this; }

            // Return by value only needs one version.
            //Camara getCamara() const { return *this; }
//            Camara* getBascula()       { return this; }
//            const Camara* getBascula() const { return this; }
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
    }
}
#endif
/** END CAMARA **/
