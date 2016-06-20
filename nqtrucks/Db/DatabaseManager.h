#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <nqtglobal.h>

#include <QSqlDatabase>
//#include <QSqlQuery>

#include "Maestros/Maestros.h"

#include "ReportManager.h"

namespace nQTrucks {
namespace Db {

class DatabaseManager : public QObject{
    Q_OBJECT
public:
   // DatabaseManager(QObject *parent=nullptr);
    explicit DatabaseManager(Maestros::Maestros *_maestros=nullptr, QObject *parent=nullptr);
    ~DatabaseManager();

signals:
    void rowsPesoChanged();
    void workFinished();

    /** DB **/
private:
    Maestros::Maestros *m_maestros;
    QVector<SimpleMatriculas> m_RegistroMatriculas;
    ReportManager m_report_manager;

/** Registro Simple **/
public:
    void setRegistroMatriculas(const SimpleMatriculas &RegistroMatriculas);
public slots:    
    void guardarRegistroSimpleMatriculas();

/** Busqueda de PAIR **/
private:
    QStringList m_MatriculasList;
    bool encontrarPareja();
    bool buscarPareja(const long long &_id, const QString &_matricula);
    bool actualizarPareja();
    /** END DB **/
};

}
}
#endif // DATABASEMANAGER_H
