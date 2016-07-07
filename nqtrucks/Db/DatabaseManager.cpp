#include "DatabaseManager.h"

#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QVariant>
#include <QHash>
#include <QBitArray>

#include <QDebug>

namespace nQTrucks{
namespace Db{

DatabaseManager::DatabaseManager(Maestros::Maestros *_maestros, QObject *parent)
   : QObject(parent)
   , m_maestros(_maestros)
{
    m_RegistroMatriculas.resize(2);
    m_report_manager = new Db::ReportManager;

    connect(this,&DatabaseManager::printRegistroMatricula,m_report_manager,&ReportManager::printRegistroMatricula);
    connect(this,&DatabaseManager::printRegistroMatriculaProcesada,m_report_manager,&ReportManager::printRegistroMatriculaProcesada);
}

DatabaseManager::~DatabaseManager()
{

}

/** REGISTRO SIMPLE **/
void DatabaseManager::setRegistroMatriculas(RegistroMatriculas *_RegistroMatriculas){
    PrecisionA1=_RegistroMatriculas->m_results0->getMatriculaPrecisionA();
    PrecisionB1=_RegistroMatriculas->m_results0->getMatriculaPrecisionB();
    PrecisionA2=_RegistroMatriculas->m_results1->getMatriculaPrecisionA();
    PrecisionB2=_RegistroMatriculas->m_results1->getMatriculaPrecisionB();

    m_RegistroMatriculas[0] = _RegistroMatriculas; //[0] es el ultimo registro registrado = salida si pareja
    m_RegistroMatriculas[1] = new RegistroMatriculas(this);
}

void DatabaseManager::guardarRegistroRegistroMatriculas(){
    if (m_maestros->m_RegistroPeso->guardarRegistroRegistroMatriculas(m_RegistroMatriculas[0])){
        m_maestros->m_RegistroPeso->syncTable();
         emit printRegistroMatricula(m_maestros->m_RegistroPeso->getCurrentDb(),m_RegistroMatriculas[0]->getId());
        /** No Guardar Si Cualquiera de las 4 Matriculas es detectada Y BUSCAR SU PAREJA**/
        if ( PrecisionA1 >80 || PrecisionB1 >80 || PrecisionA2 >80 || PrecisionB2 >80  ){
            if(m_maestros->m_RegistroPeso->eliminaFotosCamara(m_RegistroMatriculas[0]->getId())){
                m_maestros->m_RegistroPeso->syncTable();
              /** Buscar Pareja **/
               if (encontrarPareja()){
                /** Si pareja **/
                   m_maestros->m_RegistroPeso->syncTable();
                   emit printRegistroMatriculaProcesada(m_maestros->m_RegistroPeso->getCurrentDb(),m_RegistroMatriculas[1]->getId(),m_RegistroMatriculas[0]->getId());
               }
            }
        }        
    }
    emit printFinished();
}
/** END REGISTRO SIMPLE **/


/** BUSCAR PAIR **/
bool DatabaseManager::encontrarPareja()
{
    /** Que fecha final tenemos?**/
    QDateTime rowDate = m_maestros->m_RegistroPeso->getFechaRegistro(m_RegistroMatriculas[0]->getId());

    if (!rowDate.isNull()){
        m_RegistroMatriculas[0]->setFechaRegistro(rowDate);
        //m_RegistroMatriculas[0].FechaRegistro=m_maestros->m_RegistroPeso->getFechaRegistro(m_RegistroMatriculas[0].id);
        qDebug() << "Fecha registro: " << m_RegistroMatriculas[0]->getFechaRegistro();
        qDebug() << "Fecha registro Date: " << m_RegistroMatriculas[0]->getFechaRegistro().date();

        /** Cuantas matriculas dispongo para encontrar? **/
        m_MatriculasList.clear();
        if (PrecisionA1 >80){
            m_MatriculasList << m_RegistroMatriculas[0]->m_results0->getMatriculaA();
        }

        if (PrecisionB1 >80){
            m_MatriculasList << m_RegistroMatriculas[0]->m_results0->getMatriculaB();
        }

        if (PrecisionA2 >80){
            m_MatriculasList << m_RegistroMatriculas[0]->m_results1->getMatriculaA();
        }

        if ( PrecisionB2 >80){
            m_MatriculasList << m_RegistroMatriculas[0]->m_results1->getMatriculaB();
        }

        /** Tenemos matriculas y fecha,
         * Buscamos por cada matricula que tenemos en esa fecha,
         * hasta encontrar coincidencia **/
        for (QStringList::iterator it =  m_MatriculasList.begin(); it != m_MatriculasList.end(); ++it) {
            QString matriculaabuscar = *it;           
            /** consultar bd **/
            if (m_maestros->m_RegistroPeso->buscarPareja(m_RegistroMatriculas,matriculaabuscar)){
                return true;
            }
        }
    }
    return false;
}







}
}
