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

#include "NewsagesIO.h"
#include <QDebug>

namespace nQTrucks {
namespace Devices {



NewsagesIO::NewsagesIO(QSettings *_appsettings, QObject *parent)
    : QObject(parent)
{
    ftdi_is_available = false;
    ftdi_port_name = "";
    ftdi = new QSerialPort;

/*
    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
        }
        qDebug() << "Has Product ID: " << serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier()){
            qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
        }
    }

*/
  foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            if(serialPortInfo.vendorIdentifier() == ftdi_vendor_id){
                if(serialPortInfo.productIdentifier() == ftdi_product_id){
                   ftdi_port_name = serialPortInfo.portName();
                    ftdi_is_available = true;
                }
            }
        }
    }

    if(ftdi_is_available){
        // open and configure the serialport
        ftdi->setPortName(ftdi_port_name);
        ftdi->open(QSerialPort::WriteOnly);
        ftdi->setBaudRate(QSerialPort::Baud9600);
        ftdi->setDataBits(QSerialPort::Data8);
        ftdi->setParity(QSerialPort::NoParity);
        ftdi->setStopBits(QSerialPort::OneStop);
        ftdi->setFlowControl(QSerialPort::NoFlowControl);
        sendCommand(QString("w1"));
    }else{
        // give error message if not available
        qDebug() << "No encuentro ningun FTDI!";
    }

    //qDebug() << value;


}

NewsagesIO::~NewsagesIO()
{
    if (ftdi->isOpen()){
        sendCommand(QString("w0"));
        ftdi->close();
    }
}

void NewsagesIO::sendCommand(QString command)
{
    if(ftdi->isWritable()){
        ftdi->write(command.toStdString().c_str());
    }else{
        qDebug() << "error: Puerto serie solo lectura?";
    }
}


} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/
