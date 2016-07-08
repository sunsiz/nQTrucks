#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

//#include <QObject>
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
    explicit DatabaseManager(QObject *parent=nullptr);
    ~DatabaseManager();
signals:
    void rowsPesoChanged();
    void workFinished();
    void printFinished();

    /** DB **/
private:
    Maestros::Maestros *m_maestros;
    QVector<RegistroMatriculas*> m_RegistroMatriculas;
    ReportManager *m_report_manager;

/** Registro Simple **/
public:
    void setRegistroMatriculas(RegistroMatriculas *_RegistroMatriculas);
    void setMaestros(          Maestros::Maestros *_maestros){this->m_maestros= _maestros;}
public:
    void guardarRegistroRegistroMatriculas();

signals:
    void printRegistroMatricula(const QSqlDatabase &_db,const long long &_row1);
    void printRegistroMatriculaProcesada(const QSqlDatabase &_db,const long long &_row1,const long long &_row2);
/** Busqueda de PAIR **/
private:
    float PrecisionA1,PrecisionA2,PrecisionB1,PrecisionB2;
    QStringList m_MatriculasList;
    bool encontrarPareja();
    bool buscarPareja(const long long &_id, const QString &_matricula);
    bool actualizarPareja();
    /** END DB **/
};

}
}
#endif // DATABASEMANAGER_H
