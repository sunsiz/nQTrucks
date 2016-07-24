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
#include <opencv2/core/core.hpp>
#include <QVector>

/** CONVERSORES ********************************************************/
namespace nQTrucks{
        class Tools : public QObject
        {
            Q_OBJECT
        public:
            Tools(QObject *parent=nullptr);

//            static inline QImage convertMat2QImage(const cv::Mat& _cvimage){
//                QImage qtImg;
//                cv::Mat _temp;
//                if( !_cvimage.empty() && _cvimage.depth() == CV_8U){
//                    if(_cvimage.channels() == 1){
//                        cv::cvtColor( _cvimage, _temp, CV_GRAY2RGB );
//                    }
//                    else{
//                        cv::cvtColor( _cvimage, _temp, CV_BGR2RGB );
//                    }
//                }
//                assert(_temp.isContinuous());
//                qtImg = QImage((const unsigned char *)(_temp.data),
//                               _temp.cols,
//                               _temp.rows,
//                               _temp.cols*3,
//                               QImage::Format_RGB888);
//                return qtImg;
//            }


//            static inline void convertMat2QImage( cv::Mat &_cvimage, QImage &_qtImg ){
//                if( !_cvimage.empty() && _cvimage.depth() == CV_8U){
//                    if(_cvimage.channels() == 1){
//                        _qtImg.fromData((const uchar *)(_cvimage.data),
//                                       QSize(_cvimage.cols,_cvimage.row));
//                    }
//                    else{
//                        cv::cvtColor( _cvimage, _cvimage, CV_BGR2RGB );
//                        _qtImg.fromData((const unsigned char *)(_cvimage.data),
//                                       QSize(_cvimage.cols,_cvimage.rows),
//                                       "JPG" );
//                    }
//                }
//            }
//            static inline void convertMat2ByteArray(cv::Mat _cvimage,QByteArray &_baScene){
//                QImage qtImg;
//                convertMat2QImage(_cvimage,qtImg);
//                //QByteArray baScene; // byte array with data
//                QBuffer buffer(&_baScene);
//                buffer.open(QIODevice::WriteOnly);
//                qtImg.save(&buffer,"JPG");
//                buffer.reset();
//                buffer.close();
//                qtImg.detach();
//            }


            static inline QImage convertMat2QImage(cv::Mat const &_cvimage){
                QImage qtImg;
                if( !_cvimage.empty() && _cvimage.depth() == CV_8U){
                    if(_cvimage.channels() == 1){
                        qtImg = QImage((const unsigned char *)(_cvimage.data),
                                       //_cvimage.data,
                                       _cvimage.cols,
                                       _cvimage.rows,
                                       //_cvimage.step,
                                       QImage::Format_Indexed8).copy();
                    }
                    else{
                        cv::Mat rgb;
                        cv::cvtColor( _cvimage, rgb, CV_BGR2RGB );
                        qtImg = QImage((const unsigned char *)(rgb.data),
                                       //rgb.data,
                                       rgb.cols,
                                       rgb.rows,
                                       //rgb.step,
                                       QImage::Format_RGB888 ).copy();
                    }
                }
                return qtImg;
            }

            static inline QByteArray convertMat2ByteArray(const cv::Mat& _cvimage){
                QImage qtImg;
                qtImg = convertMat2QImage(_cvimage);
                QByteArray baScene; // byte array with data
                QBuffer buffer(&baScene);
                buffer.open(QIODevice::WriteOnly);
                //qtImg.save(&buffer,"PNG");
                qtImg.save(&buffer,"JPG");
                buffer.reset();
                buffer.close();
                qtImg.detach();
                return baScene;
            }
        };
/** END CONVERSORES **********************************************************/
}
#endif  //TOOLS
