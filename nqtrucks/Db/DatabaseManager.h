#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <nqtglobal.h>

#include <QSqlDatabase>
//#include <QSqlQuery>

#include "ReportManager.h"

namespace nQTrucks {
namespace Db {

class DatabaseManager : public QObject{
    Q_OBJECT
public:
   // DatabaseManager(QObject *parent=nullptr);
    explicit DatabaseManager(QObject *parent=nullptr);
    ~DatabaseManager();

signals:
    void rowsPesoChanged();
    void workFinished();

    /** DB **/
private:
    bool initDb();
    QSqlDatabase m_db;
    //QSqlQuery *qry;
    QVector<SimpleMatriculas> m_RegistroMatriculas;
    ReportManager m_report_manager;

/** Registro Simple **/
public:
    void setRegistroMatriculas(const SimpleMatriculas &RegistroMatriculas);
public slots:    
    void guardarRegistroSimpleMatriculas();
private slots:
    void commit_and_inform();

/** Busqueda de PAIR **/
private:
    //QVariant m_FechaRegistro;
    QStringList m_MatriculasList;
    bool getFechaRegistro(const int &_id);
    bool encontrarPareja();
    bool buscarPareja(const long long &_id, const QString &_matricula);
    bool actualizarPareja();
    /** END DB **/
};

}
}
#endif // DATABASEMANAGER_H
