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

#include "nQSerialPortReader.h"
#include <QDebug>

namespace nQTrucks {
namespace Devices {

nQSerialPortReader::nQSerialPortReader(QSerialPort *serialPort, QObject *parent)
    : QObject(parent)
    , m_serialPort(serialPort)
    , m_standardOutput(stdout)
{
    QString serialPortName = "/dev/ttyUSB0";
    int serialPortBaudRate = QSerialPort::Baud9600;

    m_serialPort->setPortName(serialPortName);
    m_serialPort->setBaudRate(serialPortBaudRate);
    m_serialPort->setDataBits(QSerialPort::Data7);
    m_serialPort->setReadBufferSize(36);

    if (!m_serialPort->open(QIODevice::ReadOnly)) {
        m_standardOutput << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(m_serialPort->errorString()) << endl;
    }else{

        connect(m_serialPort, SIGNAL(readyRead()), SLOT(handleReadyRead()));
    }


}

nQSerialPortReader::~nQSerialPortReader()
{

}


void nQSerialPortReader::handleReadyRead()
{
    m_readData.append(m_serialPort->readAll());
    m_standardOutput << m_readData << endl;
}

void nQSerialPortReader::handleTimeout()
{
        m_standardOutput << QObject::tr("Data successfully received from port %1").arg(m_serialPort->portName()) << endl;
        //m_standardOutput << m_readData << endl;
        qDebug() << m_readData;
}

void nQSerialPortReader::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        m_standardOutput << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
    }
}


} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/
