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
/*
#include <QPrinter>
#include <QPrintDialog>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>

#include "time.h"
*/

#include "nQTrucksEngine_p.h"
#include "nQTrucksEngine.h"

namespace nQTrucks{

//nQTrucksConfig *nQTrucksEngine::m_config =0;
QSettings* nQTrucksEngine::m_settings = 0;

    /** Private Share Pointer Impl **/
nQTrucksEnginePrivate::nQTrucksEnginePrivate(QObject *parent)
    : QObject(parent)
    , m_settings(0)
{
}

nQTrucksEnginePrivate::~nQTrucksEnginePrivate()
{

}

QStringList nQTrucksEnginePrivate::getCameraTypes()
{
    QMetaObject MetaObject = Devices::CamaraIP::staticMetaObject;
    QMetaEnum MetaEnum = MetaObject.enumerator(MetaObject.indexOfEnumerator("CameraType"));
    QStringList listCameraTypes;
    for (int i = 0; i < MetaEnum.keyCount(); i++)
    {
        listCameraTypes.insert(MetaEnum.value(i), MetaEnum.key(i));
    }
    return listCameraTypes;
}


void nQTrucksEnginePrivate::setCamaraIP(int nDevice,  QString type, QString host, QString port, QString user, QString passwd)
{
    switch (nDevice) {
    case 1:
        m_camara1->setTipoCamara(type);
        m_camara1->setCamaraHost(host);
        m_camara1->setCamaraPort(port);
        m_camara1->setCamaraUser(user);
        m_camara1->setCamaraPass(passwd);
        break;
    case 2:
        m_camara2->setTipoCamara(type);
        m_camara2->setCamaraHost(host);
        m_camara2->setCamaraPort(port);
        m_camara2->setCamaraUser(user);
        m_camara2->setCamaraPass(passwd);
        break;
    default:
        break;
    }
}
void nQTrucksEnginePrivate::setSettings(QSettings* value)
{
    if (value){
        m_settings = value;
    }
}

QSettings*nQTrucksEnginePrivate::settings()
{
    if (m_settings){
        return m_settings;
    }else{
        m_settings = new QSettings(
                    QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("nQTrucks"),
                    QSettings::IniFormat);
        return m_settings;
    }
}

/** Public Impl **/
/** **************************************************************************************** **/

nQTrucksEngine::nQTrucksEngine(QObject *parent)
    : QObject(parent)
    //, m_config(0)
    , d_ptr(new nQTrucksEnginePrivate())

{
    Q_D(nQTrucksEngine);
    d->q_ptr=this;
    connect(d->m_camara1,SIGNAL(ReplyCamaraIPFoto(QImage)),this,SIGNAL(CamaraIPFoto1(QImage)));
    connect(d->m_camara1,SIGNAL(CamaraIPWeb(QString)),this,SIGNAL(CamaraIPWeb1(QString)));
    connect(d->m_camara2,SIGNAL(ReplyCamaraIPFoto(QImage)),this,SIGNAL(CamaraIPFoto2(QImage)));
    connect(d->m_camara2,SIGNAL(CamaraIPWeb(QString)),this,SIGNAL(CamaraIPWeb2(QString)));

}

nQTrucksEngine::~nQTrucksEngine()
{
    delete d_ptr;
}

void nQTrucksEngine::setAppConfig(QSettings *value)
{
    Q_D(nQTrucksEngine);
    d->q_ptr=this;
    if (value){
        m_settings = value;
    }
}

QSettings *nQTrucksEngine::appConfig()
{
    Q_D(nQTrucksEngine);
    d->q_ptr=this;
    return d->settings();
}


QStringList nQTrucksEngine::getTiposCamaras()
{
    Q_D(nQTrucksEngine);
    return d->getCameraTypes();
}

void nQTrucksEngine::getCamaraFoto(int _ncamara)
{
    Q_D(nQTrucksEngine);
    switch (_ncamara) {
    case 1:
        d->m_camara1->sendCamaraIPFotoRequest();
        break;
    case 2:
        d->m_camara2->sendCamaraIPFotoRequest();
        break;
    default:
        break;
    }
}

void nQTrucksEngine::setCamaraIP(int nCamara, QString type, QString host, QString port, QString user, QString passwd)
{
    Q_D(nQTrucksEngine);
    d->setCamaraIP(nCamara,type,host,port,user,passwd);
}

nQTrucksEngine::nQTrucksEngine(nQTrucksEnginePrivate &dd, QObject *parent)
    : QObject(parent)
    , d_ptr(&dd)
{
    Q_D(nQTrucksEngine);
    d->q_ptr=this;
    //conector private
    //connect(d, SIGNAL(señal private), this, SIGNAL(señal lib));
}

/** ***************************************************************************************/

} /** END NAME SPACE **/
