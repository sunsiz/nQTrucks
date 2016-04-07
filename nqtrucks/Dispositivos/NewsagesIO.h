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

#ifndef NEWSAGESIO_H
#define NEWSAGESIO_H

#include <QObject>
#include <nqtglobal.h>

#include "Firmata/firmata.h"
#include "Firmata/backends/serialinfo.h"
#include "Firmata/backends/serialport.h"

#include "Firmata/pins/digitalpin.h"

namespace nQTrucks {
namespace Devices {


class NewsagesIO : public Firmata
{
    Q_OBJECT
    Q_PROPERTY(QString IODevice   READ IODevice   WRITE setIODevice   NOTIFY IODeviceChanged)
    Q_PROPERTY(bool    ValuePin10 READ ValuePin10 WRITE setValuePin10 NOTIFY ValuePin10Changed)

public:
    explicit NewsagesIO(QSettings *_appsettings=0,QObject *parent = 0);

    /** PROPIEDADES **/
    QString IODevice()   const{return m_IODevice;}
    bool    ValuePin10() const{return m_ValuePin10;}

signals:
    void IODeviceConnectChanged(const bool &value);
    void IODeviceChanged();
    void ValuePin10Changed(const bool &value);

public slots:
    void setIODevice  (const QString &_IODevice);
    void setValuePin10(const bool &value);
    void setIODeviceConfig();
    void setIODeviceConnect(const bool &value);

private:
    QString m_IODevice;
    bool    m_ValuePin10;
    bool    m_conectado;//Guardar el valor del pin para controlar reset y ultimo estado
    /** FIN PROPIEDADES **/


    /** FTDI **/
    SerialPortList *m_ioPortList;
    SerialFirmata  *m_ioFtdi;
private slots:
    void onIOConectado();
    void onIOReset(int r, int v);
    /** END FTDI **/

    /** SETTINGS **/
private:
    QString   m_configroot;
    void      loadconfig();
    QSettings *m_settings;
    /** END SETTINGS **/

    /**     PINS      **/
    /** RELE SEMAFORO **/
public:
    DigitalPin *m_OutPin10;
    /** END RELE SEMAFORO **/
    /**      END PINS     **/


};

} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/


#endif // NEWSAGESIO_H
