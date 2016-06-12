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

#include "CamaraIP.h"
#include <QNetworkRequest>
#include <QBuffer>


namespace nQTrucks{
namespace Devices {

CamaraIP::CamaraIP(int nDevice, QSettings *_appsettings, QObject *parent)
    : QObject(parent)
    , m_nDevice(nDevice)
    , m_settings(_appsettings)
    , m_netmanager(new QNetworkAccessManager(this))
{

    /** CONECTIONS **/
    connect(m_netmanager, SIGNAL(finished(QNetworkReply*)), this , SLOT(camaraNetworkReplyFinished(QNetworkReply*)));

    /** CONFIG **/
    loadconfig();

}

CamaraIP::~CamaraIP()
{

}

void CamaraIP::loadconfig()
{
    int tipo=0;
    switch (m_nDevice) {
    case 0:
        m_CamaraHost = m_settings->value(QString(CAMARA1) + "/host","10.42.0.251").toString();
        m_CamaraPort = m_settings->value(QString(CAMARA1) + "/port","80").toString();
        m_CamaraUser = m_settings->value(QString(CAMARA1) + "/user","nqtrucks").toString();
        m_CamaraPass = m_settings->value(QString(CAMARA1) + "/pass","nqtrucks2016").toString();
        tipo = QString(m_settings->value(QString(CAMARA1) + "/tipo","0").toString()).toInt();
        m_TipoCamara  = static_cast<CameraType>(tipo);
        break;
    case 1:
        m_CamaraHost = m_settings->value(QString(CAMARA2) + "/host","10.42.0.251").toString();
        m_CamaraPort = m_settings->value(QString(CAMARA2) + "/port","80").toString();
        m_CamaraUser = m_settings->value(QString(CAMARA2) + "/user","nqtrucks").toString();
        m_CamaraPass = m_settings->value(QString(CAMARA2) + "/pass","nqtrucks2016").toString();
        tipo = QString(m_settings->value(QString(CAMARA2) + "/tipo","0").toString()).toInt();
        m_TipoCamara  = static_cast<CameraType>(tipo);
        break;
    }

    m_errorCamaraIP = " Camara numero: "+
                      QString::number(m_nDevice+1)+
                      " Host: "+
                      m_CamaraHost;

}

QUrl CamaraIP::setCamaraURL()
{
    loadconfig();
    QUrl curl("http://");
    curl.setHost(m_CamaraHost);
    curl.setPort(m_CamaraPort.toInt());
    curl.setUserName(m_CamaraUser);
    curl.setPassword(m_CamaraPass);

    int jpgnumber = m_nDevice +1;

    switch (m_TipoCamara) {
    case CameraType::DEBUG:
        curl= QString("http://localhost/" + QString::number(jpgnumber)+ ".jpg");
        qDebug() << "Camara: " << m_nDevice << " url: " << curl.toString();
        break;
    case CameraType::HIKVISION :
        curl.setPath("/Streaming/channels/101/picture");
        break;
    case CameraType::CAMTRONIC:
        /** TODO Implemetar mas tipos **/
        break;
    default:
        break;
    }
    emit CamaraIPWeb(curl.toString());
    return curl;
}

void CamaraIP::sendCamaraIPFotoRequest()
{
    /**  Peticion ***/
    QUrl Servidor = setCamaraURL();
    QNetworkRequest request(Servidor);


    /** TODO Stuff SSL (por probar) **/
    //QSslConfiguration config = request.sslConfiguration();
    //config.setPeerVerifyMode(QSslSocket::VerifyNone);
    //QSsl::TlsV1SslV3;
    //config.setProtocol(QSsl::TlsV1SslV3);
    //request.setSslConfiguration(config);

    m_netmanager->get(request);
}

void CamaraIP::camaraNetworkReplyFinished(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();    
    QImage temp;
    //m_RegistroCamara = new Registros::Camara;
    m_RegistroCamara.OrigenFoto = cv::Mat::zeros( fotoSize, CV_8UC3 );
    m_RegistroCamara.OrigenFoto = cv::Scalar( 0, 0, 255 );

    if (temp.loadFromData(data)){

        QBuffer buffer(&data);
        buffer.open(QIODevice::ReadOnly);
        const char* begin = reinterpret_cast<char*>(data.data());
        const char* end = begin + data.size();
        std::vector<char> pic(begin, end);
        buffer.close();

        m_RegistroCamara.OrigenFotoByte=data;
        cv::imdecode(pic,CV_LOAD_IMAGE_COLOR,&m_RegistroCamara.OrigenFoto);       
        cv::resize(m_RegistroCamara.OrigenFoto,m_RegistroCamara.OrigenFoto,fotoSize);


    }else{
        /** CAMARA ERROR ByteArray **/
        temp = QImage(fotoWidth, fotoHeight, QImage::Format_RGB32);
        temp.fill(Qt::red);

        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        temp.save(&buffer, "PNG");
        buffer.close();
        //buffer.reset();

        m_RegistroCamara.OrigenFotoByte=ba;

        /** CAMARA ERROR CV::MAT **/

        emit CamaraError(m_errorCamaraIP);
        //fotoCamaraErrorCV.release();
    }

    emit ReplyCamaraIPFotoCV(m_RegistroCamara.OrigenFoto);
    emit ReplyCamaraIPFoto(m_RegistroCamara.OrigenFotoByte);
    emit ReplyCamaraIP(m_RegistroCamara);

    data.clear();
    temp.detach();
    //delete m_RegistroCamara;
    reply->deleteLater();

}


} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/
