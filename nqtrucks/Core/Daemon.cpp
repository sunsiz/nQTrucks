#include "Daemon.h"
#include <QtSql>
#include <QVariant>

#include <QThread>

namespace nQTrucks {
namespace Core{

const int max_db_image_width = 1280;
const int max_db_image_height = 720;
const cv::Size fotoSize(max_db_image_width,max_db_image_height);

const int max_db_image_matricula_width = 55;
const int max_db_image_matricula_height = 11;


Daemon::Daemon(Devices::nQSerialPortReader *_bascula, Devices::NewsagesIO *_newsagesIO, QVector<Devices::CamaraIP *> _camara, Devices::NewsagesAlpr *_alpr1, Devices::NewsagesAlpr *_alpr2, QObject *parent)
    : QObject(parent)
    , m_bascula(_bascula)
    , m_newsagesIO(_newsagesIO)
    , m_camara(_camara)
    , m_alpr1(_alpr1)
    , m_alpr2(_alpr2)
    , m_init(false)
    , m_registrando(false)
    //, m_entrando(false)

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
        m_newsagesIO->setSemaforo(SEMAFORO_VERDE);
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
      //m_entrando=false;
      //Activo Semaforo rojo
      m_newsagesIO->setSemaforo(SEMAFORO_ROJO);

      m_registro_simple.camara1.clear();
      m_registro_simple.camara1resize.clear();
      m_registro_simple.camara2.clear();
      m_registro_simple.camara2resize.clear();

      // Peso:
      m_registro_simple.bascula=_nuevaPesada;

      //Espero 4 Fotos
      m_foto_numero=0;
      camaraconn1   = connect(m_camara[0],SIGNAL(ReplyCamaraIPFoto(QByteArray)) ,this,SLOT(onReplyCamaraIPFoto1(QByteArray)));
      camaraconn2   = connect(m_camara[1],SIGNAL(ReplyCamaraIPFoto(QByteArray)) ,this,SLOT(onReplyCamaraIPFoto2(QByteArray)));
      camaraconncv1 = connect(m_camara[0],SIGNAL(ReplyCamaraIPFotoCV(cv::Mat))  ,this,SLOT(onReplyCamaraIPFotoCV1(cv::Mat)));
      camaraconncv2 = connect(m_camara[1],SIGNAL(ReplyCamaraIPFotoCV(cv::Mat))  ,this,SLOT(onReplyCamaraIPFotoCV2(cv::Mat)));

      m_camara[0]->sendCamaraIPFotoRequest();
      m_camara[1]->sendCamaraIPFotoRequest();
  }

}

