#include "DatabaseManager.h"
#include <QtSql>
#include <QDebug>

namespace nQTrucks{
namespace Db{

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

void DatabaseManager::guardarRegistroSimpleMatriculas(){
    /** TODO First row always corrupt? **/
    initDb();

    if (!m_db.isOpen() && !m_db.open() )
    {
      qDebug() << m_db.lastError();
      qDebug() << "Failed to connect." ;
    }else{
        qDebug( "Connected!" );
        /** No Guardar Si Cualquiera de las 4 Matriculas es detectada **/
        if ( m_registro_simple_matriculas.precisionA1>80 ||
             m_registro_simple_matriculas.precisionB1>80 ||
             m_registro_simple_matriculas.precisionA2>80 ||
             m_registro_simple_matriculas.precisionB2>80 )
        {
            m_registro_simple_matriculas.registrosimple.camara1.clear();
            m_registro_simple_matriculas.registrosimple.camara1resize.clear();
            m_registro_simple_matriculas.registrosimple.camara2.clear();
            m_registro_simple_matriculas.registrosimple.camara2resize.clear();
        }

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

        qry.bindValue(":pesobruto",         m_registro_simple_matriculas.registrosimple.bascula.iBruto);
        qry.bindValue(":pesoneto",          m_registro_simple_matriculas.registrosimple.bascula.iNeto);
        qry.bindValue(":pesotara",          m_registro_simple_matriculas.registrosimple.bascula.iTara);

        qry.bindValue(":fotocamara1",       m_registro_simple_matriculas.registrosimple.camara1resize);
        qry.bindValue(":fotomatriculaA1",   m_registro_simple_matriculas.fotomatriculaA1);
        qry.bindValue(":fotomatriculaB1",   m_registro_simple_matriculas.fotomatriculaB1);
        qry.bindValue(":matriculaA1",       m_registro_simple_matriculas.matriculaA1);
        qry.bindValue(":matriculaB1",       m_registro_simple_matriculas.matriculaB1);
        qry.bindValue(":precisionA1",       QString::number(m_registro_simple_matriculas.precisionA1,'g',6));
        qry.bindValue(":precisionB1",       QString::number(m_registro_simple_matriculas.precisionB1,'g',6));


        qry.bindValue(":fotocamara2",       m_registro_simple_matriculas.registrosimple.camara2resize);
        qry.bindValue(":fotomatriculaA2",   m_registro_simple_matriculas.fotomatriculaA2);
        qry.bindValue(":fotomatriculaB2",   m_registro_simple_matriculas.fotomatriculaB2);
        qry.bindValue(":matriculaA2",       m_registro_simple_matriculas.matriculaA2);
        qry.bindValue(":matriculaB2",       m_registro_simple_matriculas.matriculaB2);
        qry.bindValue(":precisionA2",       QString::number(m_registro_simple_matriculas.precisionA2,'g',6));
        qry.bindValue(":precisionB2",       QString::number(m_registro_simple_matriculas.precisionB2,'g',6));

         if( !qry.exec() ){
           qDebug() << qry.lastError();
            m_db.rollback();
         }else{
            m_db.commit();

           qDebug() <<  "Inserted!: " << qry.lastInsertId().toInt();
            int lastrow = qry.lastInsertId().toInt();
           /** Report Pesada **/
            m_report_manager.printRegistroMatricula(lastrow);


           /** Buscar Pareja **/
                /** Si pareja **/
                    /** Report Pareja **/


         }
        m_db.close();

    }
    emit workFinished();
}

void DatabaseManager::setRegistroSimpleMatriculas(const Registros::SimpleMatriculas &_registro){
    m_registro_simple_matriculas = _registro;
}


}
}
