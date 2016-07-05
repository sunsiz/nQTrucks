#include <QLoggingCategory>
#include "Camara.h"
    /** CAMARA **********************************************************************************************/
namespace nQTrucks {
        Camara::Camara(){
            /** Camara Types **/
            QLoggingCategory::setFilterRules("qt.network.ssl.warning=false");
            qRegisterMetaType<Camara>("Camara");
        }
        void Camara::setCamara(const Camara &value){
            m_OrigenFoto.release();
            value.m_OrigenFoto.copyTo(m_OrigenFoto);
        }

        void Camara::setCamara(Camara *value){
            m_OrigenFoto.release();
            value->m_OrigenFoto.copyTo(m_OrigenFoto);

        }

        void Camara::setOrigenFotoQ(const QImage &OrigenFotoQ)
        {
            m_OrigenFotoQ = OrigenFotoQ;
        }

        void Camara::setOrigenFotoByte(const QByteArray &OrigenFotoByte)
        {
            m_OrigenFotoByte = OrigenFotoByte;
        }
        Camara::~Camara(){
            m_OrigenFoto.release();
            m_OrigenFotoByte.clear();
            m_OrigenFotoQ.detach();
        }
        void Camara::setOrigenFoto(const cv::Mat &value){
            value.copyTo(m_OrigenFoto);
        }
    /** END CAMARA **********************************************************************************************/
}




