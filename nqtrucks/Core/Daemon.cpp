#include "Daemon.h"
#include <QtSql>

namespace nQTrucks {
namespace Core{

Daemon::Daemon(Devices::nQSerialPortReader *_bascula, Devices::NewsagesIO *_newsagesIO, Devices::CamaraIP *_camara1, Devices::CamaraIP *_camara2, QObject *parent)
    : QObject(parent)
    , m_bascula(_bascula)
    , m_newsagesIO(_newsagesIO)
    , m_camara1(_camara1)
    , m_camara2(_camara2)
    , m_init(false)
    , m_registrando(false)
    , m_saliendo(false)
{
    qRegisterMetaType<Registro_Simple>("Registro_Simple");

    /*TODO SETTINGS AND CLASS */
    db = QSqlDatabase::addDatabase( "QMYSQL" );
    db.setHostName( "localhost" );
    db.setDatabaseName( "nqtrucks" );
    db.setUserName( "nqtrucks" );
    db.setPassword( "nqtrucks" );

    QObject::connect(this,SIGNAL(initChanged(bool)),this,SLOT(onStartStop(bool)));
    QObject::connect(m_bascula,SIGNAL(BasculaPesoNuevo(t_Bascula)),this,SLOT(onPesoNuevo(t_Bascula)));
    QObject::connect(m_bascula,SIGNAL(BasculaChanged(t_Bascula)),this,SLOT(onBasculaChanged(t_Bascula)));

}

void Daemon::setInit(bool init)
{
    if(m_init != init){
        m_init = init;
        emit initChanged(m_init);
    }
}

void Daemon::onStartStop(const bool &_init){
    if (_init){
        m_bascula->connectPort(_init);
        m_newsagesIO->setIODeviceConnect(true);
        m_newsagesIO->setValuePin10(false);
    }else{
        m_bascula->connectPort(false);
        m_registrando=false;
    }
}

/** PESO *****************************/
void Daemon::onPesoNuevo(const t_Bascula &_nuevaPesada)
{
  if((m_init) && (!m_registrando)){
      // Bloqueo Registro
      m_registrando=true;
      m_saliendo=false;
      //Activo Semaforo rojo
      m_newsagesIO->setValuePin10(m_registrando);

      m_registro_simple=m_registro_simple_vacio;
      // Peso:
      m_registro_simple.bascula=_nuevaPesada;

      //Espero 2 Fotos
      m_foto_numero=0;
      connect(m_camara1,SIGNAL(ReplyCamaraIPFoto(QByteArray)),this,SLOT(onReplyCamaraIPFoto1(QByteArray)));
      connect(m_camara2,SIGNAL(ReplyCamaraIPFoto(QByteArray)),this,SLOT(onReplyCamaraIPFoto2(QByteArray)));
      m_camara1->sendCamaraIPFotoRequest();
      m_camara2->sendCamaraIPFotoRequest();
  }

}

void Daemon::onBasculaChanged(const t_Bascula &_pesoRT){

    // TODO  ==0  por == tolerancia de error;
    int terror=40;
    if ((m_saliendo) && (_pesoRT.iBruto<terror)){
        m_saliendo=false;
    }

    if( (m_saliendo)  && (!m_registrando)){
        //Activo Semaforo rojo
           // if (!m_newsagesIO->ValuePin10()){
                m_newsagesIO->setValuePin10(!m_registrando);
           // }
        }
}




/** END PESO ******************************/


/** FOTOS   ******************************/
void Daemon::onReplyCamaraIPFoto1(const QByteArray &_Reply)
{
    m_camara1->disconnect();
    m_registro_simple.camara1=_Reply;
    m_foto_numero++;
    //Tengo las dos fotos semaforo GO! y Registro linea
    if(m_foto_numero==2){
        m_saliendo=true;
        m_newsagesIO->setValuePin10(false);
        onGuardarRegistroSimple(m_registro_simple);
    }
}

void Daemon::onReplyCamaraIPFoto2(const QByteArray &_Reply)
{
    m_camara2->disconnect();
    m_registro_simple.camara2=_Reply;
    m_foto_numero++;
    //Tengo las dos fotos semaforo GO! y Registro linea
    if(m_foto_numero==2){
        m_saliendo=true;
        m_newsagesIO->setValuePin10(false);
        onGuardarRegistroSimple(m_registro_simple);
    }
}

/** END FOTOS **************************/


/** DB **/
void Daemon::onGuardarRegistroSimple(const Registro_Simple &_registro)
{
    if( !db.open() )
    {
      qDebug() << db.lastError();
      //qFatal( "Failed to connect." );
      qDebug() << "Failed to connect." ;
    }else{
        qDebug( "Connected!" );

        db.transaction();
        QSqlQuery qry;
        qry.prepare( "INSERT INTO registros( pesobruto,  pesoneto,  pesotara,  fotocamara1,  fotocamara2)"
                     "               VALUES(:pesobruto, :pesoneto, :pesotara, :fotocamara1, :fotocamara2)" );

        qry.bindValue(":pesobruto",   _registro.bascula.iBruto);
        qry.bindValue(":pesoneto",    _registro.bascula.iNeto);
        qry.bindValue(":pesotara",    _registro.bascula.iTara);
        qry.bindValue(":fotocamara1", _registro.camara1);
        qry.bindValue(":fotocamara2", _registro.camara2);

         if( !qry.exec() ){
           qDebug() << qry.lastError();
            db.rollback();
         }else{
            db.commit();
           qDebug( "Inserted!" );
         }

        db.close();
    }
    m_registrando=false;


}
/** END DB **/





}
}
