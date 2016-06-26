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
    ~ReportManager();

public:
    void printRegistroMatricula(         const QSqlDatabase &_db, const long long &_row1);
    void printRegistroMatriculaProcesada(const QSqlDatabase &_db, const long long &_row1, const long long &_row2);
private:
    QString informe_Peso          = QApplication::applicationDirPath()+"/reports/peso_matricula.lrxml";
    QString informe_PesoProcesado = QApplication::applicationDirPath()+"/reports/peso_procesado.lrxml";
};

}
}
#endif // REPORTMANAGER_H
