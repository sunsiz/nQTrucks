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

#include "nQTrucksEngine_p.h"
#include "nQTrucksEngine.h"

namespace nQTrucks{
QSettings* nQTrucksEngine::m_settings = 0;


/** ****************************************************************************************************
 * IMPL PRIVATE ****************************************************************************************
********************************************************************************************************/
/** Private Share Pointer Impl **/
nQTrucksEnginePrivate::nQTrucksEnginePrivate(QObject *parent)
: QObject(parent)
, m_settings(0)
{

    /** ALPR TYPES **/
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<Planck>("Planck");
    qRegisterMetaType<nQTrucks::t_Prewarp>("nQTrucks::t_Prewarp");
    qRegisterMetaType<Registros::MatriculaResults>("Registros::MatriculaResults");


    /** Daemon Types **/
    qRegisterMetaType<Registros::Simple>            ("Registros::Simple");
    qRegisterMetaType<Registros::Matriculas>        ("Registros::Matriculas");
    qRegisterMetaType<SimpleMatriculas>             ("SimpleMatriculas");

    /** Bascula Types **/
    qRegisterMetaType<Bascula>("Bascula");

    /** Camara Types **/
    QLoggingCategory::setFilterRules("qt.network.ssl.warning=false");
    qRegisterMetaType<Registros::Camara>("Registros::Camara");

    m_camara.resize(2);
    m_camara[0] = new Devices::CamaraIP(0,settings(), this);
    m_camara[1] = new Devices::CamaraIP(1,settings(), this);

    m_alpr.resize(2);
    m_alpr[0] = new Devices::NewsagesAlpr(0,settings(),this);
    m_alpr[1] = new Devices::NewsagesAlpr(1,settings(),this);

    m_report_editor = new LimeReport::ReportEngine(this);


}

nQTrucksEnginePrivate::~nQTrucksEnginePrivate()
{

}

/** SETTINGS **********************************************************************************************/
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
/** END SETTINGS ********************************************************************************************/

/** CAMARAS  *************************************************************************************************/
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
/** END CAMARAS **************************************************************************************************/

/** NEWSAGES I/O *************************************************************************************************/
QStringList nQTrucksEnginePrivate::getIODevices()
{
    QStringList listIODevices;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        /* qDebug() << QObject::tr("Port: ") + info.portName() + "\n"
                    + QObject::tr("Location: ") + info.systemLocation() + "\n"
                    + QObject::tr("Description: ") + info.description() + "\n"
                    + QObject::tr("Manufacturer: ") + info.manufacturer() + "\n"
                    + QObject::tr("Serial number: ") + info.serialNumber() + "\n"
                    + QObject::tr("Vendor Identifier: ") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Product Identifier: ") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";
       */

//        if (info.manufacturer()=="Newsages NewsTechs")
//        {
//            if(info.description()=="NewsagesIO")
//            {
                if (!info.isBusy())
                {
                        listIODevices.append(info.systemLocation());
                }
//            }
//        }
    }


    return listIODevices;
}
/** FIN NEWSAGES I/O ************************************************************************************************************/

/** BASCULAS ********************************************************************************************************************/
QStringList nQTrucksEnginePrivate::getBasculaTypes()
{
    QMetaObject MetaObject = Devices::nQSerialPortReader::staticMetaObject;
    QMetaEnum MetaEnum = MetaObject.enumerator(MetaObject.indexOfEnumerator("BasculaType"));
    QStringList listTypes;
    for (int i = 0; i < MetaEnum.keyCount(); i++)
    {
        listTypes.insert(MetaEnum.value(i), MetaEnum.key(i));
    }
    return listTypes;
}

QStringList nQTrucksEnginePrivate::getSerialDevices()
{
    QStringList listDevices;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
         qDebug() << QObject::tr("Port: ") + info.portName() + "\n"
                    + QObject::tr("Location: ") + info.systemLocation() + "\n"
                    + QObject::tr("Description: ") + info.description() + "\n"
                    + QObject::tr("Manufacturer: ") + info.manufacturer() + "\n"
                    + QObject::tr("Serial number: ") + info.serialNumber() + "\n"
                    + QObject::tr("Vendor Identifier: ") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Product Identifier: ") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";

//        if (info.manufacturer()=="Newsages NewsTechs")
//        {
//            if(info.description()=="NewsagesIO")
//            {
//                if (!info.isBusy())
//                {
//                    if(info.serialNumber()=="")
//                    {
                        listDevices.append(info.systemLocation());
//                    }
//                }
//            }
//        }
    }
    return listDevices;

}
/** END BASCULAS ***************************************************************************************/

