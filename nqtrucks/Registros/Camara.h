#ifndef NQTRUCKS_CAMARA_H
#define NQTRUCKS_CAMARA_H

#include "Tools.h"

/** CAMARA **/
namespace nQTrucks{

    #define CAMARA1     "Camara1"
    #define CAMARA2     "Camara2"
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
            Camara*         getCamara()       { return this; }
            const   Camara* getCamara() const { return this; }
            Camara&         getCamaraPtr()       { return *this; }
            const   Camara& getCamaraPtr() const { return *this; }
            void setCamara(const Camara &value);
            void setCamara(Camara* value);

            //~Camara();
        private:
            cv::Mat           m_OrigenFoto        = cv::Mat::zeros(FotoSize, CV_8UC3 );                 //Imagen Original
            QByteArray        m_OrigenFotoByte;
            QImage            m_OrigenFotoQ       = QImage(FotoWidth,FotoHeight,QImage::Format_RGB888);
        public:
            void       setOrigenFoto(const cv::Mat value);
            cv::Mat    getOrigenFoto() const{return m_OrigenFoto;}
            QByteArray getOrigenFotoByte() const{return m_OrigenFotoByte;}
            QImage     getOrigenFotoQ() const{ return m_OrigenFotoQ; }

            inline void convertirFotos(){
                Tools *m_tools  = new Tools; /** MEMORY LEAK **/
                m_OrigenFotoByte.clear();
                m_OrigenFotoQ.detach();
                m_OrigenFotoByte = m_tools->convertMat2ByteArray(m_OrigenFoto.clone()); /** MEMORY LEAK **/
                m_OrigenFotoQ    = m_tools->convertMat2QImage(   m_OrigenFoto.clone());
                delete m_tools;
                emit ReplyCamaraIP(*this);
            }
        signals:
            void ReplyCamaraIP(const Camara &_ReplyCamara);
        };
}
#endif
/** END CAMARA **/
