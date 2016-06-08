#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H

#include <QObject>
#include <nqtglobal.h>
#include <LimeReport>
#include <QApplication>

namespace nQTrucks {
namespace Db {

class ReportManager : public QObject{
   Q_OBJECT
public:
    explicit ReportManager(QObject *parent=nullptr);

private:
    LimeReport::ReportEngine m_report;
private:
    QString m_informe_peso = QApplication::applicationDirPath()+"/reports/peso.lrxml";
public:
    void printRegistroMatricula(const int &row);
};

}
}
#endif // REPORTMANAGER_H
