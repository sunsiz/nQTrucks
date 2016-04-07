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
#ifndef NQTRUCKSENGINE_P_H
#define NQTRUCKSENGINE_P_H

#include <QObject>
#include <QSharedPointer>
#include <QMetaEnum>
#include <QCoreApplication>
#include <QDir>

#include "nqtglobal.h"
#include "nQTrucksEngine.h"

#include "Dispositivos/CamaraIP.h"

#include "Dispositivos/NewsagesIO.h"
#include <QtSerialPort/QSerialPortInfo>

namespace nQTrucks {

class nQTrucksEnginePrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(nQTrucksEngine)
    Q_ENUMS(CameraType)

public:
    explicit nQTrucksEnginePrivate(QObject *parent=0);
    virtual ~nQTrucksEnginePrivate();
    nQTrucksEngine * q_ptr;

    /** SETTINGS **/
    void    setSettings(QSettings* value);
    QSettings*  settings();
private:
    QSettings* m_settings;
    /** FIN SETTINGS **/

    /** CAMARAS **/
public:
    QStringList getCameraTypes();
    void setCamaraIP(int nDevice, QString type, QString host, QString port, QString user, QString passwd);
    Devices::CamaraIP *m_camara1 = new Devices::CamaraIP(1,settings(), this);
    Devices::CamaraIP *m_camara2 = new Devices::CamaraIP(2,settings(), this);
    /** FIN CAMARAS **/

    /** NEWSAGES I/O **/
public:
    Devices::NewsagesIO *m_newsagesIO= new Devices::NewsagesIO(settings(),this);
    QStringList getIODevices();
signals:
    /** END NEWSAGES I/O **/

};


}

#endif // NQTRUCKSENGINE_P_H

