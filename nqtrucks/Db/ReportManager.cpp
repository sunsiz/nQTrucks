#include "ReportManager.h"

#include <QPrinter>
#include <QPrinterInfo>

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStringListModel>

#include <LimeReport>

namespace nQTrucks {
namespace Db {

static const QString qry_empresa_id   = " Select * from empresa where id = 1 ;";
static const QString qry_matricula_id = " Select * from registros_matriculas where id = :id;";

ReportManager::ReportManager(QObject *parent)
    :QObject(parent)
{

}
ReportManager::~ReportManager(){
//    QSqlDatabase::removeDatabase("reports1");
//    QSqlDatabase::removeDatabase("reports2");
}

void ReportManager::printRegistroMatricula(const QSqlDatabase &_db, const long long &_row1)
{
//    QMutex _cluje;
//    _cluje.lock();
    if (!QPrinterInfo::defaultPrinter().isNull()){
        QPrinter _default_printer;
        _default_printer.setPrinterName(QPrinterInfo::defaultPrinter().printerName());

        QSqlDatabase _dbtemp = QSqlDatabase::cloneDatabase(_db,"reports1");
        if (_dbtemp.open()){
            /** Registro Empresa **/        
            QSqlQuery* m_empresa = new QSqlQuery(_dbtemp);
            m_empresa->prepare(qry_empresa_id);
            m_empresa->exec();
            QSqlQueryModel* empresaModel = new QSqlQueryModel;
            empresaModel->setQuery(*m_empresa);

            /** Registro 1 **/
            QSqlQuery* m_registro1 = new QSqlQuery(_dbtemp);
            m_registro1->prepare(qry_matricula_id);
            m_registro1->bindValue(":id",_row1);
            m_registro1->exec();
            QSqlQueryModel* registroModel1 = new QSqlQueryModel;
            registroModel1->setQuery(*m_registro1);

            /** Report **/
            LimeReport::ReportEngine *m_report = new LimeReport::ReportEngine;
            m_report->loadFromFile(informe_Peso);
            m_report->dataManager()->addModel("registro1", registroModel1,true);
            m_report->dataManager()->addModel("empresa",   empresaModel,  true);
            m_report->printReport(&_default_printer);

            delete m_report;
//            registroModel1->clear();
//            delete registroModel1;

            m_registro1->finish();
            m_registro1->clear();
            delete m_registro1;

//            empresaModel->clear();
//            delete empresaModel;

            m_empresa->finish();
            m_empresa->clear();
            delete m_empresa;

            _dbtemp.close();

        }
    }
    //_cluje.unlock();
}


void ReportManager::printRegistroMatriculaProcesada(const QSqlDatabase &_db, const long long &_row1, const long long &_row2)
{
//    QMutex _cluje;
//    _cluje.lock();
    if (!QPrinterInfo::defaultPrinter().isNull()){
        QPrinter _default_printer;
        _default_printer.setPrinterName(QPrinterInfo::defaultPrinter().printerName());

        QSqlDatabase _dbtemp = QSqlDatabase::cloneDatabase(_db,"reports2");
        if (_dbtemp.open()){
            /** Registro Empresa **/
            QSqlQuery* m_empresa = new QSqlQuery(_dbtemp);
            m_empresa->prepare(qry_empresa_id);
            m_empresa->exec();
            QSqlQueryModel* empresaModel = new QSqlQueryModel;
            empresaModel->setQuery(*m_empresa);

            /** Registro 1 **/
            QSqlQuery* m_registro1 = new QSqlQuery(_dbtemp);
            m_registro1->prepare(qry_matricula_id);
            m_registro1->bindValue(":id",_row1);
            m_registro1->exec();
            QSqlQueryModel* registroModel1 = new QSqlQueryModel;
            registroModel1->setQuery(*m_registro1);

            /** Registro 2 **/
            QSqlQuery* m_registro2 = new QSqlQuery(_dbtemp);
            m_registro2->prepare(qry_matricula_id);
            m_registro2->bindValue(":id",_row2);
            m_registro2->exec();
            QSqlQueryModel* registroModel2 = new QSqlQueryModel;
            registroModel2->setQuery(*m_registro2);

            /** Report **/
            LimeReport::ReportEngine *m_report = new LimeReport::ReportEngine;
            m_report->loadFromFile(informe_PesoProcesado);
            m_report->dataManager()->addModel("registro2", registroModel2,true);
            m_report->dataManager()->addModel("registro1", registroModel1,true);
            m_report->dataManager()->addModel("empresa",   empresaModel,  true);
            m_report->printReport(&_default_printer);

            delete m_report;
            /** END REPORT **/

//            registroModel2->clear();
//            delete registroModel2;
            m_registro2->finish();
            m_registro2->clear();
            delete m_registro2;
            /** END Registro 2 **/


//            registroModel1->clear();
//            delete registroModel1;
            m_registro1->finish();
            m_registro1->clear();
            delete m_registro1;
            /** END Registro 1 **/

//            empresaModel->clear();
//            delete empresaModel;
            m_empresa->finish();
            m_empresa->clear();
            delete m_empresa;
            /** END Registro Empresa **/

            _dbtemp.close();
            //QSqlDatabase::removeDatabase(_dbtemp.connectionName());
        }
    }
//    _cluje.unlock();
}





}
}
