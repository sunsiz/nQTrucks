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
    Q_PROPERTY(CameraType TipoCamara READ TipoCamara WRITE setTipoCamara NOTIFY TipoCamaraChanged)
    Q_PROPERTY(QString    CamaraHost READ CamaraHost WRITE setCamaraHost NOTIFY CamaraHostChanged)
    Q_PROPERTY(QString    CamaraPort READ CamaraPort WRITE setCamaraPort NOTIFY CamaraPortChanged)
    Q_PROPERTY(QString    CamaraUser READ CamaraUser WRITE setCamaraUser NOTIFY CamaraUserChanged)
    Q_PROPERTY(QString    CamaraPass READ CamaraPass WRITE setCamaraPass NOTIFY CamaraPassChanged)


public:
    explicit CamaraIP(int nDevice=0, QSettings *_appsettings=0, QObject *parent = 0);

    enum CameraType
    {
        HIKVISION=0,
        CAMTRONIC
    };
    CameraType TipoCamara() const{return m_TipoCamara;}
    QString    CamaraHost() const{return m_CamaraHost;}
    QString    CamaraPort() const{return m_CamaraPort;}
    QString    CamaraUser() const{return m_CamaraUser;}
    QString    CamaraPass() const{return m_CamaraPass;}

public slots:
    void setTipoCamara(const CameraType &_TipoCamara);
    void setCamaraHost(const QString    &_CamaraHost);
    void setCamaraPort(const QString    &_CamaraPort);
    void setCamaraUser(const QString    &_CamaraUser);
    void setCamaraPass(const QString    &_CamaraPass);

signals:
    void TipoCamaraChanged();
    void CamaraHostChanged();
    void CamaraPortChanged();
    void CamaraUserChanged();
    void CamaraPassChanged();

private:
    CameraType  m_TipoCamara = CameraType::HIKVISION;
    QString     m_CamaraHost;
    QString     m_CamaraPort;
    QString     m_CamaraUser;
    QString     m_CamaraPass;

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
    QImage fotoCamaraError;

public slots:
    void sendCamaraIPFotoRequest();

private slots:
    void camaraNetworkReplyFinished(QNetworkReply *reply);

signals:
    void ReplyCamaraIPFoto(const QImage &Reply);
    void CamaraIPWeb(const QString &CamaraWeb);

    /** END REDES **/
};

}
}
#endif // NQCAMARAS_H
