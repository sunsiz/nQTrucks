#include "DatabaseManager.h"
#include <QtSql>
#include <QDebug>

namespace nQTrucks{
namespace Db{

static const QString qry_insert_simple =  "INSERT INTO registros_matriculas( "
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
                                         " WHERE id =  :id;";

static const QString qry_fecharegistro = " SELECT fecha "
                                         " FROM registros_matriculas "
                                         " WHERE id = :id ";


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
                qry.bindValue(":id",                m_RegistroMatriculas[0].id);
                qry.bindValue(":fotocamara1",       QVariant());
                qry.bindValue(":fotocamara2",       QVariant());

                if (!qry.exec()){
                    qDebug() << qry.lastError();
                }else{
                    m_db.commit();
                    /** Buscar Pareja **/
                        if (encontrarPareja()){
                         /** Si pareja **/
                         /** TODO Report Pareja **/

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
        //m_RegistroMatriculas[0].FechaRegistro = getFechaRegistro(m_RegistroMatriculas[0].id);
        qDebug() << "registro: " << m_RegistroMatriculas[0].FechaRegistro;


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
        for (QStringList::iterator it =  m_MatriculasList.begin(); it != list.end(); ++it) {
                QString current = *it;
                qDebug() << "[[" << current << "]]";

                /** consultar bd **/



            }



    }

    /* Busqueda de pareja hacia atras
    SET @id1=452;
    SET @fechaactual=curdate();
    SET @fechaactual=DATE('2016-05-31 17:08:22');
    SET @matriculabuscar = 'R0002BCJ';


    SELECT id,fecha,matriculaA1, matriculaB1 , matriculaA2, matriculaB2 FROM nqtrucks.registros_matriculas
    WHERE
    // FILTROS ESTATICOS //
    !procesado
    AND(fotocamara1 IS NULL  AND fotocamara2 IS NULL)

    // FILTROS DINAMICOS //
    AND(
        id != @id1
        AND
        DATE(fecha) = @fechaactual //DEBUG curdate()//
    )
    AND(
        matriculaA1 = @matriculabuscar OR
        matriculaB1 = @matriculabuscar OR
        matriculaA2 = @matriculabuscar OR
        matriculaB1 = @matriculabuscar
    )

    ORDER by Fecha DESC

    LIMIT 1;
    */

    return false;
}

bool DatabaseManager::getFechaRegistro(const int &_id){

    QSqlQuery qry(m_db);
    qry.prepare(  qry_fecharegistro);
    qry.bindValue(":id", m_RegistroMatriculas[_id].id);

    if (!qry.exec()) { // make sure your query has been executed successfully
        qDebug() << qry.lastError(); // show the error
    } else {
        while (qry.next()) {
            //qDebug() << qry.value("fecha");
             m_RegistroMatriculas[_id].FechaRegistro =  qry.value("fecha").toDateTime();
            return true;
        }
    }

    return false;
}




}
}
