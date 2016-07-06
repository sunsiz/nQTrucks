#include <QLoggingCategory>
#include "Camara.h"
    /** CAMARA **********************************************************************************************/
namespace nQTrucks {
        Camara::Camara(QObject *parent)
            :QObject(parent)
        {
            /** Camara Types **/
            QLoggingCategory::setFilterRules("qt.network.ssl.warning=false");
            qRegisterMetaType<Camara>("Camara");
        }
        void Camara::setCamara(const Camara &value){
            m_OrigenFoto.release();
            m_OrigenFoto=value.m_OrigenFoto.clone();
        }
        Camara::~Camara(){
            m_OrigenFoto.release();
        }
        void Camara::setOrigenFoto(const cv::Mat &value){
            m_OrigenFoto.release();
            m_OrigenFoto = value.clone();
        }
    /** END CAMARA **********************************************************************************************/
}




