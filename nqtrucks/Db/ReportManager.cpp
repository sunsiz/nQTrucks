#include "ReportManager.h"

#include <QPrinter>
#include <QPrinterInfo>

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStringListModel>

#include <LimeReport>

namespace nQTrucks {
namespace Db {

ReportManager::ReportManager(QObject *parent)
    :QObject(parent)
{

}

void ReportManager::printRegistroMatricula(const QSqlDatabase &_db, const int &row)
{
    if (!QPrinterInfo::defaultPrinter().isNull()){
        QPrinter _default_printer;
        _default_printer.setPrinterName(QPrinterInfo::defaultPrinter().printerName());

        if (_db.isOpen()){
            QSqlQuery* m_matriculas = new QSqlQuery(_db);
            m_matriculas->prepare(" Select *"
                                  " from registros_matriculas "
                                  " where id = :id");
            m_matriculas->bindValue(":id",row);
            m_matriculas->exec();
            QSqlQueryModel* matriculasModel = new QSqlQueryModel();
            matriculasModel->setQuery(*m_matriculas);

            LimeReport::ReportEngine *m_report = new LimeReport::ReportEngine();
            m_report->loadFromFile(m_informe_peso);
            m_report->dataManager()->addModel("registros_matriculas",matriculasModel,true);
            m_report->printReport(&_default_printer);
        };
    }
}




}
}
