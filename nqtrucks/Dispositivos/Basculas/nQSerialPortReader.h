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

#ifndef NQSERIALPORTREADER_H
#define NQSERIALPORTREADER_H

#include <QtSerialPort/QSerialPort>
#include <QByteArray>
#include <QObject>

#include <nqtglobal.h>

namespace nQTrucks {
namespace Devices {

class nQSerialPortReader : public QObject
{
    Q_OBJECT
    Q_ENUMS(BasculaType)
public:
    explicit nQSerialPortReader(QSettings *_appsettings=0,QObject *parent=nullptr);
    ~nQSerialPortReader();

    enum BasculaType
    {
        NEWSAGES0=0,
        NEWSAGES1
    };

    /** SETTINGS **/
private slots:
    void setBasculaType(const int &_TipoBascula);
    void setBasculaPort(const QString &_IODevice);
    void setBasculaMuestras(const int &_muestras);
private:
    QSettings *m_settings;
    void loadconfig();
    /** END SETTINGS **/

    /** INTERFACE REALTIME **/
private:
    QSerialPort *m_serialPort;
    int          m_type=0;
    QString      m_serialBuffer="";
    int          charInicio=0;
    QByteArray   m_serialData;
    Bascula      m_bascula;
    QString      m_serialPortName;
public slots:
    void connectPort(const bool &_value);
private slots:
    void connectBasculaType(int _type);
    void handleError(QSerialPort::SerialPortError error);  
signals:
    void BasculaChanged(const Bascula &bascula);
    void BasculaStatus(const bool &status);
    /** END INTERFACE REALTIME**/

    /** LECTURA SEGUN TIPOS **/
private slots:
    void ReadType0();
    /** END LECTURA SEGUN TIPOS **/


    /** INTERFACE ESTABILIZADA **/
private:
    int  m_muestras=0;
    int  m_BasculaMuestras=100;
    bool m_inicio_peso=false;
    Bascula    m_bascula_estable;
signals:
    void BasculaPesoNuevo(Bascula _nuevaPesada);
private slots:
    void MuestrearBascula(const Bascula &_bascula);
    /** FIN INTERFACE ESTABILIZADA **/

};

} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/
#endif // NQSERIALPORTREADER_H
