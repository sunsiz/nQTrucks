#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <nqtglobal.h>

#include <QSqlDatabase>

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
    void workFinished();

    /** DB **/
private:
    void initDb();
    QSqlDatabase m_db;
    SimpleMatriculas m_RegistroMatriculas;
    ReportManager m_report_manager;
    long long m_lastrecord=0;




public:
    void setRegistroMatriculas(const SimpleMatriculas &RegistroMatriculas);

public slots:
    void guardarRegistroSimpleMatriculas();

    /** END DB **/
};

}
}
#endif // DATABASEMANAGER_H
