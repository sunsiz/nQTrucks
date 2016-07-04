#include "Tools.h"
#include <QMetaEnum>

namespace nQTrucks{
        Tools::Tools(){
            qRegisterMetaType<Tools>  ("Tools");
            qRegisterMetaType<cv::Mat>("cv::Mat");
        }




}
