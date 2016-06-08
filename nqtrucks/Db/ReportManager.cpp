#include "ReportManager.h"
#include <QPrinter>
#include <QPrinterInfo>

namespace nQTrucks {
namespace Db {

ReportManager::ReportManager(QObject *parent)
    :QObject(parent)
{

}

void ReportManager::printRegistroMatricula(const int &row)
{

    if (!QPrinterInfo::defaultPrinter().isNull()){
        QPrinter _default_printer;
        _default_printer.setPrinterName(QPrinterInfo::defaultPrinter().printerName());
        m_report.loadFromFile(m_informe_peso);
        m_report.dataManager()->clearUserVariables();
        m_report.dataManager()->setReportVariable(QString("ID"),row);
        m_report.printReport(&_default_printer);
    }


}

}
}
