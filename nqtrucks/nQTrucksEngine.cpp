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
#include <QMetaEnum>

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


    /** Bascula Types **/
    //qRegisterMetaType<Bascula>("Bascula");

    m_camara.resize(2);
    m_camara[0] = new Devices::CamaraIP(0,settings(), this);
    m_camara[1] = new Devices::CamaraIP(1,settings(), this);

    m_alpr.resize(2);
    m_alpr[0] = new Devices::NewsagesAlpr(0,settings(),this);
    m_alpr[1] = new Devices::NewsagesAlpr(1,settings(),this);

    m_report_editor = new LimeReport::ReportEngine(this);  /** MEMORY LEAK **/

    /** MAESTROS **/
    m_maestros = new Maestros::Maestros(this);

    /** Report Manager **/
    m_report_manager = new Db::ReportManager(this);
    /** END Report Manager **/


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

        if (info.vendorIdentifier()==0x1a86)
        {
            if(info.productIdentifier()==0x7523)
            {
                if (!info.isBusy())
                {
//                    if(info.serialNumber()=="")
//                    {
                        listIODevices.append(info.systemLocation());
//                    }
                }
            }
        }
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

         /*
          *
 "Port: ttyUSB0\nLocation: /dev/ttyUSB0\nDescription: USB2.0-Serial\nManufacturer: 1a86\nSerial number: \nVendor Identifier: 1a86\nProduct Identifier: 7523\nBusy: No\n"
          * */

        if (info.vendorIdentifier()!=0x1a86)
        {
            if(info.productIdentifier()!=0x7523)
            {
                if (!info.isBusy())
                {
//                    if(info.serialNumber()=="")
//                    {
                        listDevices.append(info.systemLocation());
//                    }
                }
            }
        }
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
    connect(d->m_camara[0],SIGNAL(ReplyCamaraIP(Camara)),this,SIGNAL(CamaraIP1(Camara)));
    connect(d->m_camara[0],SIGNAL(CamaraIPWeb(QString)),            this,SIGNAL(CamaraIPWeb1(QString)));
    /** 2 **/
    connect(d->m_camara[1],SIGNAL(ReplyCamaraIP(Camara)),this,SIGNAL(CamaraIP2(Camara)));
    connect(d->m_camara[1],SIGNAL(CamaraIPWeb(QString)),            this,SIGNAL(CamaraIPWeb2(QString)));
    /** END CAMARAS IP **/

    /** NEWSAGES IO **/
    connect(d->m_newsagesIO,&Devices::NewsagesIO::SemaforoConnectChanged,this,&nQTrucksEngine::SemaforoConnectChanged);
    connect(d->m_newsagesIO,&Devices::NewsagesIO::SemaforoEstadoChanged, this,&nQTrucksEngine::SemaforoEstadoChanged);
    /** END NEWSAGES IO **/

    /** BASCULAS **/
    connect(d->m_basculaReader1,&Devices::nQSerialPortReader::BasculaStatus, this,&nQTrucksEngine::BasculaStatus);
    connect(d->m_basculaReader1,&Devices::nQSerialPortReader::BasculaChanged,this,&nQTrucksEngine::BasculaChanged);
    /** END BASCULAS **/

    /** NEWSAGES ALPR **/
    /** 1 **/
    //connect(d->m_alpr[0],&Camara::ReplyOriginalFoto, this,&Camara::ReplyOriginalFoto1);
    connect(d->m_alpr[0],&Devices::NewsagesAlpr::ReplyOriginalFoto,                this,&nQTrucksEngine::ReplyOriginalFoto1);
    connect(d->m_alpr[0],&Devices::NewsagesAlpr::ReplyMatriculaCalibrationResults, this,&nQTrucksEngine::ReplyMatriculaCalibrationResults1);
    connect(d->m_alpr[0],&Devices::NewsagesAlpr::ReplyMatriculaResults,            this,&nQTrucksEngine::ReplyMatriculaResults1);

    /** 2 **/
    connect(d->m_alpr[1],&Devices::NewsagesAlpr::ReplyOriginalFoto,                this,&nQTrucksEngine::ReplyOriginalFoto2);
    connect(d->m_alpr[1],&Devices::NewsagesAlpr::ReplyMatriculaCalibrationResults, this,&nQTrucksEngine::ReplyMatriculaCalibrationResults2);
    connect(d->m_alpr[1],&Devices::NewsagesAlpr::ReplyMatriculaResults,            this,&nQTrucksEngine::ReplyMatriculaResults2);
    /** END NEWSAGES ALPR **/

    /** MAESTROS **/
    reloadMaestros();
    connect(d->m_maestros->m_RegistroPeso,SIGNAL(rangoFechasChanged(QVector<QDate>)),this,SIGNAL(rangoFechasChanged(QVector<QDate>)));


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
void nQTrucksEngine::calibrarFoto(const int &_device, const MatriculaResults &_camara)
{
    Q_D(nQTrucksEngine);
    switch (_device) {
    case 0:
        d->m_alpr[0]->calibrarFoto(_camara);
        break;
    case 1:
        d->m_alpr[1]->calibrarFoto(_camara);
        break;
    default:
        break;
    }
}

