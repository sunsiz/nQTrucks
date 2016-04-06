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

//#include <QtSerialPort/QSerialPort>
//#include <QtSerialPort/QSerialPortInfo>

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

    /** FTDI **/
private:
    /* FTDI Arduino MiniPro
     *  Has vendor ID:  true
        Vendor ID:  1027
        Has Product ID:  true
        Product ID:  24577
     */
    SerialPortList *m_ioPortList;
    SerialFirmata  *m_ioFtdi;
    /** END FTDI **/

    /** PINS **/
public:
    DigitalPin *m_ioPin10;
private:
    bool m_ioPin10Value; //Guardar el valor del para controlar reset y ultimo estado
public slots:
    void getIO10(bool value);
    void conectado();
    void disponible(int r, int v);
    /** END PINS **/


};

} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/


#endif // NEWSAGESIO_H
