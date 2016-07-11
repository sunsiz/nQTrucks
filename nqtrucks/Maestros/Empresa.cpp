#include "Empresa.h"
#include <QDebug>

namespace nQTrucks{
    namespace Maestros{

    static const QString qry_empresa_insert_or_update =    " INSERT INTO empresa"
                                                "         ( id,   razon,  nif,  direccion1,  direccion2,  direccion3,  certificado,  enac ) "
                                                " VALUES  ( 1,         :razon, :nif, :direccion1, :direccion2, :direccion3, :certificado, :enac ) "
                                                " ON DUPLICATE KEY UPDATE "
                                                " razon       = :razon, "
                                                " nif         = :nif, "
                                                " direccion1  = :direccion1, "
                                                " direccion2  = :direccion2, "
                                                " direccion3  = :direccion3, "
                                                " certificado = :certificado, "
                                                " enac        = :enac "
                                                ";";

        Empresa::Empresa(QObject *parent)
            : QueryModel(parent){
            configQueries();
            setTable();
        }


        void Empresa::configQueries(){
            m_DefaultQuery = "select * from empresa ;" ;//+ QString(appServer_tablename);
        }



        void Empresa::setTable(){
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

        void Empresa::actualizarEmpresa(const QVector<QString> &_empresaVector){
                QSqlQuery qry(m_db);
                qry.prepare(qry_empresa_insert_or_update);

                qry.bindValue(":razon",        _empresaVector[0]);
                qry.bindValue(":nif",          _empresaVector[1]);
                qry.bindValue(":direccion1",   _empresaVector[2]);
                qry.bindValue(":direccion2",   _empresaVector[3]);
                qry.bindValue(":direccion3",   _empresaVector[4]);
                qry.bindValue(":certificado",  _empresaVector[5]);
                qry.bindValue(":enac",         _empresaVector[6]);


                if(qry.exec()){ setTable();}

        }

    } /** End namespace Maestros **/
} /** End namespace nQTrucks **/


