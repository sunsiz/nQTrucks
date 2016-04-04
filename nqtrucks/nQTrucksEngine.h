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
#ifndef NQTRUCKSENGINE_H
#define NQTRUCKSENGINE_H

#include <QObject>
#include <QSettings>
#include <nqtglobal.h>

namespace nQTrucks {
class nQTrucksEnginePrivate;

class NQTRUCKSLIBSHARED_EXPORT nQTrucksEngine : public QObject
{
    Q_OBJECT
public:
    explicit nQTrucksEngine(QObject *parent=0);
    ~nQTrucksEngine();

    /** CONFIG **/
public:
    void setAppConfig(QSettings *value){m_config->setSettings(value);}
    QSettings *appConfig(){return m_config->settings();}
private:
    nQTrucksConfig *m_config;
    /** END CONFIG **/

    /** Camaras **/
public:
    QStringList getTiposCamaras();
    void setCamaraIP(int nCamara, QString host, QString port, QString user, QString passwd,int type);
    void getCamaraFoto(int _ncamara);
signals:
    void CamaraIPFoto1(const QImage &foto);
    void CamaraIPWeb1(const QString &url);
    void CamaraIPFoto2(const QImage &foto);
    void CamaraIPWeb2(const QString &url);
    /** END Camaras **/

    /** nQTrucksEnginePrivate **/
protected:
    nQTrucksEnginePrivate * const d_ptr;
    nQTrucksEngine(nQTrucksEnginePrivate &dd, QObject * parent=0);
private:
    Q_DECLARE_PRIVATE(nQTrucksEngine)
    /** FIN nQTrucksEnginePrivate **/
};

}
#endif // NQTRUCKSENGINE_H
