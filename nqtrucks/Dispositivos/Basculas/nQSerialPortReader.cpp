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

nQSerialPortReader::nQSerialPortReader(QSettings *_appsettings, QObject *parent)
    : QObject(parent)
    , m_serialPort(new QSerialPort(this))
    , m_serialBuffer("")
    , m_settings(_appsettings)
{
    qRegisterMetaType<t_Bascula>("t_Bascula");
    emit BasculaStatus(false);
    //connectPort();
}

nQSerialPortReader::~nQSerialPortReader()
{
    if(m_serialPort->isOpen()){
        m_serialPort->close();
    }
}

void nQSerialPortReader::setBasculaType(const int &_TipoBascula)
{
    if (m_type != _TipoBascula){
        m_type = _TipoBascula;
        m_settings->setValue("tipo",QString::number(m_type));
        m_settings->sync();
    }
}

void nQSerialPortReader::setBasculaPort(const QString &_IODevice)
{
    if (m_serialPortName != _IODevice) {
        m_serialPortName = _IODevice;
        m_settings->setValue("device",m_serialPortName);
        m_settings->sync();
    }
}

void nQSerialPortReader::loadconfig(){
    m_configroot = (QString(BASCULA));
    m_settings->beginGroup(m_configroot);
    setBasculaPort(m_settings->value("device","/dev/ttyUSB0").toString());
    setBasculaType(m_settings->value("tipo","0").toInt());
    m_settings->endGroup();
    m_settings->sync();
}
void nQSerialPortReader::connectPort(const bool &_value){
    if(m_serialPort->isOpen()){
        m_serialPort->close();
        emit BasculaStatus(false);
    }
    if(_value){
        loadconfig();
        m_serialPort->setPortName(m_serialPortName);
        connectBasculaType(m_type);
    }


}


/** LOGICA **/
void nQSerialPortReader::connectBasculaType(int _type)
{
    switch (_type) {
    case BasculaType::NEWSAGES0:
        m_serialPort->setBaudRate(QSerialPort::Baud9600);
        m_serialPort->setDataBits(QSerialPort::Data7);
        if (m_serialPort->open(QIODevice::ReadOnly)) {
            emit BasculaStatus(true);
            connect(m_serialPort, SIGNAL(readyRead()), SLOT(ReadType0()));
        }
        break;
    default:
        break;
    }
}


void nQSerialPortReader::handleError(QSerialPort::SerialPortError serialPortError)
{
    qDebug() << "Error Serie: " << serialPortError;
    if (serialPortError == QSerialPort::ReadError) {
        emit BasculaStatus(false);
    }
}


/** Funciones de lectura segun Tipos **/

void nQSerialPortReader::ReadType0()
{
    m_serialData = m_serialPort->read(1);
    switch (charInicio) {

    case 0:
        m_serialBuffer="";
        if( m_serialData == QChar(2)) {
            emit BasculaStatus(true);
            charInicio=1;
        }
        break;
    case 1:
        if(m_serialData != QChar(3)){m_serialBuffer += QString::fromStdString(m_serialData.toStdString());
        }else{
            charInicio=0;
            m_bascula.bEstadoAnterior = m_bascula.bEstado;
            if(m_serialBuffer.mid(1,1) != "E"){m_bascula.bEstado= false;
            }else m_bascula.bEstado=true;

            m_bascula.iBruto = m_serialBuffer.mid(4,8).toFloat();
            m_bascula.iTara = m_serialBuffer.mid(14,8).toFloat();
            m_bascula.iNeto = m_serialBuffer.mid(24,8).toFloat();

            if (m_serialBuffer.mid(24,8).contains("-")){m_bascula.iNeto = -m_bascula.iNeto;}
            emit BasculaChanged(m_bascula);
        }
        break;
    default:
        break;
    }
}



} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/
