#include "Daemon.h"
#include <QtSql>
#include <QVariant>

namespace nQTrucks {
namespace Core{

Daemon::Daemon(Devices::nQSerialPortReader *_bascula, Devices::NewsagesIO *_newsagesIO, Devices::CamaraIP *_camara1, Devices::CamaraIP *_camara2, Devices::NewsagesAlpr *_alpr1, Devices::NewsagesAlpr *_alpr2, QObject *parent)
    : QObject(parent)
    , m_bascula(_bascula)
    , m_newsagesIO(_newsagesIO)
    , m_camara1(_camara1)
    , m_camara2(_camara2)
    , m_alpr1(_alpr1)
    , m_alpr2(_alpr2)
    , m_init(false)
    , m_registrando(false)
    , m_entrando(false)

{
    qRegisterMetaType<Registro_Simple>("Registro_Simple");
    qRegisterMetaType<Registro_Simple_Matriculas>("Registro_Simple_Matriculas");

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
      m_entrando=false;
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
    int peso_minimo=40;
    if (_pesoRT.iBruto > peso_minimo){
        if (!m_saliendo){
            m_entrando=true;
            //TODO  no machakar el IO
            m_newsagesIO->setValuePin10(m_entrando);
        }
    }else{
        if (m_saliendo){
            m_saliendo=false;
        }
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
void Daemon::onGuardarRegistroSimple(Registro_Simple &_registro)
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

            m_registro_simple_matriculas = m_registro_simple_matriculas_vacio;

            //Pilla el registro simple
            m_registro_simple_matriculas.registrosimple = _registro;

            //consigue matriculas
            m_alpr_numero=0;
            connect(m_alpr1,SIGNAL(ReplyMatriculaResults(t_MatriculaResults)),this,SLOT(onReplyMatriculaResults1(t_MatriculaResults)));
            connect(m_alpr2,SIGNAL(ReplyMatriculaResults(t_MatriculaResults)),this,SLOT(onReplyMatriculaResults2(t_MatriculaResults)));
            m_alpr1->processFoto(byteArray2Mat(m_registro_simple_matriculas.registrosimple.camara1));
            m_alpr2->processFoto(byteArray2Mat(m_registro_simple_matriculas.registrosimple.camara2));
           qDebug() <<  "Inserted!: " << qry.lastInsertId().toInt();
         }

        db.close();
    }
    m_registrando=false;


}


void Daemon::onGuardarRegistroSimpleMatriculas(Registro_Simple_Matriculas &_registro){

    if( !db.open() )
    {
      qDebug() << db.lastError();
      qDebug() << "Failed to connect." ;
    }else{
        qDebug( "Connected!" );
        db.transaction();
        QSqlQuery qry;
        qry.prepare( "INSERT INTO registros_matriculas( "
                     " pesobruto,  pesoneto,  pesotara, "
                     " fotocamara1, fotomatriculaA1, fotomatriculaB1, matriculaA1,  matriculaB1, precisionA1, precisionB1,"
                     " fotocamara2, fotomatriculaA2, fotomatriculaB2, matriculaA2,  matriculaB2, precisionA2, precisionB2 "
                     " ) "
                     " VALUES  ("
                     " :pesobruto,   :pesoneto,  :pesotara, "
                     " :fotocamara1, :fotomatriculaA1, :fotomatriculaB1, :matriculaA1,  :matriculaB1, :precisionA1, :precisionB1,"
                     " :fotocamara2, :fotomatriculaA2, :fotomatriculaB2, :matriculaA2,  :matriculaB2, :precisionA2, :precisionB2 "
                     " );"
                     );

        qry.bindValue(":pesobruto",         _registro.registrosimple.bascula.iBruto);
        qry.bindValue(":pesoneto",          _registro.registrosimple.bascula.iNeto);
        qry.bindValue(":pesotara",          _registro.registrosimple.bascula.iTara);

        qry.bindValue(":fotocamara1",       _registro.registrosimple.camara1);
        qry.bindValue(":fotomatriculaA1",   _registro.fotomatriculaA1);
        qry.bindValue(":fotomatriculaB1",   _registro.fotomatriculaB1);
        qry.bindValue(":matriculaA1",       _registro.matriculaA1);
        qry.bindValue(":matriculaB1",       _registro.matriculaB1);
        qry.bindValue(":precisionA1",       QString::number(_registro.precisionA1,'g',6));
        qry.bindValue(":precisionB1",       QString::number(_registro.precisionB1,'g',6));


        qry.bindValue(":fotocamara2",       _registro.registrosimple.camara2);
        qry.bindValue(":fotomatriculaA2",   _registro.fotomatriculaA2);
        qry.bindValue(":fotomatriculaB2",   _registro.fotomatriculaB2);
        qry.bindValue(":matriculaA2",       _registro.matriculaA2);
        qry.bindValue(":matriculaB2",       _registro.matriculaB2);
        qry.bindValue(":precisionA2",       QString::number(_registro.precisionA2,'g',6));
        qry.bindValue(":precisionB2",       QString::number(_registro.precisionB2,'g',6));





         if( !qry.exec() ){
           qDebug() << qry.lastError();
            db.rollback();
         }else{
            db.commit();
            //desconecta
            m_alpr1->disconnect();
            m_alpr2->disconnect();
           qDebug() <<  "Inserted!: " << qry.lastInsertId().toInt();
         }
        db.close();
    }


}

/** END DB **/




/** ALPRS **/

void Daemon::onReplyMatriculaResults1(const t_MatriculaResults &_registro){

    m_registro_simple_matriculas.matriculaA1=_registro.MatriculaA;
    m_registro_simple_matriculas.matriculaB1=_registro.MatriculaB;
    m_registro_simple_matriculas.precisionA1=_registro.MatriculaPrecisionA;
    m_registro_simple_matriculas.precisionB1=_registro.MatriculaPrecisionB;
    m_registro_simple_matriculas.fotomatriculaA1=_registro.MatriculaFotoAByte;
    m_registro_simple_matriculas.fotomatriculaB1=_registro.MatriculaFotoBByte;

    m_alpr_numero++;
    if(m_alpr_numero ==2){
        onGuardarRegistroSimpleMatriculas(m_registro_simple_matriculas);
    }
}

void Daemon::onReplyMatriculaResults2(const t_MatriculaResults &_registro){

    m_registro_simple_matriculas.matriculaA2=_registro.MatriculaA;
    m_registro_simple_matriculas.matriculaB2=_registro.MatriculaB;
    m_registro_simple_matriculas.precisionA2=_registro.MatriculaPrecisionA;
    m_registro_simple_matriculas.precisionB2=_registro.MatriculaPrecisionB;
    m_registro_simple_matriculas.fotomatriculaA2=_registro.MatriculaFotoAByte;
    m_registro_simple_matriculas.fotomatriculaB2=_registro.MatriculaFotoBByte;

    m_alpr_numero++;
    if(m_alpr_numero ==2){
        onGuardarRegistroSimpleMatriculas(m_registro_simple_matriculas);
    }

}

/** END ALPRS **/






cv::Mat Daemon::byteArray2Mat(QByteArray & byteArray)
{
    const char* begin = reinterpret_cast<char*>(byteArray.data());
    const char* end = begin + byteArray.size();
    std::vector<char> pic(begin, end);
    cv::Mat mat =  cv::imdecode(pic,CV_LOAD_IMAGE_COLOR);
    return mat.clone();
}


}
}
