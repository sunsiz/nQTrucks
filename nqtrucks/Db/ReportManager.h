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
    QString m_Informe_Peso          = QApplication::applicationDirPath()+"/reports/peso_matricula.lrxml";
    QString m_Informe_PesoProcesado = QApplication::applicationDirPath()+"/reports/peso_procesado.lrxml";
public:
    void printRegistroMatricula(         const QSqlDatabase &_db, const long long &_row1);
    void printRegistroMatriculaProcesada(const QSqlDatabase &_db, const long long &_row2, const long long &_row1);
};

}
}
#endif // REPORTMANAGER_H
