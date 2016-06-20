#include "QueryModel.h"


//--------------------------------
// SQL QUERY MODEL
//
// DOES SOME FUN STUFF TO DISPLAY STUFF IN QML
//--------------------------------

#include <QSqlQuery>

#include <QDebug>


namespace nQTrucks {
    namespace Db {

        QueryModel::QueryModel(QObject *parent):
            QSqlQueryModel(parent)
        {
            qRegisterMetaType<QueryModel*>("QueryModel*");

        }

        QueryModel::~QueryModel()
        {
            if (m_db.isOpen()){
                m_db.close();
            }
        }

        /*  REIMPLEMENT QSqlQueryModel
         *******************************/
        QHash<int, QByteArray> QueryModel::roleNames() const{
            QHash<int, QByteArray> roles;
            for ( int i = 0; i < this->record().count(); i++)
            {
                roles[Qt::UserRole + i + 1] = this->record().fieldName(i).toLocal8Bit();
            }
            return roles;
        }

        QVariant QueryModel::data(const QModelIndex &index, int role) const{
            QVariant value = QSqlQueryModel::data(index, role);
            if (role < Qt::UserRole)
            {
                value = QSqlQueryModel::data(index, role);
            }
            else {
                int row = index.row();
                int col = role - Qt::UserRole - 1;
                QModelIndex modelIndex = this->index(row, col);
                value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
            }
            return value;
        }


        //Consulta default
        bool QueryModel::setQuery(const QString &query)//,const QSqlDatabase &_db)
        {
            QSqlQueryModel::setQuery(query, m_db);
            if (this->query().record().isEmpty())
            {
                //qWarning() << "QueryModel::setQuery() - " << this->query().lastError();
                return false;
            }
            return true;
        }

        bool QueryModel::setCustomView(const QString &query){
            QSqlQueryModel::setQuery(query, m_db);
            if (this->query().record().isEmpty())
            {
                //qWarning() << "QueryModel::setQuery() - " << this->query().lastError();
                return false;
            }
            return true;
        }

        /* END REIMPLEMENT QSqlQueryModel
         *******************************/


        //DATABASE MANAGER
        bool QueryModel::initDB()
        {
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
            if ( !m_db.isOpen() )
            {
                if (!m_db.open()) {
                    qWarning() << "Unable to connect to database, giving up:" << m_db.lastError();
                    return false;
                }
            }
            return true;
        }

        // Ejecutar Consultas: Create drop select,Insert, etc.
        bool QueryModel::execQuery(const QByteArray &_query)
        {
            bool ret = false;
            if (initDB()) {
                QSqlQuery query(m_db);
                ret = query.exec(_query);
                //query.clear();
            }

            if (!ret){
                //qWarning() << "QueryModel::execQuery() -" << db.lastError();
            }
            return ret;
        }


        // debug Errores DB
        QSqlError QueryModel::lastError()//const QSqlDatabase &db)
        {
            return m_db.lastError();
        }







    } //name space Db
} //name space nQtrucks
