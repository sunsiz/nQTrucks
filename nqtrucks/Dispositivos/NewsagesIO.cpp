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
    , m_settings(_appsettings)
{

    /** FTDI **/
    m_ioPortList= new SerialPortList(this);
    m_ioFtdi = new SerialFirmata(this);
    this->setBackend(m_ioFtdi);
    this->setInitPins(false);
    m_conectado=false;
    m_semaforo=0;

    /** RELE */
    m_OutPin10 = new DigitalPin(this);
    m_OutPin11 = new DigitalPin(this);
    m_ValuePin10=false;
    m_ValuePin11=false;

    /** PINS **/
    m_OutPin10->setPin(10);
    m_OutPin10->setOutput(true);
    m_OutPin10->setFirmata(this);

    m_OutPin11->setPin(11);
    m_OutPin11->setOutput(true);
    m_OutPin11->setFirmata(this);

    connect(m_OutPin10,SIGNAL(valueChanged(bool)),this,SLOT(setValuePin10(bool)));
    connect(m_OutPin11,SIGNAL(valueChanged(bool)),this,SLOT(setValuePin11(bool)));
    connect(this->m_ioFtdi, &FirmataBackend::protocolVersion ,this, &NewsagesIO::onIOConectado);

    //setSemaforo(SEMAFORO_VERDE);
}

/** PROPIEDADES *******************************************************/
void NewsagesIO::setValuePin10(const bool &value){
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

void NewsagesIO::setValuePin11(const bool &value){
    if(m_ValuePin11!=value){
        if(m_conectado){
            if(m_ioFtdi->isAvailable()){
                m_ValuePin11=value;
                m_OutPin11->setValue(m_ValuePin11);
                emit ValuePin13Changed(m_OutPin11->value());
            }
        }
    }
}
/** END PROPIEDADES *******************************************************/
/** FTDI ******************************************************************/
void NewsagesIO::onIOConectado(){
    /** PINS **/
    m_OutPin10->initialize();
    m_OutPin10->setValue(m_ValuePin10);
    m_OutPin11->initialize();
    m_OutPin11->setValue(m_ValuePin11);
    int last_status = (int)m_ValuePin10 + (int)m_ValuePin11;
    setSemaforo(last_status);
}

void NewsagesIO::setSemaforo(const int &_color){
    if (getSemaforo()!= _color){
    setSemaforoEstado(_color);
    }

}

/** END FTDI *********************************************************************************/

/** SEMAFORO *********************************************************/
void NewsagesIO::setSemaforoDevice(const QString &_IODevice){
    if (m_IODevice != _IODevice) {
        m_IODevice = _IODevice;
        m_settings->setValue(QString(NEWSAGESIO) + "/device",m_IODevice);
    }
}

void NewsagesIO::setSemaforoDeviceConnect(const bool &value){
    setConectado(value);
    if(getConectado()){
        loadconfig();
        m_ioFtdi->setDevice(m_IODevice);
        if(m_ioFtdi->isAvailable()){
            emit ValuePin10Changed(m_OutPin10->value());
            emit ValuePin13Changed(m_OutPin11->value());
        }else{
            setConectado(false);
            // SIN SEMAFOROOOO
            QMessageBox::information(nullptr,"Info",statusText() + ":" + m_IODevice);
        }
    }else{
        this->m_ioFtdi->setDevice("null");
    }
    emit SemaforoConnectChanged(getConectado());
}

void NewsagesIO::setSemaforoEstado(const int &_color)
{
    switch (_color) {
    case SEMAFORO_VERDE:
        m_semaforo = SEMAFORO_VERDE;
        setValuePin10(false);
        setValuePin11(false);
        break;
    case SEMAFORO_AMARILLO:
        m_semaforo = SEMAFORO_AMARILLO;
        setValuePin10(true);
        setValuePin11(false);
        break;
    case SEMAFORO_ROJO:
        m_semaforo = SEMAFORO_ROJO;
        setValuePin10(true);
        setValuePin11(true);
        break;
    default:
        m_semaforo = SEMAFORO_VERDE;
        setValuePin10(false);
        setValuePin11(false);
        break;
    }
    emit SemaforoEstadoChanged(getSemaforo());
}
/** END SEMAFORO *********************************************************/



} /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/
