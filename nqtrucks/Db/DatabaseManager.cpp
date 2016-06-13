#include "DatabaseManager.h"
#include <QtSql>
#include <QDebug>

namespace nQTrucks{
namespace Db{

//DatabaseManager::DatabaseManager(QObject *parent)
//    : QObject(parent)
//{

//}

DatabaseManager::DatabaseManager(QObject *parent)
   : QObject(parent)
{

}

DatabaseManager::~DatabaseManager(){

}

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

void DatabaseManager::setRegistroMatriculas(const SimpleMatriculas &RegistroMatriculas)
{
    m_RegistroMatriculas = RegistroMatriculas;
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

        qry.bindValue(":pesobruto",         m_RegistroMatriculas.bascula.iBruto);
        qry.bindValue(":pesoneto",          m_RegistroMatriculas.bascula.iNeto);
        qry.bindValue(":pesotara",          m_RegistroMatriculas.bascula.iTara);

        qry.bindValue(":fotocamara1",       m_RegistroMatriculas.results[0].camara.OrigenFotoByte);
        qry.bindValue(":fotomatriculaA1",   m_RegistroMatriculas.results[0].MatriculaFotoAByte);
        qry.bindValue(":fotomatriculaB1",   m_RegistroMatriculas.results[0].MatriculaFotoBByte);
        qry.bindValue(":matriculaA1",       m_RegistroMatriculas.results[0].MatriculaA);
        qry.bindValue(":matriculaB1",       m_RegistroMatriculas.results[0].MatriculaB);
        qry.bindValue(":precisionA1",       QString::number(m_RegistroMatriculas.results[0].MatriculaPrecisionA,'g',6));
        qry.bindValue(":precisionB1",       QString::number(m_RegistroMatriculas.results[0].MatriculaPrecisionB,'g',6));


        qry.bindValue(":fotocamara2",       m_RegistroMatriculas.results[1].camara.OrigenFotoByte);
        qry.bindValue(":fotomatriculaA2",   m_RegistroMatriculas.results[1].MatriculaFotoAByte);
        qry.bindValue(":fotomatriculaB2",   m_RegistroMatriculas.results[1].MatriculaFotoBByte);
        qry.bindValue(":matriculaA2",       m_RegistroMatriculas.results[1].MatriculaA);
        qry.bindValue(":matriculaB2",       m_RegistroMatriculas.results[1].MatriculaB);
        qry.bindValue(":precisionA2",       QString::number(m_RegistroMatriculas.results[1].MatriculaPrecisionA,'g',6));
        qry.bindValue(":precisionB2",       QString::number(m_RegistroMatriculas.results[1].MatriculaPrecisionB,'g',6));

         if( !qry.exec() ){
           qDebug() << qry.lastError();
            m_db.rollback();
         }else{
            m_db.commit();
           qDebug() <<  "Inserted!: " << qry.lastInsertId().toInt();
            m_lastrecord = qry.lastInsertId().toLongLong();
           /** Report Pesada **/
            m_report_manager.printRegistroMatricula(m_db,m_lastrecord);

            /** No Guardar Si Cualquiera de las 4 Matriculas es detectada **/
            if ( m_RegistroMatriculas.results[0].MatriculaPrecisionA >80 || m_RegistroMatriculas.results[0].MatriculaPrecisionB >80 ||
                 m_RegistroMatriculas.results[1].MatriculaPrecisionA >80 || m_RegistroMatriculas.results[1].MatriculaPrecisionB >80  )
            {

                m_db.transaction();
                qry.prepare(" UPDATE registros_matriculas "
                            " SET "
                            " fotocamara1 = :fotocamara1, "
                            " fotocamara2 = :fotocamara2  "
                            " WHERE id =  :id;");
                qry.bindValue(":id",                m_lastrecord);
                qry.bindValue(":fotocamara1",       QVariant());
                qry.bindValue(":fotocamara2",       QVariant());

                if (!qry.exec()){
                    qDebug() << qry.lastError();
                }else{
                    m_db.commit();
                    /** Buscar Pareja **/
                         /** Si pareja **/
                             /** Report Pareja **/
                }

            }







         }
        m_db.close();

    }
    emit workFinished();
}


}
}
