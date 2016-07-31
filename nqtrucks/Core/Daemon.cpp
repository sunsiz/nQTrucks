#include "Daemon.h"
#include <QVariant>
#include <QBuffer>

#include <QThread>

namespace nQTrucks {
namespace Core{

Daemon::Daemon(Devices::nQSerialPortReader *_bascula, Devices::NewsagesIO *_newsagesIO, QVector<Devices::CamaraIP*> _camara, QVector<Devices::NewsagesAlpr*> _alpr, Maestros::Maestros *_maestros, QObject *parent)
    : QObject(parent)
    , m_bascula(_bascula)
    , m_newsagesIO(_newsagesIO)
    , m_camara(_camara)
    , m_alpr(_alpr)
    , m_maestros(_maestros)
    , m_init(false)
    , m_registrando(false)
    , m_DaemonStatus(DAEMON_ESPERANDO)
    , m_bfoto1(false)
    , m_bfoto2(false)
    , m_balpr1(false)
    , m_balpr2(false)
    //, m_tools(new Tools(this))

{
    QObject::connect(this,     &Daemon::initChanged,                          this,&Daemon::onStartStop);
    QObject::connect(m_bascula,&Devices::nQSerialPortReader::BasculaPesoNuevo,this,&Daemon::onPesoNuevo);
    QObject::connect(m_bascula,&Devices::nQSerialPortReader::BasculaChanged,  this,&Daemon::onBasculaChanged);

}

void Daemon::setInit(bool init){    
    if(m_init != init){
        m_init = init;        
        emit initChanged(m_init);
    }
}

void Daemon::onStartStop(const bool &_init){
    setRegistrando(false);
    setDaemonStatus(DAEMON_ESPERANDO);
    m_balpr1=false;
    m_balpr2=false;
    m_bfoto1=false;
    m_bfoto2=false;

    m_bascula->connectPort(_init);
    m_newsagesIO->setSemaforoDeviceConnect(_init);

    if(_init){
        m_tolerancia_minima=m_bascula->reloadTolerancia_minima();
    }

}

bool Daemon::registrando() const {
    return m_registrando;
}

void Daemon::setRegistrando(bool registrando){
    m_registrando = registrando;
    if (registrando){
        setDaemonStatus(DAEMON_REGISTRANDO);
    }else {
        setDaemonStatus(DAEMON_SALIENDO);
    }
    emit registrandoChanged(m_registrando);
}

int Daemon::DaemonStatus() const{
    return m_DaemonStatus;
}

void Daemon::setDaemonStatus(int _status){
    m_DaemonStatus=_status;
}

/** PESO *****************************/
void Daemon::onPesoNuevo(const Bascula &_nuevaPesada){
    if((m_init) && (!m_registrando)){
      // Bloqueo Registro
      setRegistrando(true);
      m_RegistroMatriculas = new RegistroMatriculas;
      // Peso:
      m_RegistroMatriculas->m_bascula->setBascula(_nuevaPesada);
      //Espero 2 Fotos
      m_bfoto1=false;
      m_bfoto2=false;

      camaraconn1=connect(m_camara[0], &Devices::CamaraIP::ReplyCamaraIP, [=](const MatriculaResults  &_Reply){
          QObject::disconnect(camaraconn1);
          m_RegistroMatriculas->m_results0->setMatriculaResults(_Reply);
          m_bfoto1=true;
          //Tengo las fotos semaforo GO! y Registro linea
          if(m_bfoto1 && m_bfoto2){
              onGuardarRegistroSimple();
          }
      });

      camaraconn2=connect(m_camara[1], &Devices::CamaraIP::ReplyCamaraIP, [=](const MatriculaResults  &_Reply){
          QObject::disconnect(camaraconn2);
          m_RegistroMatriculas->m_results1->setMatriculaResults(_Reply);
          m_bfoto2=true;
          //Tengo las fotos semaforo GO! y Registro linea
          if(m_bfoto1 && m_bfoto2){
              onGuardarRegistroSimple();
          }
      });

      //Pido las dos fotos (TODO control en la clase CAMARA de los timeout si camara esta rota o desconectada)
      m_camara[0]->sendCamaraIPFotoRequest();
      m_camara[1]->sendCamaraIPFotoRequest();
  }

}

void Daemon::onBasculaChanged(const Bascula &_pesoRT){
    // Control de semaforo,
    // si no estoy registrando y entra carga mayor de la tolerancia, activo amarillo
    // si no se estabiliza y se pone a registrar, si baja de la tolerancia (objeto no para para su pesaje)
    // devolvemos el semaforo a verde.

    switch (m_DaemonStatus) {
    case DAEMON_ESPERANDO:
        m_newsagesIO->setSemaforo(SEMAFORO_VERDE);

        if (_pesoRT.getIBruto() >m_tolerancia_minima){
            setDaemonStatus(DAEMON_ENTRANDO);
        }else{
            setDaemonStatus(DAEMON_ESPERANDO);
        }
        break;

    case DAEMON_ENTRANDO:
        m_newsagesIO->setSemaforo(SEMAFORO_AMARILLO);

        if (_pesoRT.getIBruto() >m_tolerancia_minima){
            setDaemonStatus(DAEMON_ENTRANDO);
        }else{
            setDaemonStatus(DAEMON_ESPERANDO);
        }

        break;

    case DAEMON_REGISTRANDO:
        m_newsagesIO->setSemaforo(SEMAFORO_ROJO);

        break;

    case DAEMON_SALIENDO:
        m_newsagesIO->setSemaforo(SEMAFORO_VERDE);

        if (_pesoRT.getIBruto() >m_tolerancia_minima){
            setDaemonStatus(DAEMON_SALIENDO);
        }else{
            setDaemonStatus(DAEMON_ESPERANDO);
        }
        break;

    default:
        m_newsagesIO->setSemaforo(SEMAFORO_VERDE);

        setDaemonStatus(DAEMON_ESPERANDO);
        break;
    }
}
/** END PESO ******************************/


/** DB **/
void Daemon::onGuardarRegistroSimple(){
    if (registrando()){
        //consigue matriculas
        // Espero dos detectores
        m_balpr1=false;
        m_balpr2=false;

        alprconn1 = connect(m_alpr[0], &Devices::NewsagesAlpr::ReplyMatriculaResults, [=](const MatriculaResults &_registro){
            QObject::disconnect(alprconn1);            
            m_RegistroMatriculas->m_results0->setMatriculaResults(_registro);
            // Espero dos detectores
            m_balpr1=true;
            if(m_balpr1 && m_balpr2){
                onGuardarRegistroRegistroMatriculas();
            }
        });

        alprconn2 = connect(m_alpr[1],&Devices::NewsagesAlpr::ReplyMatriculaResults, [=](const MatriculaResults  &_registro){
            QObject::disconnect(alprconn2);
            m_RegistroMatriculas->m_results1->setMatriculaResults(_registro);
            // Espero dos detectores
            m_balpr2=true;
            if(m_balpr1 && m_balpr2){
                onGuardarRegistroRegistroMatriculas();
            }
        });
        //Peticion a los detectores
        m_alpr[0]->processFoto(*m_RegistroMatriculas->m_results0);
        m_alpr[1]->processFoto(*m_RegistroMatriculas->m_results1); 
    }   
}


void Daemon::onGuardarRegistroRegistroMatriculas(){
    if(m_registrando){
        // Ya puede irse el objeto de la bascula
        //m_newsagesIO->setSemaforo(SEMAFORO_VERDE);
        //setSaliendo(true);
        // Informo con la señal de los resultados
        emit RegistroChanged(m_RegistroMatriculas);

        /* Ejecuta el registro Simple */
        /** MODO HILO PRINCIPAL: **/
        tareaDb = new Db::DatabaseManager;
        connect( tareaDb, &Db::DatabaseManager::rowsPesoChanged, this, &Daemon::rowsPesoChanged);
        tareaDb->setMaestros(m_maestros);
        tareaDb->setRegistroMatriculas(m_RegistroMatriculas);
        tareaDb->guardarRegistroRegistroMatriculas();
        tareaDb->deleteLater();

        emit rowsPesoChanged();
        /** CONTROL DELETE **/
        m_RegistroMatriculas->deleteLater();
        setRegistrando(false);
        m_tolerancia_minima=m_bascula->reloadTolerancia_minima();
    }


    /** TODO MULTI HILO TODO: **/
}

/** END DB **/


///** END ALPRS *




}
}
