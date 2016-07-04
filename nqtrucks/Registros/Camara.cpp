#include <QLoggingCategory>
#include "Camara.h"
    /** CAMARA **********************************************************************************************/
namespace nQTrucks {
        Camara::Camara(QObject *parent)
            : QObject(parent)
        {
            /** Camara Types **/
            QLoggingCategory::setFilterRules("qt.network.ssl.warning=false");
            //qRegisterMetaType<Camara>("Camara");
            //m_OrigenFoto = cv::Mat(fotoSize, CV_8UC3 );
            convertirFotos();
        }

        void Camara::setCamara(const Camara &value)
        {
            m_OrigenFoto=value.m_OrigenFoto;
            m_OrigenFotoByte=value.m_OrigenFotoByte;
            m_OrigenFotoQ=value.m_OrigenFotoQ;
        }

        void Camara::setCamara(Camara *value)
        {
            m_OrigenFoto=value->m_OrigenFoto;
            m_OrigenFotoByte=value->m_OrigenFotoByte;
            m_OrigenFotoQ=value->m_OrigenFotoQ;

        }
//        Camara::~Camara(){
//            m_OrigenFoto.release();
//            m_OrigenFotoByte.clear();
//            m_OrigenFotoQ.detach();
//            //deleteLater();
//        }
        void Camara::setOrigenFoto(const cv::Mat value){
            //m_OrigenFoto.release();
            //m_OrigenFoto = cv::Mat(FotoSize, CV_8UC3 );
            m_OrigenFoto = value.clone();
            convertirFotos();
        }
    /** END CAMARA **********************************************************************************************/
}




