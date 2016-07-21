#ifndef NQTRUCKS_CAMARA_H
#define NQTRUCKS_CAMARA_H

#include "Tools.h"
#include <memory>

/** CAMARA **/
namespace nQTrucks{

    #define CAMARA1     "Camara1"
    #define CAMARA2     "Camara2"
    static const int MatriculaNewWidth = 520;
    static const int MatriculaNewHeight = 110;
    static const cv::Size MatriculaSize(MatriculaNewWidth,MatriculaNewHeight);

    static const int FotoWidth  = 1920;
    static const int FotoHeight = 1080;
    static const cv::Size FotoSize(FotoWidth,FotoHeight);

        class Camara : public QObject
        {
            Q_OBJECT
        public:
            explicit Camara(QObject *parent=nullptr);
            ~Camara();
            Camara(const Camara &other){this->setCamara(other);}
            Camara& operator=( const Camara& ) { return *this;  }
            Camara*         getCamara()          { return  this; }
            const   Camara *getCamara()    const { return  this; }
            void setCamara(const Camara &value);
            void setCamara(      Camara *value);

        private:
            cv::Mat           m_OrigenFoto   ;//     = cv::Mat::zeros(FotoSize, CV_8U );                 //Imagen Original
        public:
            void       setOrigenFoto(const cv::Mat &value);
            cv::Mat    getOrigenFoto() const{    return m_OrigenFoto;}

        };
}
#endif
/** END CAMARA **/
