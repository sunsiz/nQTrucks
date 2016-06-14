#include "DatabaseManager.h"
#include <QtSql>
#include <QDebug>

namespace nQTrucks{
namespace Db{

static const QString qry_insert_simple =  " INSERT INTO registros_matriculas( "
                                          " pesobruto,  pesoneto,  pesotara, "
                                          " fotocamara1, fotomatriculaA1, fotomatriculaB1, matriculaA1,  matriculaB1, precisionA1, precisionB1,"
                                          " fotocamara2, fotomatriculaA2, fotomatriculaB2, matriculaA2,  matriculaB2, precisionA2, precisionB2 "
                                          " ) "
                                          " VALUES  ("
                                          " :pesobruto,   :pesoneto,  :pesotara, "
                                          " :fotocamara1, :fotomatriculaA1, :fotomatriculaB1, :matriculaA1,  :matriculaB1, :precisionA1, :precisionB1,"
                                          " :fotocamara2, :fotomatriculaA2, :fotomatriculaB2, :matriculaA2,  :matriculaB2, :precisionA2, :precisionB2 "
                                          " );";

static const QString qry_delete_fotos =  " UPDATE registros_matriculas "
                                         " SET "
                                         " fotocamara1 = :fotocamara1, "
                                         " fotocamara2 = :fotocamara2  "
                                         " WHERE id =  :id0;";

static const QString qry_fecharegistro = " SELECT fecha "
                                         " FROM registros_matriculas "
                                         " WHERE id = :id ";

static const QString qry_buscarpareja =  " SELECT id,fecha,pesobruto,matriculaA1, matriculaB1 , matriculaA2, matriculaB2 FROM nqtrucks.registros_matriculas"
                                         " WHERE  "                      // FILTROS ESTATICOS //
                                         " !procesado "
                                         " AND(fotocamara1 IS NULL  AND fotocamara2 IS NULL)"
                                         " AND ( "                       // FILTROS DINAMICOS //
                                         " id != :id0 "
                                         " AND "
                                         " DATE(fecha) = DATE(:fechaabuscar) "  //DEBUG curdate()//
                                         " ) "
                                         " AND( "
                                         " matriculaA1 = :matriculabuscar OR "
                                         " matriculaB1 = :matriculabuscar OR "
                                         " matriculaA2 = :matriculabuscar OR "
                                         " matriculaB1 = :matriculabuscar "
                                         " ) "
                                         " ORDER by fecha DESC "
                                         " LIMIT 1; ";

static const QString qry_procesar_pareja =  " UPDATE registros_matriculas "
                                            " SET "
                                            " emparejado = :idpareja, "
                                            " pesoneto   = :pesoneto, "
                                            " procesado  = 1  "
                                            " WHERE id   =  :id0;";


DatabaseManager::DatabaseManager(QObject *parent)
   : QObject(parent)
{
    m_RegistroMatriculas.resize(2);
}

DatabaseManager::~DatabaseManager(){

}

/** DB **/
void DatabaseManager::initDb(){
    qDebug() << "conexion inicial es: " << m_db.connectionName();

    if ( !QSqlDatabase::contains("nqtrucks")) {
        m_db = QSqlDatabase::addDatabase("QMYSQL","nqtrucks");
        m_db.setDatabaseName( "nqtrucks" );
        m_db.setHostName(     "localhost" );
        m_db.setUserName(     "nqtrucks" );
        m_db.setPassword(     "nqtrucks" );
        qDebug() << "conexion al crear es: " << m_db.connectionName();
    } else {
        m_db = QSqlDatabase::database("nqtrucks");
        qDebug() << "conexion al volver a usar es: " << m_db.connectionName();
    }
    qDebug() << "conexion al verificar es: " << m_db.connectionName();
}
/** END DB **/

/** REGISTRO SIMPLE **/
void DatabaseManager::setRegistroMatriculas(const SimpleMatriculas &RegistroMatriculas)
{
    m_RegistroMatriculas[0] = RegistroMatriculas;
}

void DatabaseManager::guardarRegistroSimpleMatriculas(){
    /** TODO First row always corrupt? **/
    initDb();

    if (!m_db.isOpen() && !m_db.open() )
    {
        qDebug() << m_db.lastError();
      qDebug() << "Failed to connect." ;
    }else{
        qDebug( "Connected!" );
        m_db.transaction();
        QSqlQuery qry(m_db);
        qry.prepare(qry_insert_simple);

        qry.bindValue(":pesobruto",         m_RegistroMatriculas[0].bascula.iBruto);
        qry.bindValue(":pesoneto",          m_RegistroMatriculas[0].bascula.iNeto);
        qry.bindValue(":pesotara",          m_RegistroMatriculas[0].bascula.iTara);

        qry.bindValue(":fotocamara1",       m_RegistroMatriculas[0].results[0].camara.OrigenFotoByte);
        qry.bindValue(":fotomatriculaA1",   m_RegistroMatriculas[0].results[0].MatriculaFotoAByte);
        qry.bindValue(":fotomatriculaB1",   m_RegistroMatriculas[0].results[0].MatriculaFotoBByte);
        qry.bindValue(":matriculaA1",       m_RegistroMatriculas[0].results[0].MatriculaA);
        qry.bindValue(":matriculaB1",       m_RegistroMatriculas[0].results[0].MatriculaB);
        qry.bindValue(":precisionA1",       QString::number(m_RegistroMatriculas[0].results[0].MatriculaPrecisionA,'g',6));
        qry.bindValue(":precisionB1",       QString::number(m_RegistroMatriculas[0].results[0].MatriculaPrecisionB,'g',6));


        qry.bindValue(":fotocamara2",       m_RegistroMatriculas[0].results[1].camara.OrigenFotoByte);
        qry.bindValue(":fotomatriculaA2",   m_RegistroMatriculas[0].results[1].MatriculaFotoAByte);
        qry.bindValue(":fotomatriculaB2",   m_RegistroMatriculas[0].results[1].MatriculaFotoBByte);
        qry.bindValue(":matriculaA2",       m_RegistroMatriculas[0].results[1].MatriculaA);
        qry.bindValue(":matriculaB2",       m_RegistroMatriculas[0].results[1].MatriculaB);
        qry.bindValue(":precisionA2",       QString::number(m_RegistroMatriculas[0].results[1].MatriculaPrecisionA,'g',6));
        qry.bindValue(":precisionB2",       QString::number(m_RegistroMatriculas[0].results[1].MatriculaPrecisionB,'g',6));

         if( !qry.exec() ){
           qDebug() << qry.lastError();
            m_db.rollback();
         }else{
            m_db.commit();
            m_RegistroMatriculas[0].id = qry.lastInsertId().toLongLong();
            qDebug() <<  "Inserted!: " << m_RegistroMatriculas[0].id;
           /** Report Pesada **/
            m_report_manager.printRegistroMatricula(m_db,m_RegistroMatriculas[0].id);

            /** No Guardar Si Cualquiera de las 4 Matriculas es detectada Y BUSCAR SU PAREJA**/
            if ( m_RegistroMatriculas[0].results[0].MatriculaPrecisionA >80 || m_RegistroMatriculas[0].results[0].MatriculaPrecisionB >80 ||
                 m_RegistroMatriculas[0].results[1].MatriculaPrecisionA >80 || m_RegistroMatriculas[0].results[1].MatriculaPrecisionB >80  )
            {

                m_db.transaction();
                qry.prepare(  qry_delete_fotos);
                qry.bindValue(":id0",                m_RegistroMatriculas[0].id);
                qry.bindValue(":fotocamara1",       QVariant());
                qry.bindValue(":fotocamara2",       QVariant());

                if (!qry.exec()){
                    qDebug() << qry.lastError();
                }else{
                    m_db.commit();
                    /** Buscar Pareja **/
                        if (encontrarPareja()){
                         /** Si pareja **/                           
                         m_report_manager.printRegistroMatriculaProcesada(m_db,m_RegistroMatriculas[0].id,m_RegistroMatriculas[1].id);
                        }
                    /** END BUSCAR PAREJA **/
                }

            }

         }
        m_db.close();
    }
    emit workFinished();
}
/** END REGISTRO SIMPLE **/


/** BUSCAR PAIR **/
bool DatabaseManager::encontrarPareja()
{
    /** Que fecha final tenemos?**/
    if (getFechaRegistro(0)){
        qDebug() << "Fecha registro Date: " << m_RegistroMatriculas[0].FechaRegistro.date();
        qDebug() << "Fecha registro: " << m_RegistroMatriculas[0].FechaRegistro;


        /** De Cuantas matriculas dispongo para encontrar? **/
        m_MatriculasList.clear();
        if (m_RegistroMatriculas[0].results[0].MatriculaPrecisionA >80){
            m_MatriculasList << m_RegistroMatriculas[0].results[0].MatriculaA;
        }

        if (m_RegistroMatriculas[0].results[0].MatriculaPrecisionB >80){
            m_MatriculasList << m_RegistroMatriculas[0].results[0].MatriculaB;
        }

        if (m_RegistroMatriculas[0].results[1].MatriculaPrecisionA >80){
            m_MatriculasList << m_RegistroMatriculas[0].results[1].MatriculaA;
        }

        if (m_RegistroMatriculas[0].results[1].MatriculaPrecisionB >80){
            m_MatriculasList << m_RegistroMatriculas[0].results[1].MatriculaB;
        }

        /** Tenemos matriculas y fecha,
         * Buscamos por cada matricula que tenemos en esa fecha,
         * hasta encontrar coincidencia **/
        for (QStringList::iterator it =  m_MatriculasList.begin(); it != m_MatriculasList.end(); ++it) {
                QString current = *it;
                qDebug() << "[[" << current << "]]";

                /** consultar bd **/
                if (buscarPareja(0,current)){
                    return true;
                }
            }
    }

    return false;
}

bool DatabaseManager::buscarPareja(const long long &_id, const QString &_matricula){
    /** TODO ENTRE FECHAS **/


    QSqlQuery qry(m_db);
    qry.prepare(qry_buscarpareja);
    qry.bindValue(":id0", m_RegistroMatriculas[_id].id);
    qry.bindValue(":fechaabuscar", m_RegistroMatriculas[_id].FechaRegistro.date());
    qry.bindValue(":matriculabuscar", _matricula);

    if (!qry.exec()) { // make sure your query has been executed successfully
        qDebug() << qry.lastError(); // show the error
    } else {
        /** Si existe la pareja, adquiero su id y el peso bruto **/
        if (qry.first()){
            qDebug() << "Pareja: " << qry.record();
            m_RegistroMatriculas[1].id = qry.value("id").toLongLong();
            //m_RegistroMatriculas[1].bascula.iBruto = qry.value("pesobruto").toFloat();
            m_RegistroMatriculas[1].bascula.iBruto = qry.value("pesobruto").toFloat()+300; //DEBUG
            return actualizarPareja();         /** Actualizo a Procesado y
                                               * consigo el Peso Verificado
                                               * http://www.worldshipping.org/industry-issues/safety/WSC_Summarizes_the_Basic_Elements_of_the_SOLAS_Container_Weight_Verification_Requirement___February_2015.pdf **/
        }
    }
    return false;
}

bool DatabaseManager::actualizarPareja(){

    float neto = abs(m_RegistroMatriculas[0].bascula.iBruto - m_RegistroMatriculas[1].bascula.iBruto);
    m_RegistroMatriculas[0].bascula.iNeto = neto;
    m_RegistroMatriculas[1].bascula.iNeto = neto;

    QSqlQuery qry(m_db);
    qry.prepare(qry_procesar_pareja);
    /** Primera pareja 0 **/
    m_db.transaction();
    qry.bindValue(":idpareja", m_RegistroMatriculas[1].id);
    qry.bindValue(":pesoneto", m_RegistroMatriculas[0].bascula.iNeto);
    qry.bindValue(":id0", m_RegistroMatriculas[0].id);
    if (!qry.exec()){
        qDebug() << qry.lastError();
        return false;
    }else{
        m_db.commit();
    }

    /** Segunda pareja 0 **/
    m_db.transaction();
    qry.bindValue(":idpareja", m_RegistroMatriculas[0].id);
    qry.bindValue(":pesoneto", m_RegistroMatriculas[1].bascula.iNeto);
    qry.bindValue(":id0", m_RegistroMatriculas[1].id);
    if (!qry.exec()){
        qDebug() << qry.lastError();
        return false;
    }else{
        m_db.commit();
    }



    return true;
}


bool DatabaseManager::getFechaRegistro(const int &_id){
    QSqlQuery qry(m_db);
    qry.prepare(  qry_fecharegistro);
    qry.bindValue(":id", m_RegistroMatriculas[_id].id);
    if (!qry.exec()) { // make sure your query has been executed successfully
        qDebug() << qry.lastError(); // show the error
    } else {
        while (qry.next()) {
             m_RegistroMatriculas[_id].FechaRegistro =  qry.value("fecha").toDateTime();
            return true;
        }
    }

    return false;
}




}
}
