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
#include <QLoggingCategory>


#include<QVector>

#include "nqtglobal.h"
#include "nQTrucksEngine.h"

#include "Dispositivos/CamaraIP.h"
#include "Dispositivos/NewsagesIO.h"
#include <QtSerialPort/QSerialPortInfo>
#include <Dispositivos/Basculas/nQSerialPortReader.h>
#include <Dispositivos/Alpr/NewsagesAlpr.h>

#include <Core/Daemon.h>

/** REPORTS **/
#include <QApplication>
#include <QDir>
#include <QTreeWidgetItem>
#include "LimeReport"

/** Maestros **/
#include "Maestros/Maestros.h"

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
    void       setSettings(QSettings* value);
    QSettings  *settings();
private:
    QSettings* m_settings;
    /** FIN SETTINGS **/

    /** CAMARAS **/
public:
    QStringList getCameraTypes();
    QVector<Devices::CamaraIP*> m_camara;// = new Devices::CamaraIP(0,settings(), this);
    /** FIN CAMARAS **/

    /** NEWSAGES I/O **/
public:
    Devices::NewsagesIO *m_newsagesIO= new Devices::NewsagesIO(settings(),this);
    QStringList getIODevices();
signals:
    /** END NEWSAGES I/O **/

    /** BASCULAS **/
public:
    QStringList getBasculaTypes();
    QStringList getSerialDevices();
private:
    Devices::nQSerialPortReader *m_basculaReader1= new Devices::nQSerialPortReader(settings(),this);
    /** END BASCULAS **/

    /** ALPR **/
    QVector<Devices::NewsagesAlpr*> m_alpr;
    /** END ALPR **/

    /** CORE **/
private:
    Core::Daemon *m_daemon;
    /** END CORE **/

    /** REPORTS **/
private:
    LimeReport::ReportEngine *m_report_editor;
    /** END REPORTS **/

    /** MAESTROS ****************************************************************************/
public:
    Maestros::Maestros *m_maestros;
    /** FIN MAESTROS ************************************************************************/


};


}

#endif // NQTRUCKSENGINE_P_H

