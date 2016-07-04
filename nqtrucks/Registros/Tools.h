#ifndef NQTRUCKS_TOOLS_H
#define NQTRUCKS_TOOLS_H

#include <QObject>
#include <QImage>
#include <QSettings>
#include <QByteArray>
#include <QVariant>
#include <QBuffer>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>

/** CONVERSORES ********************************************************/
namespace nQTrucks{
        class Tools: public QObject{
            Q_OBJECT
        public:
            explicit Tools(QObject *parent=nullptr);
            QImage     convertMat2QImage(   const cv::Mat &_cvimage);
            QByteArray convertMat2ByteArray(const cv::Mat &_cvimage);

//            inline cv::Mat  convertQImage2Mat(   const QImage     &_qimage){
//                QByteArray baScene; // byte array with data
//                QBuffer buffer(&baScene);
//                buffer.open(QIODevice::WriteOnly);
//                _qimage.save(&buffer,"JPG");

//                const char* begin = reinterpret_cast<char*>(baScene.data());
//                const char* end = begin + baScene.size();
//                std::vector<char> pic(begin, end);
//                buffer.reset();
//                buffer.close();
//                baScene.clear();
//                return cv::Mat(cv::imdecode(pic,CV_LOAD_IMAGE_COLOR)).clone();
//                //return _cvtemp;
//            }


            //static cv::Mat    convertByteArray2Mat(QByteArray _Bytearray);
            //static QByteArray resizeByteArray2ByteArray(QByteArray _ByteArray, const int &_w, const int &_h);
        };
/** END CONVERSORES **********************************************************/
}
#endif  //TOOLS
