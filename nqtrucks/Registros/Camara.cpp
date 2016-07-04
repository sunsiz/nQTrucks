#include <QLoggingCategory>
#include "Camara.h"
    /** CAMARA **********************************************************************************************/
namespace nQTrucks {
        Camara::Camara(){
            /** Camara Types **/
            QLoggingCategory::setFilterRules("qt.network.ssl.warning=false");
            qRegisterMetaType<Camara>("Camara");
            //m_OrigenFoto = cv::Mat(fotoSize, CV_8UC3 );
            convertirFotos();
        }
        void Camara::setCamara(const Camara &value){
            m_OrigenFoto.release();
            m_OrigenFotoByte.clear();
            m_OrigenFotoQ.detach();
            value.m_OrigenFoto.copyTo(m_OrigenFoto);
            m_OrigenFotoByte=value.m_OrigenFotoByte;
            m_OrigenFotoQ=value.m_OrigenFotoQ;
        }

        void Camara::setCamara(Camara *value){
            value->m_OrigenFoto.copyTo(m_OrigenFoto);
            m_OrigenFotoByte.clear();
            m_OrigenFotoQ.detach();
            m_OrigenFotoByte=value->m_OrigenFotoByte;
            m_OrigenFotoQ=value->m_OrigenFotoQ;

        }
        Camara::~Camara(){
            m_OrigenFoto.release();
            m_OrigenFotoByte.clear();
            m_OrigenFotoQ.detach();
        }
        void Camara::setOrigenFoto(const cv::Mat &value){
//            m_OrigenFoto.release();
            value.copyTo(m_OrigenFoto);
        }
    /** END CAMARA **********************************************************************************************/
}




