#include "RegistroPeso.h"
#include <QDebug>

namespace nQTrucks{
    namespace Maestros{

    static const QString qry_fecharegistro =    " SELECT fecha  FROM registros_matriculas WHERE id = :id ";

    static const QString qry_insert_simple =    " INSERT INTO registros_matriculas( "
                                                " pesobruto,  pesoneto,  pesotara,  "
                                                " fotocamara1, fotomatriculaA1, fotomatriculaB1, matriculaA1,  matriculaB1, precisionA1, precisionB1,"
                                                " fotocamara2, fotomatriculaA2, fotomatriculaB2, matriculaA2,  matriculaB2, precisionA2, precisionB2 "
                                                " ) "
                                                " VALUES  ("
                                                " :pesobruto,   :pesoneto,  :pesotara, "
                                                " :fotocamara1, :fotomatriculaA1, :fotomatriculaB1, :matriculaA1,  :matriculaB1, :precisionA1, :precisionB1,"
                                                " :fotocamara2, :fotomatriculaA2, :fotomatriculaB2, :matriculaA2,  :matriculaB2, :precisionA2, :precisionB2 "
                                                " );";

    static const QString qry_delete_fotos =     " UPDATE registros_matriculas SET fotocamara1 = :fotocamara1, fotocamara2 = :fotocamara2 WHERE id =  :id0;";

    static const QString qry_buscarpareja =     " SELECT id,fecha,pesobruto,matriculaA1, matriculaB1 , matriculaA2, matriculaB2 FROM nqtrucks.registros_matriculas"
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

    static const QString qry_procesar_pareja =  " UPDATE registros_matriculas  SET emparejado = :idpareja, pesoneto   = :pesoneto, procesado  = 1  WHERE id   =  :id0;";

    static const QString qry_fecha_min_max   =  " SELECT MIN(DATE(fecha) as fechamin, MAX(DATE(fecha) as fechamax FROM nqtrucks.registros_matriculas; ";




        RegistroPeso::RegistroPeso(QObject *parent)
            : QueryModel(parent)
        {
            configQueries();
            setTable();
        }


        void RegistroPeso::configQueries(){
            m_DefaultQuery = "select * from registros_matriculas ;" ;//+ QString(appServer_tablename);
        }

        void RegistroPeso::setFecha_min_max()
        {
            QSqlQuery qry(m_db);
            qry.prepare(qry_fecha_min_max);
            if(qry.exec()){
                while (qry.next()) {
                    m_fecha_min_max[0] =  qry.value("fechamin").toDate();
                    m_fecha_min_max[1] =  qry.value("fechamax").toDate();
                    emit rangoFechasChanged(m_fecha_min_max);
                }
            }

        }


        void RegistroPeso::setTable(){
            beginResetModel();
            setFecha_min_max();
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
                while (qry.next()) {
                    _fecha =  qry.value("fecha").toDateTime();
                    return _fecha;
                }
            }
            return _fecha;
        }

        bool RegistroPeso::buscarPareja(QVector<SimpleMatriculas> &RegistrosMatriculas, const QString &_matricula){
            /** TODO ENTRE FECHAS **/
             qDebug() << "BUSCANDO:  [[" << _matricula << "]]";
            QSqlQuery qry(m_db);
            qry.prepare(qry_buscarpareja);
            qry.bindValue(":id0", RegistrosMatriculas[0].id);
            qry.bindValue(":fechaabuscar", RegistrosMatriculas[0].FechaRegistro.date());
            qry.bindValue(":matriculabuscar", _matricula);

            if (qry.exec()) { // make sure your query has been executed successfully
                /** Si existe la pareja, adquiero su id y el peso bruto **/
                while (qry.next()){
                    RegistrosMatriculas[1].id = qry.value("id").toLongLong();
                    RegistrosMatriculas[1].bascula.iBruto = qry.value("pesobruto").toFloat()+300; //DEBUG
                    return actualizarPareja(RegistrosMatriculas);
                    /** Actualizo a Procesado y
                    * consigo el Peso Verificado
                    * http://www.worldshipping.org/industry-issues/safety/WSC_Summarizes_the_Basic_Elements_of_the_SOLAS_Container_Weight_Verification_Requirement___February_2015.pdf
                    **/
                }
            }
            return false;
        }

        bool RegistroPeso::actualizarPareja(QVector<SimpleMatriculas> &RegistrosMatriculas)
        {
            float neto = abs(RegistrosMatriculas[0].bascula.iBruto - RegistrosMatriculas[1].bascula.iBruto);
            RegistrosMatriculas[0].bascula.iNeto = neto;
            RegistrosMatriculas[1].bascula.iNeto = neto;
            qDebug() << "Actualizando Parejas";
            QSqlQuery qry(m_db);
            QSqlQuery qry2(m_db);
            qry.prepare(qry_procesar_pareja);
            qry2.prepare(qry_procesar_pareja);

            /** Primera pareja 0 **/
            qry.bindValue(":idpareja", RegistrosMatriculas[1].id);
            qry.bindValue(":pesoneto", RegistrosMatriculas[0].bascula.iNeto);
            qry.bindValue(":id0", RegistrosMatriculas[0].id);
            if (qry.exec()){
                qDebug() << "actualizando pareja 1";
                /** Segunda pareja 1 **/
                qry2.bindValue(":idpareja", RegistrosMatriculas[0].id);
                qry2.bindValue(":pesoneto", RegistrosMatriculas[1].bascula.iNeto);
                qry2.bindValue(":id0",      RegistrosMatriculas[1].id);
                if (qry2.exec()){
                    qDebug() << "actualizando pareja 2";
                    return true;
                }
            }
            return false;
        }





    } /** End namespace Maestros **/
} /** End namespace nQTrucks **/


