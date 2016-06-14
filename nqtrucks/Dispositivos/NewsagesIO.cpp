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
    m_OutPin2 = new DigitalPin(this);
    m_OutPin3 = new DigitalPin(this);
    m_OutPin4 = new DigitalPin(this);

    m_ValuePin2=true;
    m_ValuePin3=false;
    m_ValuePin4=false;

    /** PINS **/
    m_OutPin2->setPin(2);
    m_OutPin2->setOutput(true);
    m_OutPin2->setFirmata(this);

    m_OutPin3->setPin(3);
    m_OutPin3->setOutput(true);
    m_OutPin3->setFirmata(this);

    m_OutPin4->setPin(4);
    m_OutPin4->setOutput(true);
    m_OutPin4->setFirmata(this);


    connect(m_OutPin2,SIGNAL(valueChanged(bool)),this,SLOT(setValuePin2(bool)));
    connect(m_OutPin3,SIGNAL(valueChanged(bool)),this,SLOT(setValuePin3(bool)));
    connect(m_OutPin4,SIGNAL(valueChanged(bool)),this,SLOT(setValuePin4(bool)));
    connect(this->m_ioFtdi, &FirmataBackend::protocolVersion ,this, &NewsagesIO::onIOConectado);

    //setSemaforo(SEMAFORO_VERDE);
}

/** PROPIEDADES *******************************************************/
void NewsagesIO::setValuePin2(const bool &value){
    if(m_ValuePin2!=value){
        if(m_conectado){
            if(m_ioFtdi->isAvailable()){
                m_ValuePin2=value;
                m_OutPin2->setValue(m_ValuePin2);
                emit ValuePin2Changed(m_OutPin2->value());
            }
        }
    }
}

void NewsagesIO::setValuePin3(const bool &value){
    if(m_ValuePin3!=value){
        if(m_conectado){
            if(m_ioFtdi->isAvailable()){
                m_ValuePin3=value;
                m_OutPin3->setValue(m_ValuePin3);
                emit ValuePin3Changed(m_OutPin3->value());
            }
        }
    }
}
void NewsagesIO::setValuePin4(const bool &value){
    if(m_ValuePin4!=value){
        if(m_conectado){
            if(m_ioFtdi->isAvailable()){
                m_ValuePin4=value;
                m_OutPin4->setValue(m_ValuePin4);
                emit ValuePin3Changed(m_OutPin4->value());
            }
        }
    }
}


/** END PROPIEDADES *******************************************************/
/** FTDI ******************************************************************/
void NewsagesIO::onIOConectado(){
    /** PINS **/
    m_OutPin2->initialize();
    m_OutPin2->setValue(m_ValuePin2);
    m_OutPin3->initialize();
    m_OutPin3->setValue(m_ValuePin3);   
    m_OutPin4->initialize();
    m_OutPin4->setValue(m_ValuePin3);
    setSemaforo(m_semaforo);
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
            emit ValuePin2Changed(m_OutPin2->value());
            emit ValuePin3Changed(m_OutPin3->value());
            emit ValuePin4Changed(m_OutPin4->value());
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
        setValuePin2(true);
        setValuePin3(false);
        setValuePin4(false);
        break;
    case SEMAFORO_AMARILLO:
        m_semaforo = SEMAFORO_AMARILLO;
        setValuePin2(false);
        setValuePin3(true);
        setValuePin4(false);
        break;
    case SEMAFORO_ROJO:
        m_semaforo = SEMAFORO_ROJO;
        setValuePin2(false);
        setValuePin3(false);
        setValuePin4(true);
        break;
    default:
        m_semaforo = SEMAFORO_VERDE;
        setValuePin2(true);
        setValuePin3(false);
        setValuePin4(false);
        break;
    }
    emit SemaforoEstadoChanged(getSemaforo());
}
/** END SEMAFORO *********************************************************/



} /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/
