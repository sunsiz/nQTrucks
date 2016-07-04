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
            inline QImage convertMat2QImage(const cv::Mat &_cvimage){
                QImage qtImg= QImage();
                if( !_cvimage.empty() && _cvimage.depth() == CV_8U ){
                    if(_cvimage.channels() == 1){
                        qtImg = QImage( (const unsigned char *)(_cvimage.data),
                                        _cvimage.cols,
                                        _cvimage.rows,
                                        //_cvimage.step,
                                        QImage::Format_Indexed8 ).copy();
                    }
                    else{
                        cv::cvtColor( _cvimage, _cvimage, CV_BGR2RGB );
                        qtImg = QImage( (const unsigned char *)(_cvimage.data),
                                        _cvimage.cols,
                                        _cvimage.rows,
                                        //_cvimage.step,
                                        QImage::Format_RGB888 ).copy();
                    }
                }
                return qtImg;
            }

            inline cv::Mat  convertQImage2Mat(   const QImage     &_qimage){
                QByteArray baScene; // byte array with data
                QBuffer buffer(&baScene);
                buffer.open(QIODevice::WriteOnly);
                _qimage.save(&buffer,"JPG");

                const char* begin = reinterpret_cast<char*>(baScene.data());
                const char* end = begin + baScene.size();
                std::vector<char> pic(begin, end);
                buffer.close();
                baScene.clear();
                return cv::Mat(cv::imdecode(pic,CV_LOAD_IMAGE_COLOR));//.clone();
                //return _cvtemp;
            }

            inline QByteArray convertMat2ByteArray(const cv::Mat    &_cvimage){
                QImage qtImg = convertMat2QImage(_cvimage.clone());
                QByteArray baScene; // byte array with data
                QBuffer buffer(&baScene);
                buffer.open(QIODevice::WriteOnly);
                qtImg.save(&buffer,"JPG");
                buffer.close();
                return baScene;
            }

            //static cv::Mat    convertByteArray2Mat(QByteArray _Bytearray);
            //static QByteArray resizeByteArray2ByteArray(QByteArray _ByteArray, const int &_w, const int &_h);
        };
/** END CONVERSORES **********************************************************/
}
#endif  //TOOLS
