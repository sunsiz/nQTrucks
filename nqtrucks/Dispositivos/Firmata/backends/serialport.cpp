// QFirmata - a Firmata library for QML
//
// Copyright 2016 - Calle Laakkonen
//
// QFirmata is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// QFirmata is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#include "serialport.h"

#include <QPointer>
#include <QSerialPort>
#include <QDebug>

struct SerialFirmata::Private {
    QPointer<QSerialPort> port;
    QString device;
    int baudRate;

    Private()
    : baudRate(57600)
    { }
};

SerialFirmata::SerialFirmata(QObject *parent)
    : FirmataBackend(parent), d(new Private)
{
}

SerialFirmata::~SerialFirmata()
{
    delete d;
}

QString SerialFirmata::device() const
{
    return d->device;
}

void SerialFirmata::setDevice(const QString &device)
{
    if(device != d->device) {
        d->device = device;
        if(device.isEmpty()) {
            d->port = nullptr;
            setStatusText(QStringLiteral("Device not set"));
        /** NEWSAGES
         * CONECT AND DISCONNECT
         **/
        } else {
           if(device=="null"){
              if(d->port->isOpen()){
                 d->port->close();
               }
        }else{

            d->port = new QSerialPort(device);
            d->port->setBaudRate(d->baudRate);
            connect(
                d->port,
                static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
                [this](QSerialPort::SerialPortError e) {
                    QString msg;
                    switch(e) {
                    case QSerialPort::NoError: msg = QStringLiteral("No error"); break;
                    case QSerialPort::DeviceNotFoundError: msg = QStringLiteral("Device not found"); break;
                    case QSerialPort::PermissionError: msg = QStringLiteral("Permission denied"); break;
                    case QSerialPort::OpenError: msg = QStringLiteral("Device already opened"); break;
                    default: msg = QString("Error code %1").arg(e);
                    }

                    setStatusText(msg);
                });

            if(!d->port->open(QIODevice::ReadWrite)) {
                QString msg;
                qWarning() << "Error opening" << device << d->port->error();
                msg = QStringLiteral("Error opening ");
                setStatusText(msg);
            } else {
                connect(d->port, &QSerialPort::readyRead, this, &SerialFirmata::onReadyRead);
                setAvailable(true);
                setStatusText(QStringLiteral("Serial port opened"));
            }
        }
        }

        emit deviceChanged(device);
    }
}

int SerialFirmata::baudRate() const
{
    return d->baudRate;
}

void SerialFirmata::setBaudRate(int baudRate)
{
    if(baudRate != d->baudRate) {
        d->baudRate = baudRate;
        if(d->port) {
            if(!d->port->setBaudRate(baudRate)) {
                qWarning() << "Error setting baud rate" << baudRate << d->port->error();
            }
        }

        emit baudRateChanged(baudRate);
    }
}

void SerialFirmata::writeBuffer(const uint8_t *buffer, int len)
{
    if(!d->port) {
        qWarning() << "Device" << d->device << "not open!";
        return;
    }
    qint64 written = d->port->write((const char*)buffer, len);
    if(written != len) {
        qWarning() << d->device << "error while writing buffer" << d->port->error();
    }
}

void SerialFirmata::onReadyRead()
{
    Q_ASSERT(d->port);
    char buffer[256];
    int len;
    do {
        len = d->port->read(buffer, sizeof(buffer));
        if(len>0)
            bytesRead(buffer, len);
    } while(len>0);
}