void Daemon::onBasculaChanged(const t_Bascula &_pesoRT){
    int peso_minimo=40;
    if (_pesoRT.iBruto > peso_minimo){
        if (!m_saliendo && !m_registrando){
            //m_entrando=true;
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


/** FOTOS   ******************************/
void Daemon::onReplyCamaraIPFoto1(const QByteArray &_Reply)
{
    QObject::disconnect(camaraconn1);
    m_registro_simple.camara1=_Reply;
    m_foto_numero++;
    //Tengo las fotos semaforo GO! y Registro linea
    if(m_foto_numero==4){
        m_saliendo=true;
        m_newsagesIO->setSemaforo(SEMAFORO_VERDE);
        onGuardarRegistroSimple(m_registro_simple);
    }
}

void Daemon::onReplyCamaraIPFotoCV1(const cv::Mat &_Reply)
{
    QObject::disconnect(camaraconncv1);

    cv::Mat cvimage =cv::Mat::zeros( fotoSize, CV_8UC3 );
    cv::resize(_Reply,cvimage,fotoSize);
    m_registro_simple.camara1resize=convertMat2ByteArray(cvimage);
    cvimage.release();

    m_foto_numero++;
    //Tengo las fotos semaforo GO! y Registro linea
    if(m_foto_numero==4){
        m_saliendo=true;
        m_newsagesIO->setSemaforo(SEMAFORO_VERDE);
        onGuardarRegistroSimple(m_registro_simple);
    }
}

void Daemon::onReplyCamaraIPFoto2(const QByteArray &_Reply)
{
    QObject::disconnect(camaraconn2);
    m_registro_simple.camara2=_Reply;
    m_foto_numero++;
    //Tengo las fotos semaforo GO! y Registro linea
    if(m_foto_numero==4){
        m_saliendo=true;
        m_newsagesIO->setSemaforo(SEMAFORO_VERDE);
        onGuardarRegistroSimple(m_registro_simple);
    }
}

void Daemon::onReplyCamaraIPFotoCV2(const cv::Mat &_Reply)
{
    QObject::disconnect(camaraconncv2);
    cv::Mat cvimage =cv::Mat::zeros( fotoSize, CV_8UC3 );
    cv::resize(_Reply,cvimage,fotoSize);
    m_registro_simple.camara2resize=convertMat2ByteArray(cvimage);
    cvimage.release();

    m_foto_numero++;
    //Tengo las fotos semaforo GO! y Registro linea
    if(m_foto_numero==4){
        m_saliendo=true;
        m_newsagesIO->setSemaforo(SEMAFORO_VERDE);
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
        qry.bindValue(":fotocamara1", _registro.camara1resize);
        qry.bindValue(":fotocamara2", _registro.camara2resize);

         if( !qry.exec() ){
           qDebug() << qry.lastError();
            db.rollback();
         }else{
            db.commit();


            //Pilla el registro simple
            m_registro_simple_matriculas.registrosimple = _registro;          

            //consigue matriculas
            m_alpr_numero=0;
            alprconn1 = connect(m_alpr1,SIGNAL(ReplyMatriculaResults(t_MatriculaResults)),this,SLOT(onReplyMatriculaResults1(t_MatriculaResults)));
            alprconn2 = connect(m_alpr2,SIGNAL(ReplyMatriculaResults(t_MatriculaResults)),this,SLOT(onReplyMatriculaResults2(t_MatriculaResults)));
            m_alpr1->processFoto(byteArray2Mat(_registro.camara1));
            m_alpr2->processFoto(byteArray2Mat(_registro.camara2));
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

        qry.bindValue(":fotocamara1",       _registro.registrosimple.camara1resize);
        qry.bindValue(":fotomatriculaA1",   _registro.fotomatriculaA1);
        qry.bindValue(":fotomatriculaB1",   _registro.fotomatriculaB1);
        qry.bindValue(":matriculaA1",       _registro.matriculaA1);
        qry.bindValue(":matriculaB1",       _registro.matriculaB1);
        qry.bindValue(":precisionA1",       QString::number(_registro.precisionA1,'g',6));
        qry.bindValue(":precisionB1",       QString::number(_registro.precisionB1,'g',6));


        qry.bindValue(":fotocamara2",       _registro.registrosimple.camara2resize);
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
           qDebug() <<  "Inserted!: " << qry.lastInsertId().toInt();
         }
        db.close();
    }

//    m_registro_simple_matriculas.fotomatriculaA1.clear();
//    m_registro_simple_matriculas.fotomatriculaB1.clear();
//    m_registro_simple_matriculas.fotomatriculaA2.clear();
//    m_registro_simple_matriculas.fotomatriculaB2.clear();
//    m_registro_simple_matriculas.matriculaA1.clear();
//    m_registro_simple_matriculas.matriculaA2.clear();
//    m_registro_simple_matriculas.matriculaB1.clear();
//    m_registro_simple_matriculas.matriculaB2.clear();
//    m_registro_simple_matriculas.registrosimple.camara1.clear();
//    m_registro_simple_matriculas.registrosimple.camara1resize.clear();
//    m_registro_simple_matriculas.registrosimple.camara2.clear();
//    m_registro_simple_matriculas.registrosimple.camara2resize.clear();
}

/** END DB **/




/** ALPRS **/

void Daemon::onReplyMatriculaResults1(const t_MatriculaResults &_registro){
    QObject::disconnect(alprconn1);
    m_registro_simple_matriculas.matriculaA1=_registro.MatriculaA;
    m_registro_simple_matriculas.matriculaB1=_registro.MatriculaB;
    m_registro_simple_matriculas.precisionA1=_registro.MatriculaPrecisionA;
    m_registro_simple_matriculas.precisionB1=_registro.MatriculaPrecisionB;
    m_registro_simple_matriculas.fotomatriculaA1= _registro.MatriculaFotoAByte;
    m_registro_simple_matriculas.fotomatriculaB1= _registro.MatriculaFotoBByte;

    m_alpr_numero++;
    if(m_alpr_numero ==2){
        onGuardarRegistroSimpleMatriculas(m_registro_simple_matriculas);
    }
}

void Daemon::onReplyMatriculaResults2(const t_MatriculaResults &_registro){
    QObject::disconnect(alprconn2);
    m_registro_simple_matriculas.matriculaA2=_registro.MatriculaA;
    m_registro_simple_matriculas.matriculaB2=_registro.MatriculaB;
    m_registro_simple_matriculas.precisionA2=_registro.MatriculaPrecisionA;
    m_registro_simple_matriculas.precisionB2=_registro.MatriculaPrecisionB;
    m_registro_simple_matriculas.fotomatriculaA2= _registro.MatriculaFotoAByte;
    m_registro_simple_matriculas.fotomatriculaB2= _registro.MatriculaFotoBByte;

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
    //cv::Mat mat =  cv::imdecode(pic,CV_LOAD_IMAGE_COLOR);
    return cv::imdecode(pic,CV_LOAD_IMAGE_COLOR);
}

QImage Daemon::convertMat2QImage(const cv::Mat &src)
{
    QImage qtImg= QImage();
    if( !src.empty() && src.depth() == CV_8U ){
        if(src.channels() == 1){
            qtImg = QImage( (const unsigned char *)(src.data),
                            src.cols,
                            src.rows,
                            QImage::Format_Indexed8 );
        }
        else{
            cv::cvtColor( src, src, CV_BGR2RGB );
            qtImg = QImage( (const unsigned char *)(src.data),
                            src.cols,
                            src.rows,
                            src.step,
                            QImage::Format_RGB888 );
        }
    }
    return qtImg;
}


QByteArray Daemon::convertMat2ByteArray(const cv::Mat &img){
    QImage qtImg = convertMat2QImage(img);
    QByteArray baScene; // byte array with data
    QBuffer buffer(&baScene);
    buffer.open(QIODevice::WriteOnly);
    qtImg.save(&buffer,"PNG");
    buffer.reset();
    buffer.close();
    return baScene;
}




}
}
