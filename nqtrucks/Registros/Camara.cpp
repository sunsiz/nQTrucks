#include "Camara.h"
    /** CAMARA **********************************************************************************************/
namespace nQTrucks {
    namespace Registros{
        Camara::Camara(QObject *parent)
            : QObject(parent)
        {
            //m_OrigenFoto = cv::Mat(fotoSize, CV_8UC3 );
        }

        Camara::~Camara(){
            clear();
            //delete m_OrigenFoto;
        }

        void Camara::setCamara(const Camara &value){
            setOrigenFoto(    value.getOrigenFoto());
            setOrigenFotoByte(value.getOrigenFotoByte());
            setOrigenFotoQ(   value.getOrigenFotoQ());
        }
        void Camara::clear(){
            m_OrigenFoto.release();
            m_OrigenFotoByte.clear();
            if (!m_OrigenFotoQ.isNull()) m_OrigenFotoQ.detach();
        }

        cv::Mat Camara::getOrigenFoto() const{
            return m_OrigenFoto;
        }

        void Camara::setOrigenFoto(const cv::Mat value){
            clear();
            //m_OrigenFoto;
            m_OrigenFoto = cv::Mat(FotoSize, CV_8UC3 );
            m_OrigenFoto = value.clone();

        }

        QByteArray Camara::getOrigenFotoByte() const{
            return m_OrigenFotoByte;
        }
        void Camara::setOrigenFotoByte(const QByteArray &OrigenFotoByte){
            m_OrigenFotoByte.clear();
            m_OrigenFotoByte = OrigenFotoByte;
        }

        QImage Camara::getOrigenFotoQ() const{
            return m_OrigenFotoQ;
        }

        void Camara::setOrigenFotoQ(const QImage &OrigenFotoQ){
            m_OrigenFotoQ.detach();
            m_OrigenFotoQ = OrigenFotoQ;
        }
    /** END CAMARA **********************************************************************************************/
    }
}