/** ****************************************************************************************************
 *  END IMPL PRIVATE ***********************************************************************************
********************************************************************************************************/


/** *****************************************************************************************
 * PUBLIC IMPL ******************************************************************************
*********************************************************************************************/
nQTrucksEngine::nQTrucksEngine(QObject *parent)
    : QObject(parent)
    //, m_config(0)
    , d_ptr(new nQTrucksEnginePrivate())

{
    Q_D(nQTrucksEngine);
    d->q_ptr=this;

    /** CAMARAS IP **/
    /** 1 **/
    connect(d->m_camara[0],SIGNAL(ReplyCamaraIPFotoCV(cv::Mat)),this,SIGNAL(CamaraIPFotoCV1(cv::Mat)));
    connect(d->m_camara[0],SIGNAL(CamaraIPWeb(QString)),this,SIGNAL(CamaraIPWeb1(QString)));
    /** 2 **/
    connect(d->m_camara[1],SIGNAL(ReplyCamaraIPFotoCV(cv::Mat)),this,SIGNAL(CamaraIPFotoCV2(cv::Mat)));
    connect(d->m_camara[1],SIGNAL(CamaraIPWeb(QString)),this,SIGNAL(CamaraIPWeb2(QString)));
    /** END CAMARAS IP **/

    /** NEWSAGES IO **/
    connect(d->m_newsagesIO,SIGNAL(SemaforoConnectChanged(bool)),this,SIGNAL(SemaforoConnectChanged(bool)));
    //connect(d->m_newsagesIO,SIGNAL(readyChanged(bool)),this,SIGNAL(IODeviceReadyChanged(bool)));
    connect(d->m_newsagesIO,SIGNAL(SemaforoEstadoChanged(int)),this,SIGNAL(SemaforoEstadoChanged(int)));
    /** END NEWSAGES IO **/

    /** BASCULAS **/
    connect(d->m_basculaReader1,SIGNAL(BasculaStatus(bool)),this,SIGNAL(BasculaStatus(bool)));
    connect(d->m_basculaReader1,SIGNAL(BasculaChanged(Bascula)),this,SIGNAL(BasculaChanged(Bascula)));
    /** END BASCULAS **/

    /** NEWSAGES ALPR **/
    /** 1 **/
    connect(d->m_alpr[0],SIGNAL(ReplyOriginalFoto(cv::Mat)),this,SIGNAL(ReplyOriginalFoto1(cv::Mat)));
    connect(d->m_alpr[0],SIGNAL(ReplyOriginalFotoBlanca(cv::Mat)),this,SIGNAL(ReplyOriginalFotoBlanca1(cv::Mat)));
    connect(d->m_alpr[0],SIGNAL(ReplyOriginalFotoRoja(cv::Mat)),this,SIGNAL(ReplyOriginalFotoRoja1(cv::Mat)));
    connect(d->m_alpr[0],SIGNAL(ReplyMatriculaResults(Registros::MatriculaResults)),this,SIGNAL(ReplyMatriculaResults1(Registros::MatriculaResults)));

    /** 2 **/
    connect(d->m_alpr[1],SIGNAL(ReplyOriginalFoto(cv::Mat)),this,SIGNAL(ReplyOriginalFoto2(cv::Mat)));
    connect(d->m_alpr[1],SIGNAL(ReplyOriginalFotoBlanca(cv::Mat)),this,SIGNAL(ReplyOriginalFotoBlanca2(cv::Mat)));
    connect(d->m_alpr[1],SIGNAL(ReplyOriginalFotoRoja(cv::Mat)),this,SIGNAL(ReplyOriginalFotoRoja2(cv::Mat)));
    connect(d->m_alpr[1],SIGNAL(ReplyMatriculaResults(Registros::MatriculaResults)),this,SIGNAL(ReplyMatriculaResults2(Registros::MatriculaResults)));
    /** END NEWSAGES ALPR **/

}

