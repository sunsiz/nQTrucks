#include "RegistroPeso.h"
#include <QDebug>

namespace nQTrucks{
    namespace Maestros{

    static const QString qry_fecharegistro = " SELECT fecha "
                                             " FROM registros_matriculas "
                                             " WHERE id = :id ";

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


