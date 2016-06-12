#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H

#include <QObject>
#include <nqtglobal.h>
#include <QApplication>
#include <QSqlDatabase>

namespace nQTrucks {
namespace Db {

class ReportManager : public QObject{
   Q_OBJECT
public:
    explicit ReportManager(QObject *parent=nullptr);

private:
    QString m_informe_peso = QApplication::applicationDirPath()+"/reports/peso_matricula.lrxml";
public:
    void printRegistroMatricula(const QSqlDatabase &_db, const int &row);
};

}
}
#endif // REPORTMANAGER_H
