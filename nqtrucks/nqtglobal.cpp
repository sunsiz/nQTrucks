/***************************************************************************
 *   This file is part of the nQTrucks project                             *
 *   Copyright (C) 2015 by Efraím Pérez                                    *
 *   newsages2014@gmail.com                                                *
 *                                                                         *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/
#include "nqtglobal.h"

#include <QString>
#include <QCoreApplication>
#include <QDir>
#include <QBuffer>

//QSettings * nQTrucks::nQTrucksConfig::m_settings = 0;

QString nQTrucks::extractClassName(QString className)
{
    int startPos=className.lastIndexOf("::");
    if(startPos==-1) startPos=0;
    else startPos+=2;
    return className.right(className.length()-startPos);
}


namespace nQTrucks {
/** TOOLS **************************************************************/
Tools::Tools(QObject *parent)
    :QObject(parent)
{

}
/** CONVERSORES ***********/
cv::Mat Tools::convertQImage2Mat(const QImage &_qimage){
    QByteArray baScene; // byte array with data
    QBuffer buffer(&baScene);
    buffer.open(QIODevice::WriteOnly);
    _qimage.save(&buffer,"PNG");

    const char* begin = reinterpret_cast<char*>(baScene.data());
    const char* end = begin + baScene.size();
    std::vector<char> pic(begin, end);
    buffer.close();
    //baScene.clear();
    return cv::imdecode(pic,CV_LOAD_IMAGE_COLOR).clone();
}

QImage Tools::convertMat2QImage(const cv::Mat &_cvimage)
{
    QImage qtImg= QImage();
    if( !_cvimage.empty() && _cvimage.depth() == CV_8U ){
        if(_cvimage.channels() == 1){
            qtImg = QImage( (const unsigned char *)(_cvimage.data),
                            _cvimage.cols,
                            _cvimage.rows,
                            _cvimage.step,
                            QImage::Format_Indexed8 );
        }
        else{
            cv::cvtColor( _cvimage, _cvimage, CV_BGR2RGB );
            qtImg = QImage( (const unsigned char *)(_cvimage.data),
                            _cvimage.cols,
                            _cvimage.rows,
                            _cvimage.step,
                            QImage::Format_RGB888 );
        }
    }
    return qtImg;
}

QByteArray Tools::convertMat2ByteArray(const cv::Mat &_cvimage){
    QImage qtImg = convertMat2QImage(_cvimage.clone());
    QByteArray baScene; // byte array with data
    QBuffer buffer(&baScene);
    buffer.open(QIODevice::WriteOnly);
    qtImg.save(&buffer,"PNG");
    buffer.close();
    return baScene;
}


//cv::Mat Tools::convertByteArray2Mat(QByteArray _ByteArray )
//{
//    const char* begin = reinterpret_cast<char*>(_ByteArray.data());
//    const char* end = begin + _ByteArray.size();
//    std::vector<char> pic(begin, end);
//    cv::Mat mat =  cv::imdecode(pic,CV_LOAD_IMAGE_COLOR);
//    return mat.clone();
//}

//QByteArray Tools::resizeByteArray2ByteArray(QByteArray _ByteArray, const int &_w, const int &_h)
//{
//    const char* begin = reinterpret_cast<char*>(_ByteArray.data());
//    const char* end = begin + _ByteArray.size();
//    std::vector<char> pic(begin, end);
//    cv::Mat cvresize =  cv::imdecode(pic,CV_LOAD_IMAGE_COLOR);

//    int max_width = _w;
//    int max_height = _h;

//    if (cvresize.cols > max_width){
//        float aspect = max_width / ((float)cvresize.cols);
//        float y = ((float)cvresize.rows) * aspect;
//        cv::resize(cvresize, cvresize, cv::Size((int) max_width, (int) y));
//       }

//    if (cvresize.rows > max_height){
//        float aspect = max_height / ((float)cvresize.rows);
//        float x = ((float)cvresize.cols) * aspect;
//        cv::resize(cvresize, cvresize, cv::Size((int) x, (int) max_height));
//    }

//    return convertMat2ByteArray(cvresize.clone());
//}
/** END CONVERSORES **********/
/** END TOOLS ************************************************************/



//}

