#include "Tools.h"
#include <QMetaEnum>

namespace nQTrucks{
        Tools::Tools(QObject *parent)
            :QObject(parent){
            //qRegisterMetaType<Tools>  ("Tools");
            qRegisterMetaType<cv::Mat>("cv::Mat");
        }




}