nQTrucksEngine::nQTrucksEngine(nQTrucksEnginePrivate &dd, QObject *parent)
    : QObject(parent)
    , d_ptr(&dd)
{
    Q_D(nQTrucksEngine);
    d->q_ptr=this;

}

nQTrucksEngine::~nQTrucksEngine()
{
    delete d_ptr;
}

/** SETTINGS ***********************************************************************************************************/
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
/** END SETTINGS ********************************************************************************************************/

/** CAMARAS *************************************************************************************************************/
QStringList nQTrucksEngine::getTiposCamaras()
{
    Q_D(nQTrucksEngine);
    return d->getCameraTypes();
}

void nQTrucksEngine::getCamaraFoto(int _ncamara)
{
    Q_D(nQTrucksEngine);
    switch (_ncamara) {
    case 0:
        d->m_camara[0]->sendCamaraIPFotoRequest();
        break;
    case 1:
        d->m_camara[1]->sendCamaraIPFotoRequest();
        break;
    default:
        break;
    }
}

/** END CAMARAS ***********************************************************************************************************/

/** NEWSAGES I/O **********************************************************************************************************/
QStringList nQTrucksEngine::getIODevices()
{
    Q_D(nQTrucksEngine);
    return d->getIODevices();
}

void nQTrucksEngine::setSemaforoDevicesConnect(const bool &_value)
{
    Q_D(nQTrucksEngine);
    d->m_newsagesIO->setSemaforoDeviceConnect(_value);
}

void nQTrucksEngine::setSemaforoStatus(const int &_color)
{
    Q_D(nQTrucksEngine);
    d->m_newsagesIO->setSemaforo(_color);
}
/** END NEWSAGES I/O *******************************************************************************************************/

/** BASCULAS  **************************************************************************************************************/
QStringList nQTrucksEngine::getTiposBasculas()
{
    Q_D(nQTrucksEngine);
    return d->getBasculaTypes();
}

void nQTrucksEngine::setBasculaConnect(bool _value)
{
    Q_D(nQTrucksEngine);
    d->m_basculaReader1->connectPort(_value);
}

QStringList nQTrucksEngine::getSerialDevices()
{
    Q_D(nQTrucksEngine);
    return d->getSerialDevices();
}
/** END BASCULAS ************************************************************************************************************/


/** ALRP ********************************************************************************************************************/
void nQTrucksEngine::calibrarFoto(const int &_device, const cv::Mat &_foto)
{
    Q_D(nQTrucksEngine);
    switch (_device) {
    case 0:
        d->m_alpr[0]->calibrarFoto(_foto.clone());
        break;
    case 1:
        d->m_alpr[1]->calibrarFoto(_foto.clone());
        break;
    default:
        break;
    }
}

void nQTrucksEngine::getFotoMatricula(const int &_device, const cv::Mat &_foto)
{
     Q_D(nQTrucksEngine);
    switch (_device) {
    case 0:
         d->m_alpr[0]->processFoto(_foto.clone());
        break;
    case 1:
         d->m_alpr[1]->processFoto(_foto.clone());
        break;
    default:
        break;
    }
}

/** END ALRP ****************************************************************************************************************/

/** CORE ****************************************************************************/
void nQTrucksEngine::setInitDaemon(const bool &_init)
{
    Q_D(nQTrucksEngine);
    if (_init){
         d->m_daemon = new Core::Daemon(d->m_basculaReader1,d->m_newsagesIO,d->m_camara,d->m_alpr, this);
        auto daemon_connect = connect(d->m_daemon,SIGNAL(RegistroChanged(SimpleMatriculas)),
                this       ,SIGNAL(daemonRegistroChanged(SimpleMatriculas)));
         d->m_daemon->setInit(_init);
    } else{
        d->m_daemon->setInit(_init);
        d->m_daemon->deleteLater();
    }
}

void nQTrucksEngine::report_desingReport()
{
    Q_D(nQTrucksEngine);
    d->m_report_editor->designReport();
}
/** END CORE ************************************************************************/

/** REPORTS ************************************************************/
void nQTrucksEngine::report_loadFromFile(const QString &_file)
{
    Q_D(nQTrucksEngine);
    d->m_report_editor->loadFromFile(_file);
}
/** END REPORTS *******************************************************/


/** *****************************************************************************************
 * END PUBLIC IMPL ******************************************************************************
*********************************************************************************************/

} /** END NAME SPACE **/
