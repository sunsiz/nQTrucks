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
#include <QMessageBox>

namespace nQTrucks {
namespace Devices {

//bool NewsagesIO::m_ioPin10Value=true;

NewsagesIO::NewsagesIO(QSettings *_appsettings, QObject *parent)
    : Firmata(parent)
    , m_ValuePin10(false)
    , m_conectado(false)
    , m_ioPortList(new SerialPortList)
    , m_ioFtdi(new SerialFirmata)
    , m_settings(_appsettings)
    , m_OutPin10(new DigitalPin)
{
    /** FIRMATA **/
    this->setBackend(m_ioFtdi);
    this->setInitPins(false);

    /** PINS **/
    m_OutPin10->setPin(13);
    m_OutPin10->setOutput(true);
    m_OutPin10->setFirmata(this);

    connect(m_OutPin10,SIGNAL(valueChanged(bool)),this,SLOT(setValuePin10(bool)));
    connect(this->m_ioFtdi, &FirmataBackend::protocolVersion ,this, &NewsagesIO::onIOConectado);
}

void NewsagesIO::setIODeviceConnect(const bool &value){
    m_conectado=value;
    if(m_conectado){
        loadconfig();
        m_ioFtdi->setDevice(m_IODevice);
        if(m_ioFtdi->isAvailable()){
            emit ValuePin10Changed(m_OutPin10->value());
        }else{
            m_conectado=false;
            // SIN SEMAFOROOOO
            QMessageBox::information(nullptr,"Info",statusText() + ":" + m_IODevice);
        }
    }else{
        this->m_ioFtdi->setDevice("null");
    }

    emit IODeviceConnectChanged(m_conectado);
}


/** SETTINGS **/
void NewsagesIO::setIODevice(const QString &_IODevice){
    if (m_IODevice != _IODevice) {
        m_IODevice = _IODevice;
        m_settings->setValue("device",m_IODevice);
    }
}

void NewsagesIO::loadconfig(){
    m_configroot = (QString(NEWSAGESIO));
    m_settings->beginGroup(m_configroot);
    setIODevice(m_settings->value("device").toString());
    m_settings->endGroup();
}

void NewsagesIO::setIODeviceConfig(){
    loadconfig();
}

/** END SETTINGS **/

void NewsagesIO::onIOReset(int v, int r)
{
    Q_UNUSED(v);
    Q_UNUSED(r);
    onIOConectado();
}

void NewsagesIO::onIOConectado()
{
    /** PINS **/
    m_OutPin10->initialize();
    m_OutPin10->setValue(m_ValuePin10);
    emit ValuePin10Changed(m_OutPin10->value());

}

void NewsagesIO::setValuePin10(const bool &value)
{
    qDebug() << "valor: " << value;

    if(m_ValuePin10!=value){
        if(m_conectado){
            if(m_ioFtdi->isAvailable()){
                m_ValuePin10=value;
                m_OutPin10->setValue(m_ValuePin10);
                emit ValuePin10Changed(m_OutPin10->value());
            }
        }
    }
}

} /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/


/*
void NewsagesIO::getIOPorts(){
    m_ioPortList->refresh();
    //emit ioPortsChanged(m_portlist);
    QStringList _ioPortList;

    if (m_ioPortList->rowCount() > 0)
      {
         for (int row = 0; row < m_ioPortList->rowCount(); row++)
         {
           QModelIndex idx = (m_ioPortList->index(row, 0));
           qDebug() << m_ioPortList->data(idx,SerialPortList::SerialPortRoles::NameRole).toString();
         }
      }



    //emit ioPortsChanged((QAbstractListModel)m_ioPortList);
}
*/
/*

    ftdi_is_available = false;
    ftdi_port_name = "";
    ftdi = new QSerialPort;

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
    */




