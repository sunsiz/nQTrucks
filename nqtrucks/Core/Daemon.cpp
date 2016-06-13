#include "Daemon.h"
#include <QVariant>
#include <QBuffer>

#include <QThread>

namespace nQTrucks {
namespace Core{

Daemon::Daemon(Devices::nQSerialPortReader *_bascula, Devices::NewsagesIO *_newsagesIO, QVector<Devices::CamaraIP*> _camara, QVector<Devices::NewsagesAlpr*> _alpr, QObject *parent)
    : QObject(parent)
    , m_bascula(_bascula)
    , m_newsagesIO(_newsagesIO)
    , m_camara(_camara)
    , m_alpr(_alpr)
    , m_init(false)
    , m_registrando(false)
    , m_saliendo(false)
    , m_bfoto1(false)
    , m_bfoto2(false)
    , m_balpr1(false)
    , m_balpr2(false)
    //, m_tools(new Tools(this))

{
    QObject::connect(this,SIGNAL(initChanged(bool)),this,SLOT(onStartStop(bool)));
    QObject::connect(m_bascula,SIGNAL(BasculaPesoNuevo(Bascula)),this,SLOT(onPesoNuevo(Bascula)));
    QObject::connect(m_bascula,SIGNAL(BasculaChanged(Bascula)),this,SLOT(onBasculaChanged(Bascula)));

}

void Daemon::setInit(bool init)
{    
    if(m_init != init){
        m_init = init;        
        emit initChanged(m_init);
    }
}

void Daemon::onStartStop(const bool &_init){
    m_bascula->connectPort(_init);
    m_newsagesIO->setSemaforoDeviceConnect(_init);
    m_registrando=false;
    m_saliendo=false;
    m_balpr1=false;
    m_balpr2=false;
    m_bfoto1=false;
    m_bfoto2=false;
}

/** PESO *****************************/
void Daemon::onPesoNuevo(const Bascula &_nuevaPesada)
{
  if((m_init) && (!m_registrando)){
      // Bloqueo Registro
      m_registrando=true;
      m_saliendo=false;
      //Activo Semaforo rojo
      m_newsagesIO->setSemaforo(SEMAFORO_ROJO);

      //m_RegistroMatriculas = new SimpleMatriculas;
      // Peso:
      m_RegistroMatriculas.bascula = _nuevaPesada;
      //Espero 2 Fotos
      m_bfoto1=false;
      m_bfoto2=false;

      camaraconn1=connect(m_camara[0], &Devices::CamaraIP::ReplyCamaraIP, [=](const Registros::Camara  &_Reply){
          QObject::disconnect(camaraconn1);
          m_RegistroMatriculas.results[0].camara     = _Reply;
          //m_RegistroMatriculas.results[0].OrigenFotoByte = _Reply.OrigenFotoByte;
          m_bfoto1=true;
          //Tengo las fotos semaforo GO! y Registro linea
          if(m_bfoto1 && m_bfoto2){
              //m_saliendo=true;
              //m_newsagesIO->setSemaforo(SEMAFORO_VERDE);
              onGuardarRegistroSimple();
          }
      });

      camaraconn2=connect(m_camara[1], &Devices::CamaraIP::ReplyCamaraIP, [=](const Registros::Camara  &_Reply){
          QObject::disconnect(camaraconn2);
          m_RegistroMatriculas.results[1].camara      = _Reply;
          m_bfoto2=true;
          //Tengo las fotos semaforo GO! y Registro linea
          if(m_bfoto1 && m_bfoto2){
              //m_saliendo=true;
              //m_newsagesIO->setSemaforo(SEMAFORO_VERDE);
              onGuardarRegistroSimple();
          }
      });

      m_camara[0]->sendCamaraIPFotoRequest();
      m_camara[1]->sendCamaraIPFotoRequest();
  }

}

void Daemon::onBasculaChanged(const Bascula &_pesoRT){
    int peso_minimo=40;
    if (_pesoRT.iBruto > peso_minimo){
        if (!m_saliendo && !m_registrando){
            //TODO  no machakar el IO
            m_newsagesIO->setSemaforo(SEMAFORO_AMARILLO);
        }
    }else{
        if (m_saliendo){
            m_saliendo=false;
            m_newsagesIO->setSemaforo(SEMAFORO_VERDE);
        }
    }
}
/** END PESO ******************************/


/** DB **/
void Daemon::onGuardarRegistroSimple(){
    if (m_registrando){
        //consigue matriculas
        m_balpr1=false;
        m_balpr2=false;

        alprconn1 = connect(m_alpr[0], &Devices::NewsagesAlpr::ReplyMatriculaResults, [=](const Registros::MatriculaResults &_registro){
            QObject::disconnect(alprconn1);
            m_RegistroMatriculas.results[0]= _registro;
            m_balpr1=true;
            if(m_balpr1 && m_balpr2){
                onGuardarRegistroSimpleMatriculas();
            }
        });

        alprconn2 = connect(m_alpr[1],&Devices::NewsagesAlpr::ReplyMatriculaResults, [=](const Registros::MatriculaResults  &_registro){
            QObject::disconnect(alprconn2);
            m_RegistroMatriculas.results[1]= _registro;
            m_balpr2=true;
            if(m_balpr1 && m_balpr2){
                onGuardarRegistroSimpleMatriculas();
            }
        });

        m_alpr[0]->processFoto(m_RegistroMatriculas.results[0].camara);
        m_alpr[1]->processFoto(m_RegistroMatriculas.results[1].camara);
        //m_registrando=false;
        m_bfoto1=false;
        m_bfoto2=false;
    }   
}


void Daemon::onGuardarRegistroSimpleMatriculas(){

    /* Crea un Hilo para la base de datos */
    /* Ejecuta el registro Simple */

    emit RegistroChanged(m_RegistroMatriculas);

    hiloDb = new QThread();
    tareaDb = new Db::DatabaseManager;
    tareaDb->moveToThread(hiloDb);    
    tareaDb->setRegistroMatriculas(m_RegistroMatriculas);

    connect( hiloDb,  SIGNAL(started()),      tareaDb, SLOT(guardarRegistroSimpleMatriculas())  );
    connect( tareaDb, SIGNAL(workFinished()), hiloDb,  SLOT(quit()) );
    connect( tareaDb, SIGNAL(workFinished()), tareaDb, SLOT(deleteLater()) );
    connect( hiloDb,  SIGNAL(finished()),     hiloDb,  SLOT(deleteLater()) );

    hiloDb->start();

    m_registrando=false;
    m_saliendo=true;
}

/** END DB **/


///** END ALPRS *




}
}