namespace Registros{


Camara::Camara():
    m_tools(new Tools)
{

    OrigenFoto              =cv::Mat::zeros(fotoSize, CV_8UC3 );                 //Imagen Original
    OrigenFotoByte          =m_tools->convertMat2ByteArray(OrigenFoto.clone());
    OrigenFotoQ             =m_tools->convertMat2QImage(OrigenFoto.clone());
}

Camara::~Camara(){
   m_tools->deleteLater();
}

void Camara::convertirFotos()
{
    OrigenFotoByte          =m_tools->convertMat2ByteArray(OrigenFoto.clone());
    OrigenFotoQ             =m_tools->convertMat2QImage(OrigenFoto.clone());
}


MatriculaResults::MatriculaResults()
    :m_tools(new Tools)
{
    tipo                 =0;                                                     //0 para calibracion, 1 para procesado
    id                   =0;                                                     //id fuente de captura de foto
    camara.OrigenFoto    =cv::Mat::zeros(fotoSize, CV_8UC3 );                    //Imagen Original
    camara.OrigenFotoByte=m_tools->convertMat2ByteArray(camara.OrigenFoto.clone());
    camara.OrigenFotoQ   =m_tools->convertMat2QImage(camara.OrigenFoto.clone());

    OrigenFotoPrewarp    =cv::Mat::zeros(fotoSize, CV_8UC3 );                    // Imagen con calibracion prewarp
    OrigenFotoPrewarpQ   =m_tools->convertMat2QImage(OrigenFotoPrewarp.clone());
    OrigenFotoBlanca     =cv::Mat::zeros(fotoSize, CV_8UC3 );                    //  Imagen con calibracion de Blancos
    OrigenFotoBlancaQ    =m_tools->convertMat2QImage(OrigenFotoBlanca.clone());
    OrigenFotoRoja       =cv::Mat::zeros(fotoSize, CV_8UC3 );                    // Imagen con calibracion de Rojos
    OrigenFotoRojaQ      =m_tools->convertMat2QImage(OrigenFotoRoja.clone());

    MatriculaDetectedA   =false;                                                 // Coincide con un patron de busqueda?
    MatriculaA           ="";                                                    // STring de la matricula
    MatriculaFotoA       =cv::Mat::zeros( matriculaSize, CV_8UC3 );              // Imagen recortada de la Matricula
    MatriculaFotoAByte   =m_tools->convertMat2ByteArray(MatriculaFotoA.clone());
    MatriculaFotoAQ      =m_tools->convertMat2QImage(MatriculaFotoA.clone());
    MatriculaPrecisionA  =0;                                                     // Precision del OCR
    MatriculaPrecisionAs ="0%";

    MatriculaDetectedB   =false;                                                 // Coincide con un patron de busqueda?
    MatriculaB           ="";                                                    // STring de la matricula
    MatriculaFotoB       =cv::Mat::zeros( matriculaSize, CV_8UC3 );              // Imagen recortada de la Matricula
    MatriculaFotoBByte   =m_tools->convertMat2ByteArray(MatriculaFotoB.clone());
    MatriculaFotoBQ      =m_tools->convertMat2QImage(MatriculaFotoB.clone());
    MatriculaPrecisionB  =0;                                                     // Precision del OCR
    MatriculaPrecisionBs ="0%";
}

MatriculaResults::~MatriculaResults()
{
    m_tools->deleteLater();
}

void MatriculaResults::convertirFotos()
{
    camara.OrigenFotoByte =  m_tools->convertMat2ByteArray(camara.OrigenFoto.clone());
    camara.OrigenFotoQ    =  m_tools->convertMat2QImage(camara.OrigenFoto.clone());

    OrigenFotoPrewarpQ    =  m_tools->convertMat2QImage(OrigenFotoPrewarp.clone());
    OrigenFotoBlancaQ     =  m_tools->convertMat2QImage(OrigenFotoBlanca.clone());
    OrigenFotoRojaQ       =  m_tools->convertMat2QImage(OrigenFotoRoja.clone());

    MatriculaFotoAByte    =  m_tools->convertMat2ByteArray(MatriculaFotoA.clone());
    MatriculaFotoAQ       =  m_tools->convertMat2QImage(MatriculaFotoA.clone());

    MatriculaFotoBByte    =  m_tools->convertMat2ByteArray(MatriculaFotoB.clone());
    MatriculaFotoBQ       =  m_tools->convertMat2QImage(MatriculaFotoB.clone());

}


}

void SimpleMatriculas::convertirFotos()
{
    for (int i=0; i< results.count(); i++){
    results[i].convertirFotos();
    }
}

}
