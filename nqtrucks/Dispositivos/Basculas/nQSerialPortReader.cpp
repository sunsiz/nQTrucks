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
    , m_settings(_appsettings)
{
    m_bascula = new Bascula;
    m_bascula_estable = new Bascula;
    m_serialPort = new QSerialPort(this);
    emit BasculaStatus(false);
    connect(this, &nQSerialPortReader::BasculaChanged,this,&nQSerialPortReader::MuestrearBascula);
}

void nQSerialPortReader::setBasculaType(const int &_TipoBascula){
    if (m_type != _TipoBascula){
        m_type = _TipoBascula;
        m_settings->setValue(QString(BASCULA) + "/tipo",QString::number(m_type));
    }
}

void nQSerialPortReader::setBasculaPort(const QString &_IODevice){
    if (m_serialPortName != _IODevice) {
        m_serialPortName = _IODevice;
        m_settings->setValue(QString(BASCULA) + "/device",m_serialPortName);
    }
}

void nQSerialPortReader::setBasculaMuestras(const int &_muestras){
    if(m_BasculaMuestras != _muestras){
        m_BasculaMuestras = _muestras;
        m_settings->setValue(QString(BASCULA) + "/factor_estable",m_BasculaMuestras);
    }
}

void nQSerialPortReader::setTolerancia_minima(const int &_tolerancia_minima){
    if(m_tolerancia_minima != _tolerancia_minima){
        m_tolerancia_minima = _tolerancia_minima;
        m_settings->setValue(QString(BASCULA) + "/tolerancia_minima",m_tolerancia_minima);
    }
}

int nQSerialPortReader::getTolerancia_minima() const{
    return m_tolerancia_minima;
}

int nQSerialPortReader::reloadTolerancia_minima(){
    setTolerancia_minima(m_settings->value( QString(BASCULA) + "/tolerancia_minima", "10"          ).toInt());
    return getTolerancia_minima();
}


int nQSerialPortReader::getBasculaMuestras() const{
    return m_BasculaMuestras;
}

void nQSerialPortReader::loadconfig(){
    setBasculaPort(m_settings->value(       QString(BASCULA) + "/device",            "/dev/ttyUSB0").toString());
    setBasculaType(m_settings->value(       QString(BASCULA) + "/tipo",              "0"           ).toInt());
    setBasculaMuestras(m_settings->value(   QString(BASCULA) + "/factor_estable",    "1"           ).toInt());
    setTolerancia_minima(m_settings->value( QString(BASCULA) + "/tolerancia_minima", "10"          ).toInt());

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
void nQSerialPortReader::connectBasculaType(int _type){
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


void nQSerialPortReader::handleError(QSerialPort::SerialPortError serialPortError){
    qDebug() << "Error Serie: " << serialPortError;
    if (serialPortError == QSerialPort::ReadError) {
        emit BasculaStatus(false);
    }
}


/** Funciones de lectura segun Tipos **/

void nQSerialPortReader::ReadType0(){             
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
        if(m_serialData != QChar(3)){
            m_serialBuffer += QString::fromStdString(m_serialData.toStdString());
        }else{
            charInicio=0;
            m_bascula->setBEstadoAnterior(m_bascula->getBEstado());
            if(m_serialBuffer.mid(0,1) != "E"){m_bascula->setBEstado(false);
            }else m_bascula->setBEstado(true);

            m_bascula->setIBruto(m_serialBuffer.mid( 4,8).toFloat());
            m_bascula->setITara( m_serialBuffer.mid(14,8).toFloat());
            m_bascula->setINeto( m_serialBuffer.mid(24,8).toFloat());

            //if (m_serialBuffer.mid(24,8).contains("-")){m_bascula->setINeto(fabs(m_bascula->getINeto()));}
            emit BasculaChanged(*m_bascula);
        }
        break;
    default:
        break;
    }
}


void nQSerialPortReader::MuestrearBascula(const Bascula &_bascula){
    if (m_inicio_peso){
        //if((_bascula.getBEstado()) & (_bascula.getIBruto() !=0)){
        if((_bascula.getBEstado()) && (_bascula.getIBruto() >m_BasculaMuestras)){
            m_count_muestras++;
            if(m_count_muestras == getBasculaMuestras()*10){
                m_inicio_peso=false;
                m_bascula_estable->setBascula(_bascula);
                emit BasculaPesoNuevo(*m_bascula_estable);
                setBasculaMuestras(m_settings->value(   QString(BASCULA) + "/factor_estable","1" ).toInt());
                m_count_muestras=0;
            }
        }
    }else{
        if(_bascula.getIBruto() <=m_BasculaMuestras){
            m_inicio_peso=true;
            m_count_muestras=0;
            m_bascula_estable->clear();
        }
    }
}



} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/
