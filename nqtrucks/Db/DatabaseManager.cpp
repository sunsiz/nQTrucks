#include "DatabaseManager.h"
#include <QtSql>
#include <QDebug>

namespace nQTrucks{
namespace Db{


DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{

}

void DatabaseManager::guardarRegistroSimpleMatriculas()
{
    if( !db.open() )
    {
      qDebug() << db.lastError();
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

        db.transaction();
        QSqlQuery qry(QSqlDatabase::database("registro_matriculas"));
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
            db.rollback();
         }else{
            db.commit();
           qDebug() <<  "Inserted!: " << qry.lastInsertId().toInt();
         }
        db.close();
    }
    emit workFinished();
}

void DatabaseManager::setRegistroSimpleMatriculas(const Registro_Simple_Matriculas &_registro)
{
    m_registro_simple_matriculas = _registro;
    if (!QSqlDatabase::contains("registro_matriculas")){
        /*TODO SETTINGS AND CLASS */
        db = QSqlDatabase::addDatabase( "QMYSQL","registro_matriculas" );
        db.setHostName( "localhost" );
        db.setDatabaseName( "nqtrucks" );
        db.setUserName( "nqtrucks" );
        db.setPassword( "nqtrucks" );
    }
}


}
}
