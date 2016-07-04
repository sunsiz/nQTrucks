#include "Tools.h"
#include <QMetaEnum>

namespace nQTrucks{
        Tools::Tools(QObject *parent)
            :QObject(parent)
        {
            qRegisterMetaType<cv::Mat>("cv::Mat");
        }

        QImage Tools::convertMat2QImage(const cv::Mat &_cvimage){
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

        QByteArray Tools::convertMat2ByteArray(const cv::Mat &_cvimage){
            QImage qtImg = convertMat2QImage(_cvimage.clone());
            QByteArray baScene; // byte array with data
            QBuffer buffer(&baScene);
            buffer.open(QIODevice::WriteOnly);
            qtImg.save(&buffer,"JPG");
            buffer.reset();
            buffer.close();
            qtImg.detach();
            return baScene;
        }
}
