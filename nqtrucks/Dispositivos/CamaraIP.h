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

#ifndef CAMARASIP_H
#define CAMARASIP_H

#include <QObject>
#include <nqtglobal.h>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QImage>
#include <QUrl>

namespace nQTrucks {
namespace Devices {

class CamaraIP : public QObject
{
    Q_OBJECT
    Q_ENUMS(CameraType)

public:
    explicit CamaraIP(int nDevice=0, QSettings *_appsettings=0, QObject *parent = 0);
    //~CamaraIP();

    enum CameraType
    {
        DEBUG=0,
        HIKVISION,
        CAMTRONIC
    };

private:
    CameraType  m_TipoCamara = CameraType::HIKVISION;
    QString     m_CamaraHost;
    QString     m_CamaraPort;
    QString     m_CamaraUser;
    QString     m_CamaraPass;
    /** FIN PROPIEDADES **/

    /** SETTINGS **/
private:
    QString   m_configroot;
    int       m_nDevice;
    void      loadconfig();
    QSettings *m_settings;
    /** END SETTINGS **/

    /** REDES **/
private:
    QUrl setCamaraURL();
    QNetworkAccessManager *m_netmanager;
    Registros::Camara     *m_RegistroCamara;
public slots:
    void sendCamaraIPFotoRequest();

private slots:
    void camaraNetworkReplyFinished(QNetworkReply *reply);

signals:
    void ReplyCamaraIP(const Registros::Camara &_ReplyCamara);
  //  void ReplyCamaraIPFotoCV(const cv::Mat &_ReplyCV);
  //  void ReplyCamaraIPFoto(const QByteArray &_Reply);

    void CamaraIPWeb(const QString &CamaraWeb);
    void CamaraError(const QString &errorCamaraIP);
private:
    QString m_errorCamaraIP;
    /** END REDES **/
};

} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/

#endif // NQCAMARAS_H
