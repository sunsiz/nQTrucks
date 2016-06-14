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

public:
    explicit NewsagesIO(QSettings *_appsettings=0,QObject *parent = 0);

    /** PROPIEDADES *******************************************************/
public:
    enum EstadoSemaforo{
        verde       =   SEMAFORO_VERDE,
        amarillo    =   SEMAFORO_AMARILLO,
        rojo        =   SEMAFORO_ROJO,
    };

signals:
    void ValuePin2Changed(const bool &value);
    void ValuePin3Changed(const bool &value);
    void ValuePin4Changed(const bool &value);


private slots:
    void setValuePin2(const bool &value);
    void setValuePin3(const bool &value);
    void setValuePin4(const bool &value);

private:
    QString m_IODevice;

    bool    m_ValuePin2;
    DigitalPin *m_OutPin2;

    bool    m_ValuePin3;
    DigitalPin *m_OutPin3;

    bool    m_ValuePin4;
    DigitalPin *m_OutPin4;

    bool    m_conectado;

public:
    bool getConectado() const {return m_conectado;}
    void setConectado(bool conectado) {m_conectado = conectado;}
    /** FIN PROPIEDADES **************************************************************/


    /** FTDI *************************************************************************/
private:
    SerialPortList *m_ioPortList;
    SerialFirmata  *m_ioFtdi;
private slots:
    void onIOConectado();
    void onIOReset(int r, int v) {Q_UNUSED(r); Q_UNUSED(v); onIOConectado();}


    /** END FTDI ********************************************************************/

    /** SETTINGS ********************************************************************/
private:
    void      loadconfig() {setSemaforoDevice(m_settings->value(QString(NEWSAGESIO) + "/device").toString());}
    QSettings *m_settings;
    /** END SETTINGS ****************************************************************/


    /** SEMAFORO **/
public:
    int  getSemaforo() const {return m_semaforo;}
    void setSemaforo(const int &_color);
private:
    int     m_semaforo;
private slots:
    void setSemaforoDevice  (const QString &_IODevice);
public slots:
    void setSemaforoDeviceConnect(const bool &value);
    void setSemaforoEstado(const int &_color);
signals:
    void SemaforoConnectChanged(const bool &value);
    void SemaforoChanged();
    void SemaforoEstadoChanged(const int &_color);
    /** END SEMAFORO **/

};

} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/


#endif // NEWSAGESIO_H
