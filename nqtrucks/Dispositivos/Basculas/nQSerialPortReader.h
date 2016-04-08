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
public:
    explicit nQSerialPortReader(QSettings *_appsettings=0,QObject *parent = 0);
    ~nQSerialPortReader();

    enum BasculaType
    {
        NEWSAGES0=0,
        NEWSAGES1
    };

private:
    int m_type;
    QString      m_serialPortName;
    QSerialPort *m_serialPort;
    t_Bascula    m_bascula;
    QByteArray   m_serialData;
    QString      m_serialBuffer;
    int          charInicio=0;


    /** INTERFACE **/
signals:
    void BasculaChanged(const t_Bascula &bascula);
    void BasculaStatus(const bool &status);

public slots:
    void connectPort(const bool &_value);
    /** END INTERFACE **/


    /** SETTINGS **/   
private slots:
    void setBasculaType(const int &_TipoBascula);
    void setBasculaPort(const QString &_IODevice);

private:
    QString   m_configroot;
    QSettings *m_settings;
    void loadconfig();
    /** END SETTINGS **/

private slots:
    void connectBasculaType(int _type);
    void handleError(QSerialPort::SerialPortError error);

    /** LECTURA SEGUN TIPOS **/
private slots:
    void ReadType0();
    /** END LECTURA SEGUN TIPOS **/

};

} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/
#endif // NQSERIALPORTREADER_H
