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
        Camara::~Camara(){
            m_OrigenFoto.release();
            m_OrigenFotoByte.clear();
            m_OrigenFotoQ.detach();
            deleteLater();
        }
        void Camara::setOrigenFoto(const cv::Mat value){
            m_OrigenFoto = cv::Mat(FotoSize, CV_8UC3 );
            m_OrigenFoto = value.clone();
            convertirFotos();
        }
    /** END CAMARA **********************************************************************************************/
}




