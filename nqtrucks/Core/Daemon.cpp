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
    , m_saliendo(false)
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

void Daemon::setInit(bool init)
{    
    if(m_init != init){
        m_init = init;        
        emit initChanged(m_init);
    }
}

void Daemon::onStartStop(const bool &_init){
    setRegistrando(false);
    m_saliendo=false;
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

bool Daemon::registrando() const
{
    //emit registrandoChanged(m_registrando);
    return m_registrando;
}

void Daemon::setRegistrando(bool registrando)
{
    m_registrando = registrando;
    emit registrandoChanged(m_registrando);
}

/** PESO *****************************/
void Daemon::onPesoNuevo(const Bascula &_nuevaPesada)
{
    if((m_init) && (!m_registrando)){
        // Bloqueo Registro
      setRegistrando(true);
      m_saliendo=false;
      //Activo Semaforo rojo
      m_newsagesIO->setSemaforo(SEMAFORO_ROJO);

      //m_RegistroMatriculas = new RegistroMatriculas;
      // Peso:
      m_RegistroMatriculas.m_bascula->setBascula(_nuevaPesada);
      //Espero 2 Fotos
      m_bfoto1=false;
      m_bfoto2=false;

      camaraconn1=connect(m_camara[0], &Devices::CamaraIP::ReplyCamaraIP, [=](const Camara  &_Reply){
          QObject::disconnect(camaraconn1);
          m_RegistroMatriculas.m_results0->camara->setCamara(_Reply);
          //m_RegistroMatriculas.m_results0->camara->convertirFotos();
          //m_RegistroMatriculas.results[0].OrigenFotoByte = _Reply.OrigenFotoByte;
          m_bfoto1=true;
          //Tengo las fotos semaforo GO! y Registro linea
          if(m_bfoto1 && m_bfoto2){
              //m_saliendo=true;
              //m_newsagesIO->setSemaforo(SEMAFORO_VERDE);
              onGuardarRegistroSimple();
          }
      });

      camaraconn2=connect(m_camara[1], &Devices::CamaraIP::ReplyCamaraIP, [=](const Camara  &_Reply){
          QObject::disconnect(camaraconn2);
          m_RegistroMatriculas.m_results1->camara->setCamara(_Reply);
          //m_RegistroMatriculas.m_results1->camara->convertirFotos();
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
    //int peso_minimo=m_bascula->reloadTolerancia_minima();
    if (_pesoRT.getIBruto() > m_tolerancia_minima){
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
    if (registrando()){
        //consigue matriculas
        m_balpr1=false;
        m_balpr2=false;

        alprconn1 = connect(m_alpr[0], &Devices::NewsagesAlpr::ReplyMatriculaResults, [=](const MatriculaResults &_registro){
            QObject::disconnect(alprconn1);            
            m_RegistroMatriculas.m_results0->setMatriculaResults(_registro);
            m_balpr1=true;
            if(m_balpr1 && m_balpr2){
                onGuardarRegistroRegistroMatriculas();
            }
        });

        alprconn2 = connect(m_alpr[1],&Devices::NewsagesAlpr::ReplyMatriculaResults, [=](const MatriculaResults  &_registro){
            QObject::disconnect(alprconn2);
            m_RegistroMatriculas.m_results1->setMatriculaResults(_registro);
            m_balpr2=true;
            if(m_balpr1 && m_balpr2){
                onGuardarRegistroRegistroMatriculas();
            }
        });

        m_alpr[0]->processFoto(*m_RegistroMatriculas.m_results0->camara->getCamara());
        m_alpr[1]->processFoto(*m_RegistroMatriculas.m_results1->camara->getCamara());
        m_bfoto1=false;
        m_bfoto2=false;
    }   
}


void Daemon::onGuardarRegistroRegistroMatriculas(){

    /* Crea un Hilo para la base de datos */
    /* Ejecuta el registro Simple */
    //m_RegistroMatriculas.m_results0->convertirFotos();
    //m_RegistroMatriculas.m_results1->convertirFotos();
    emit RegistroChanged(m_RegistroMatriculas);

    hiloDb = new QThread();
    tareaDb = new Db::DatabaseManager(m_maestros);
    tareaDb->moveToThread(hiloDb);    
    tareaDb->setRegistroMatriculas(m_RegistroMatriculas);

    connect( hiloDb,  &QThread::started                 , tareaDb, &Db::DatabaseManager::guardarRegistroRegistroMatriculas  );
    connect( tareaDb, &Db::DatabaseManager::workFinished, hiloDb,  &QThread::quit );
    connect( tareaDb, &Db::DatabaseManager::workFinished, tareaDb, &QObject::deleteLater );
    connect( hiloDb,  &QThread::finished                , hiloDb,  &QObject::deleteLater );
    /** INFORMAR DE CAMBIOS EN ROWS **/
    connect( tareaDb, &Db::DatabaseManager::rowsPesoChanged, this, &Daemon::rowsPesoChanged);

    hiloDb->start();

    setRegistrando(false);
    m_saliendo=true;
}

/** END DB **/


///** END ALPRS *




}
}
