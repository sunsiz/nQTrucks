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
    m_ValuePin10=false;
    m_ValuePin13=false;
    m_conectado=false;
    m_ioPortList= new SerialPortList(this);
    m_ioFtdi = new SerialFirmata(this);
    m_OutPin10 = new DigitalPin(this);
    m_OutPin13 = new DigitalPin(this);
    m_semaforo = SEMAFORO_VERDE;

    /** FIRMATA **/
    this->setBackend(m_ioFtdi);
    this->setInitPins(false);

    /** PINS **/
    m_OutPin10->setPin(10);
    m_OutPin10->setOutput(true);
    m_OutPin10->setFirmata(this);
    m_OutPin13->setPin(13);
    m_OutPin13->setOutput(true);
    m_OutPin13->setFirmata(this);

    connect(m_OutPin10,SIGNAL(valueChanged(bool)),this,SLOT(setValuePin10(bool)));
    connect(m_OutPin13,SIGNAL(valueChanged(bool)),this,SLOT(setValuePin13(bool)));
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

void NewsagesIO::setEstadoSemaforo(const int &_color)
{
    if(semaforo()!=_color){
        setSemaforo(_color);
    }
}

int NewsagesIO::semaforo() const
{
    return m_semaforo;
}

void NewsagesIO::setSemaforo(int semaforo)
{
    switch (semaforo) {
    case SEMAFORO_VERDE:
        m_semaforo = SEMAFORO_VERDE;
        setValuePin10(false);
        setValuePin13(false);
        break;
    case SEMAFORO_AMARILLO:
        m_semaforo = SEMAFORO_AMARILLO;
        setValuePin10(true);
        setValuePin13(false);
        break;
    case SEMAFORO_ROJO:
        m_semaforo = SEMAFORO_ROJO;
        setValuePin10(true);
        setValuePin13(true);
        break;
    default:
        m_semaforo = SEMAFORO_VERDE;
        setValuePin10(false);
        setValuePin13(false);
        break;
    }
    emit EstadoSemaforoChanged(m_semaforo);
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
    m_OutPin13->initialize();
    m_OutPin13->setValue(m_ValuePin13);
    /**
    emit ValuePin10Changed(m_OutPin10->value());
    emit ValuePin13Changed(m_OutPin13->value());
    **/
    int last_status = (int)m_ValuePin10 + (int)m_ValuePin10;
    setSemaforo(last_status);

}

void NewsagesIO::setValuePin10(const bool &value)
{
    //qDebug() << "valor: " << value;

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

void NewsagesIO::setValuePin13(const bool &value)
{
    //qDebug() << "valor: " << value;

    if(m_ValuePin13!=value){
        if(m_conectado){
            if(m_ioFtdi->isAvailable()){
                m_ValuePin13=value;
                m_OutPin13->setValue(m_ValuePin13);
                emit ValuePin13Changed(m_OutPin13->value());
            }
        }
    }
}


} /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/
