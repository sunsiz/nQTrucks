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

void ReportManager::printRegistroMatricula(const QSqlDatabase &_db, const long long &_row1)
{
    if (!QPrinterInfo::defaultPrinter().isNull()){
        QPrinter _default_printer;
        _default_printer.setPrinterName(QPrinterInfo::defaultPrinter().printerName());

        if (_db.isOpen()){
            /** Registro 1 **/
            QSqlQuery* m_matriculas = new QSqlQuery(_db);
            m_matriculas->prepare(" Select *"
                                  " from registros_matriculas "
                                  " where id = :id");
            m_matriculas->bindValue(":id",_row1);
            m_matriculas->exec();
            QSqlQueryModel* matriculasModel = new QSqlQueryModel();
            matriculasModel->setQuery(*m_matriculas);


            /** Report **/
            LimeReport::ReportEngine *m_report = new LimeReport::ReportEngine();
            m_report->loadFromFile(m_Informe_Peso);
            m_report->dataManager()->addModel("registro1",matriculasModel,true);
            m_report->printReport(&_default_printer);
        };
    }
}


void ReportManager::printRegistroMatriculaProcesada(const QSqlDatabase &_db, const long long &_row2, const long long &_row1)
{
    if (!QPrinterInfo::defaultPrinter().isNull()){
        QPrinter _default_printer;
        _default_printer.setPrinterName(QPrinterInfo::defaultPrinter().printerName());

        if (_db.isOpen()){
            /** Registro 1 **/
            QSqlQuery* m_registro1 = new QSqlQuery(_db);
            m_registro1->prepare(" Select *"
                                  " from registros_matriculas "
                                  " where id = :id");
            m_registro1->bindValue(":id",_row1);
            m_registro1->exec();
            QSqlQueryModel* registroModel1 = new QSqlQueryModel();
            registroModel1->setQuery(*m_registro1);

            /** Registro 2 **/
            QSqlQuery* m_registro2 = new QSqlQuery(_db);
            m_registro2->prepare(" Select *"
                                  " from registros_matriculas "
                                  " where id = :id");
            m_registro2->bindValue(":id",_row2);
            m_registro2->exec();
            QSqlQueryModel* registroModel2 = new QSqlQueryModel();
            registroModel2->setQuery(*m_registro2);


            /** Report **/
            LimeReport::ReportEngine *m_report = new LimeReport::ReportEngine();
            m_report->loadFromFile(m_Informe_PesoProcesado);
            m_report->dataManager()->addModel("registro1",registroModel1,true);
            m_report->dataManager()->addModel("registro2",registroModel2,true);
            m_report->printReport(&_default_printer);
        };
    }
}






}
}
