#include "RegistroPeso.h"
#include <QDebug>

namespace nQTrucks{
    namespace Maestros{

    static const QString qry_fecharegistro =  " SELECT fecha  FROM registros_matriculas WHERE id = :id ";

    static const QString qry_insert_simple =  " INSERT INTO registros_matriculas( "
                                              " pesobruto,  pesoneto,  pesotara,  "
                                              " fotocamara1, fotomatriculaA1, fotomatriculaB1, matriculaA1,  matriculaB1, precisionA1, precisionB1,"
                                              " fotocamara2, fotomatriculaA2, fotomatriculaB2, matriculaA2,  matriculaB2, precisionA2, precisionB2 "
                                              " ) "
                                              " VALUES  ("
                                              " :pesobruto,   :pesoneto,  :pesotara, "
                                              " :fotocamara1, :fotomatriculaA1, :fotomatriculaB1, :matriculaA1,  :matriculaB1, :precisionA1, :precisionB1,"
                                              " :fotocamara2, :fotomatriculaA2, :fotomatriculaB2, :matriculaA2,  :matriculaB2, :precisionA2, :precisionB2 "
                                              " );";

    static const QString qry_delete_fotos =   " UPDATE registros_matriculas "
                                              " SET "
                                              " fotocamara1 = :fotocamara1, "
                                              " fotocamara2 = :fotocamara2  "
                                              " WHERE id =  :id0;";


        RegistroPeso::RegistroPeso(QObject *parent)
            : QueryModel(parent)
        {
            configQueries();
            setTable();
        }


        void RegistroPeso::configQueries(){
            m_DefaultQuery = "select * from registros_matriculas ;" ;//+ QString(appServer_tablename);
        }


        void RegistroPeso::setTable(){
            beginResetModel();
            if(initDB()){

                if (setQuery(m_DefaultQuery)){
                    while(canFetchMore() ) {
                        fetchMore();
                    }
                }
            }
            endResetModel();
        }

        bool RegistroPeso::guardarRegistroSimpleMatriculas(SimpleMatriculas &RegistroMatriculas){
                QSqlQuery qry(m_db);
                qry.prepare(qry_insert_simple);
                qry.bindValue(":pesobruto",         RegistroMatriculas.bascula.iBruto);
                qry.bindValue(":pesoneto",          RegistroMatriculas.bascula.iNeto);
                qry.bindValue(":pesotara",          RegistroMatriculas.bascula.iTara);

                qry.bindValue(":fotocamara1",       RegistroMatriculas.results[0].camara.OrigenFotoByte);
                qry.bindValue(":fotomatriculaA1",   RegistroMatriculas.results[0].MatriculaFotoAByte);
                qry.bindValue(":fotomatriculaB1",   RegistroMatriculas.results[0].MatriculaFotoBByte);
                qry.bindValue(":matriculaA1",       RegistroMatriculas.results[0].MatriculaA);
                qry.bindValue(":matriculaB1",       RegistroMatriculas.results[0].MatriculaB);
                qry.bindValue(":precisionA1",       QString::number(RegistroMatriculas.results[0].MatriculaPrecisionA,'g',6));
                qry.bindValue(":precisionB1",       QString::number(RegistroMatriculas.results[0].MatriculaPrecisionB,'g',6));


                qry.bindValue(":fotocamara2",       RegistroMatriculas.results[1].camara.OrigenFotoByte);
                qry.bindValue(":fotomatriculaA2",   RegistroMatriculas.results[1].MatriculaFotoAByte);
                qry.bindValue(":fotomatriculaB2",   RegistroMatriculas.results[1].MatriculaFotoBByte);
                qry.bindValue(":matriculaA2",       RegistroMatriculas.results[1].MatriculaA);
                qry.bindValue(":matriculaB2",       RegistroMatriculas.results[1].MatriculaB);
                qry.bindValue(":precisionA2",       QString::number(RegistroMatriculas.results[1].MatriculaPrecisionA,'g',6));
                qry.bindValue(":precisionB2",       QString::number(RegistroMatriculas.results[1].MatriculaPrecisionB,'g',6));

                if(qry.exec()){
                    RegistroMatriculas.id = qry.lastInsertId().toLongLong();
                    setTable();
                }else{
                    return false;
                }
                return true;

        }
        bool RegistroPeso::eliminaFotosCamara(const long long &_id){
            QSqlQuery qry2(m_db);
            qry2.prepare(  qry_delete_fotos);
            qry2.bindValue(":id0",                _id);
            qry2.bindValue(":fotocamara1",       QVariant());
            qry2.bindValue(":fotocamara2",       QVariant());

            if(qry2.exec()){
                setTable();
                return true;
            }
            return false;
        }

        QDateTime RegistroPeso::getFechaRegistro(const long long &_id)
        {
            QSqlQuery qry(m_db);
            QDateTime _fecha;
            qry.prepare(  qry_fecharegistro);
            qry.bindValue(":id", _id);
            if (!qry.exec()) { // make sure your query has been executed successfully
                qDebug() << qry.lastError(); // show the error
            } else {
                while (qry.first()) {
                    _fecha =  qry.value("fecha").toDateTime();
                }
            }
            return _fecha;
        }





    } /** End namespace Maestros **/
} /** End namespace nQTrucks **/


