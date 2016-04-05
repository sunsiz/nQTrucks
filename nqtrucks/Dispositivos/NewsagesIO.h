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

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace nQTrucks {
namespace Devices {


class NewsagesIO : public QObject
{
    Q_OBJECT
public:
    explicit NewsagesIO(QSettings *_appsettings=0,QObject *parent = 0);
    ~NewsagesIO();
signals:

public slots:

    /** FTDI **/
private slots:
    void sendCommand(QString command);
private:
    /* FTDI Arduino MiniPro
     * Has vendor ID:  true
        Vendor ID:  1027
        Has Product ID:  true
        Product ID:  24577
     */
    QSerialPort *ftdi;
    static const quint16 ftdi_vendor_id = 1027;
    static const quint16 ftdi_product_id = 24577;
    QString ftdi_port_name;
    bool ftdi_is_available;
    /** FTDI **/


};

} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/


#endif // NEWSAGESIO_H
