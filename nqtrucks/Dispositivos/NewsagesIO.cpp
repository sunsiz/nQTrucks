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

static const QString m_IODevice = "/dev/NewsagesIO";

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
    m_OutPinVerde = new DigitalPin(this);
    m_OutPinAmarillo = new DigitalPin(this);
    m_OutPinRojo = new DigitalPin(this);

    m_ValuePinVerde=RELE_ON;
    m_ValuePinAmarillo=RELE_OFF;
    m_ValuePinRojo=RELE_OFF;

    /** PINS **/
    m_OutPinVerde->setPin(2);
    m_OutPinVerde->setOutput(true);
    m_OutPinVerde->setFirmata(this);

    m_OutPinAmarillo->setPin(3);
    m_OutPinAmarillo->setOutput(true);
    m_OutPinAmarillo->setFirmata(this);

    m_OutPinRojo->setPin(4);
    m_OutPinRojo->setOutput(true);
    m_OutPinRojo->setFirmata(this);


    connect(m_OutPinVerde,   &DigitalPin::valueChanged,        this,&NewsagesIO::setValuePinVerde);
    connect(m_OutPinAmarillo,&DigitalPin::valueChanged,        this,&NewsagesIO::setValuePinAmarillo);
    connect(m_OutPinRojo,    &DigitalPin::valueChanged,        this,&NewsagesIO::setValuePinRojo);
    connect(this->m_ioFtdi,  &FirmataBackend::protocolVersion ,this,&NewsagesIO::onIOConectado);

    //setSemaforo(SEMAFORO_VERDE);
}

/** PROPIEDADES *******************************************************/
void NewsagesIO::setValuePinVerde(const bool &value){
    if(m_ValuePinVerde!=value){
        if(m_conectado){
            if(m_ioFtdi->isAvailable()){
                m_ValuePinVerde=value;
                m_OutPinVerde->setValue(m_ValuePinVerde);
                emit ValuePinVerdeChanged(m_OutPinVerde->value());
            }
        }
    }
}

void NewsagesIO::setValuePinAmarillo(const bool &value){
    if(m_ValuePinAmarillo!=value){
        if(m_conectado){
            if(m_ioFtdi->isAvailable()){
                m_ValuePinAmarillo=value;
                m_OutPinAmarillo->setValue(m_ValuePinAmarillo);
                emit ValuePinAmarilloChanged(m_OutPinAmarillo->value());
            }
        }
    }
}
void NewsagesIO::setValuePinRojo(const bool &value){
    if(m_ValuePinRojo!=value){
        if(m_conectado){
            if(m_ioFtdi->isAvailable()){
                m_ValuePinRojo=value;
                m_OutPinRojo->setValue(m_ValuePinRojo);
                emit ValuePinRojoChanged(m_OutPinRojo->value());
            }
        }
    }
}


/** END PROPIEDADES *******************************************************/
/** FTDI ******************************************************************/
void NewsagesIO::onIOConectado(){
    /** PINS **/
    m_OutPinVerde->initialize();
    m_OutPinVerde->setValue(m_ValuePinVerde);
    m_OutPinAmarillo->initialize();
    m_OutPinAmarillo->setValue(m_ValuePinAmarillo);
    m_OutPinRojo->initialize();
    m_OutPinRojo->setValue(m_ValuePinRojo);
    setSemaforo(m_semaforo);
}

void NewsagesIO::setSemaforo(const int &_color){
    if (getSemaforo()!= _color){ setSemaforoEstado(_color); }
}

/** END FTDI *********************************************************************************/

/** SEMAFORO *********************************************************/
//void NewsagesIO::setSemaforoDevice(const QString &_IODevice){
//    if (m_IODevice != _IODevice) {
//        m_IODevice = _IODevice;
//        m_settings->setValue(QString(NEWSAGESIO) + "/device",m_IODevice);
//    }
//}

void NewsagesIO::setSemaforoDeviceConnect(const bool &value){
    setConectado(value);
    if(getConectado()){
        loadconfig();
        m_ioFtdi->setDevice(m_IODevice);
        if(m_ioFtdi->isAvailable()){
            emit ValuePinVerdeChanged(m_OutPinVerde->value());
            emit ValuePinAmarilloChanged(m_OutPinAmarillo->value());
            emit ValuePinRojoChanged(m_OutPinRojo->value());
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
        setValuePinVerde(RELE_ON);
        setValuePinAmarillo(RELE_OFF);
        setValuePinRojo(RELE_OFF);
        qDebug() << "SEMAFORO VERDE";
        break;
    case SEMAFORO_AMARILLO:
        m_semaforo = SEMAFORO_AMARILLO;
        setValuePinVerde(RELE_OFF);
        setValuePinAmarillo(RELE_ON);
        setValuePinRojo(RELE_OFF);
        qDebug() << "SEMAFORO AMARILLO";
        break;
    case SEMAFORO_ROJO:
        m_semaforo = SEMAFORO_ROJO;
        setValuePinVerde(RELE_OFF);
        setValuePinAmarillo(RELE_OFF);
        setValuePinRojo(RELE_ON);
        qDebug() << "SEMAFORO ROJO";
        break;
    default:
        m_semaforo = SEMAFORO_VERDE;
        setValuePinVerde(RELE_ON);
        setValuePinAmarillo(RELE_OFF);
        setValuePinRojo(RELE_OFF);
        qDebug() << "SEMAFORO VERDE";
        break;
    }
    emit SemaforoEstadoChanged(getSemaforo());
}
/** END SEMAFORO *********************************************************/



} /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/