void nQTrucksEngine::getFotoMatricula(const int &_device, const MatriculaResults &_camara)
{
     Q_D(nQTrucksEngine);
    switch (_device) {
    case 0:
         d->m_alpr[0]->processFoto(_camara);
        break;
    case 1:
         d->m_alpr[1]->processFoto(_camara);
        break;
    default:
        break;
    }
}

/** END ALRP ****************************************************************************************************************/

/** CORE ****************************************************************************/
void nQTrucksEngine::setInitDaemon(const bool &_init){
    Q_D(nQTrucksEngine);
    if (_init){
         d->m_daemon = new Core::Daemon(d->m_basculaReader1,d->m_newsagesIO,d->m_camara,d->m_alpr, d->m_maestros, this);
        auto daemon_connect     = connect(d->m_daemon,&Core::Daemon::RegistroChanged,   this ,&nQTrucksEngine::daemonRegistroChanged);
        auto daemon_rowchanged  = connect(d->m_daemon,&Core::Daemon::rowsPesoChanged,   this ,&nQTrucksEngine::sincronizar_maestros);
        auto daemon_registrando = connect(d->m_daemon,&Core::Daemon::registrandoChanged,this ,&nQTrucksEngine::registrandoChanged);
         d->m_daemon->setInit(_init);
    } else{
        d->m_daemon->setInit(_init);
        d->m_daemon->deleteLater();
    }
}

void nQTrucksEngine::report_desingReport(){
    Q_D(nQTrucksEngine);
    d->m_report_editor->designReport();
}
/** END CORE ************************************************************************/

/** REPORTS ************************************************************/
void nQTrucksEngine::report_loadFromFile(const QString &_file){
    Q_D(nQTrucksEngine);
    d->m_report_editor->loadFromFile(_file);
}
    /** Report Manager **/
void nQTrucksEngine::printReport(const int &_ReportId, const QVector<long long> &_vectorRows){
    Q_D(nQTrucksEngine);

    switch (_ReportId) {
    case 0:
        d->m_report_manager->printRegistroMatricula(d->m_maestros->m_RegistroPeso->getCurrentDb(),_vectorRows[0]);
        break;
    case 1:
        d->m_report_manager->printRegistroMatriculaProcesada(d->m_maestros->m_RegistroPeso->getCurrentDb(),_vectorRows[0],_vectorRows[1]);
        break;
    }

}
    /** END Report Manager **/
/** END REPORTS *******************************************************/

/** MAESTROS *********************************************************************************/

void nQTrucksEngine::sincronizar_maestros(){
    Q_D(nQTrucksEngine);
    d->m_maestros->sincronizar();
    reloadMaestros();

}

void nQTrucksEngine::updateEmpresa(const QVector<QString> _empresaVector){
    Q_D(nQTrucksEngine);
    d->m_maestros->m_Empresa->actualizarEmpresa(_empresaVector);
}

void nQTrucksEngine::reloadMaestros(){
    Q_D(nQTrucksEngine);
    RegistrosPesos  = d->m_maestros->m_RegistroPeso;
    Empresa         = d->m_maestros->m_Empresa;
    //RegistrosPesos->query();
}

/** END MAESTROS ************************************************************/




/** *****************************************************************************************
 * END PUBLIC IMPL ******************************************************************************
*********************************************************************************************/

} /** END NAME SPACE **/
