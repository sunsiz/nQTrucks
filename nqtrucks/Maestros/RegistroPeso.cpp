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

    static const QString qry_procesar_pareja =  " UPDATE registros_matriculas  SET emparejado = :idpareja, pesoneto = :pesoneto, procesado  = 1, entrada =:entrada  WHERE id   =  :id0;";

    static const QString qry_fecha_min_max   =  " SELECT MIN(DATE(fecha) as fechamin, MAX(DATE(fecha) as fechamax FROM nqtrucks.registros_matriculas; ";




        RegistroPeso::RegistroPeso(QObject *parent)
            : QueryModel(parent)
        {
            configQueries();
            setTable();
        }


        void RegistroPeso::configQueries(){
            m_DefaultQuery = "select * from registros_matriculas ;" ;//+ QString(appServer_tablename);
            //m_DefaultQuery = "SELECT id ,fecha, pesobruto, pesoneto , matriculaA1, matriculaB1, matriculaA2, matriculaB2, procesado, emparejado, entrada from registros_matriculas;";
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

        bool RegistroPeso::guardarRegistroRegistroMatriculas(RegistroMatriculas *_RegistroMatriculas){

            QSqlQuery qry(m_db);
            Tools *m_tools = new Tools;
            qry.prepare(qry_insert_simple);
            qry.bindValue(":pesobruto",                                         _RegistroMatriculas->m_bascula->getIBruto()                        );
            qry.bindValue(":pesoneto",                                          _RegistroMatriculas->m_bascula->getINeto()                         );
            qry.bindValue(":pesotara",                                          _RegistroMatriculas->m_bascula->getITara()                         );

            qry.bindValue(":fotocamara1",       m_tools->convertMat2ByteArray(  _RegistroMatriculas->m_results0->camara->getOrigenFoto())          );
            qry.bindValue(":fotomatriculaA1",   m_tools->convertMat2ByteArray(  _RegistroMatriculas->m_results0->getMatriculaFotoA())              );
            qry.bindValue(":fotomatriculaB1",   m_tools->convertMat2ByteArray(  _RegistroMatriculas->m_results0->getMatriculaFotoB())              );
            qry.bindValue(":matriculaA1",                                       _RegistroMatriculas->m_results0->getMatriculaA()                   );
            qry.bindValue(":matriculaB1",                                       _RegistroMatriculas->m_results0->getMatriculaB()                   );
            qry.bindValue(":precisionA1",       QString::number(                _RegistroMatriculas->m_results0->getMatriculaPrecisionA(),'f',2)   );
            qry.bindValue(":precisionB1",       QString::number(                _RegistroMatriculas->m_results0->getMatriculaPrecisionB(),'f',2)   );


            qry.bindValue(":fotocamara2",       m_tools->convertMat2ByteArray(  _RegistroMatriculas->m_results1->camara->getOrigenFoto())          );
            qry.bindValue(":fotomatriculaA2",   m_tools->convertMat2ByteArray(  _RegistroMatriculas->m_results1->getMatriculaFotoA())              );
            qry.bindValue(":fotomatriculaB2",   m_tools->convertMat2ByteArray(  _RegistroMatriculas->m_results1->getMatriculaFotoB())              );
            qry.bindValue(":matriculaA2",                                       _RegistroMatriculas->m_results1->getMatriculaA()                   );
            qry.bindValue(":matriculaB2",                                       _RegistroMatriculas->m_results1->getMatriculaB()                   );
            qry.bindValue(":precisionA2",       QString::number(                _RegistroMatriculas->m_results1->getMatriculaPrecisionA(),'f',2)   );
            qry.bindValue(":precisionB2",       QString::number(                _RegistroMatriculas->m_results1->getMatriculaPrecisionB(),'f',2)   );
            m_tools->deleteLater();

            if(qry.exec()){ /** MEMORY LEAK **/
                _RegistroMatriculas->setId(qry.lastInsertId().toLongLong());
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

        bool RegistroPeso::buscarPareja(QVector<RegistroMatriculas*> _RegistrosMatriculas, const QString &_matricula){
            /** TODO ENTRE FECHAS **/
             qDebug() << "BUSCANDO:  [[" << _matricula << "]]";
            QSqlQuery qry(m_db);
            qry.prepare(qry_buscarpareja);
            qry.bindValue(":id0", _RegistrosMatriculas[0]->getId());
            qry.bindValue(":fechaabuscar", _RegistrosMatriculas[0]->getFechaRegistro().date());
            qry.bindValue(":matriculabuscar", _matricula);

            if (qry.exec()) { /** MEMORY LEAK **/ // make sure your query has been executed successfully
                /** Si existe la pareja, adquiero su id y el peso bruto **/
                while (qry.next()){
                    _RegistrosMatriculas[1]->setId(qry.value("id").toLongLong());
                    _RegistrosMatriculas[1]->m_bascula->setIBruto(qry.value("pesobruto").toFloat()); //DEBUG
                    //_RegistrosMatriculas[1]->m_bascula->setIBruto(qry.value("pesobruto").toFloat()+300); //DEBUG
                    return actualizarPareja(_RegistrosMatriculas);
                    /** Actualizo a Procesado y
                    * consigo el Peso Verificado
                    * http://www.worldshipping.org/industry-issues/safety/WSC_Summarizes_the_Basic_Elements_of_the_SOLAS_Container_Weight_Verification_Requirement___February_2015.pdf
                    **/
                }
            }
            return false;
        }

        bool RegistroPeso::actualizarPareja(QVector<RegistroMatriculas *> _RegistrosMatriculas)
        {
            float neto = fabs(_RegistrosMatriculas[0]->m_bascula->getIBruto() - _RegistrosMatriculas[1]->m_bascula->getIBruto());
            _RegistrosMatriculas[0]->m_bascula->setINeto(neto);
            _RegistrosMatriculas[1]->m_bascula->setINeto(neto);
            qDebug() << "Actualizando Parejas";
            QSqlQuery qry(m_db);
            QSqlQuery qry2(m_db);
            qry.prepare(qry_procesar_pareja);
            qry2.prepare(qry_procesar_pareja);

            /** Primera pareja 0 **/
            qry.bindValue(":idpareja", _RegistrosMatriculas[1]->getId());
            qry.bindValue(":pesoneto", _RegistrosMatriculas[0]->m_bascula->getINeto());
            qry.bindValue(":id0",      _RegistrosMatriculas[0]->getId());
            qry.bindValue(":entrada",  false);
            if (qry.exec()){
                qDebug() << "actualizando pareja 1";
                /** Segunda pareja 1 **/
                qry2.bindValue(":idpareja", _RegistrosMatriculas[0]->getId());
                qry2.bindValue(":pesoneto", _RegistrosMatriculas[1]->m_bascula->getINeto());
                qry2.bindValue(":id0",      _RegistrosMatriculas[1]->getId());
                qry2.bindValue(":entrada",  true);
                if (qry2.exec()){
                    qDebug() << "actualizando pareja 2";
                    return true;
                }
            }
            return false;
        }





    } /** End namespace Maestros **/
} /** End namespace nQTrucks **/


