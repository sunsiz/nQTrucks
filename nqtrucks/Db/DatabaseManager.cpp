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
}

/** REGISTRO SIMPLE **/
void DatabaseManager::setRegistroMatriculas(const SimpleMatriculas &RegistroMatriculas)
{
    m_RegistroMatriculas[0] = RegistroMatriculas; //[0] es el ultimo registro registrado = salida si pareja
}

void DatabaseManager::guardarRegistroSimpleMatriculas(){
    if (m_maestros->m_RegistroPeso->guardarRegistroSimpleMatriculas(m_RegistroMatriculas[0])){
        m_maestros->m_RegistroPeso->syncTable();
        m_report_manager.printRegistroMatricula(m_maestros->m_RegistroPeso->getCurrentDb(),m_RegistroMatriculas[0].id);
        /** No Guardar Si Cualquiera de las 4 Matriculas es detectada Y BUSCAR SU PAREJA**/
        if ( m_RegistroMatriculas[0].results[0].MatriculaPrecisionA >80 || m_RegistroMatriculas[0].results[0].MatriculaPrecisionB >80 ||
             m_RegistroMatriculas[0].results[1].MatriculaPrecisionA >80 || m_RegistroMatriculas[0].results[1].MatriculaPrecisionB >80  ){
            if(m_maestros->m_RegistroPeso->eliminaFotosCamara(m_RegistroMatriculas[0].id)){
              /** Buscar Pareja **/
               if (encontrarPareja()){
                /** Si pareja **/
                   m_maestros->m_RegistroPeso->syncTable();
                   m_report_manager.printRegistroMatriculaProcesada(m_maestros->m_RegistroPeso->getCurrentDb(),m_RegistroMatriculas[1].id,m_RegistroMatriculas[0].id);
               }
            }
        }
    }
    emit workFinished();
}
/** END REGISTRO SIMPLE **/


/** BUSCAR PAIR **/
bool DatabaseManager::encontrarPareja()
{
    /** Que fecha final tenemos?**/
    QDateTime rowDate = m_maestros->m_RegistroPeso->getFechaRegistro(m_RegistroMatriculas[0].id);

    if (!rowDate.isNull()){
        m_RegistroMatriculas[0].FechaRegistro=rowDate;
        //m_RegistroMatriculas[0].FechaRegistro=m_maestros->m_RegistroPeso->getFechaRegistro(m_RegistroMatriculas[0].id);
        qDebug() << "Fecha registro: " << m_RegistroMatriculas[0].FechaRegistro;
        qDebug() << "Fecha registro Date: " << m_RegistroMatriculas[0].FechaRegistro.date();

        /** Cuantas matriculas dispongo para encontrar? **/
        m_MatriculasList.clear();
        if (m_RegistroMatriculas[0].results[0].MatriculaPrecisionA >80){
            m_MatriculasList << m_RegistroMatriculas[0].results[0].MatriculaA;
        }

        if (m_RegistroMatriculas[0].results[0].MatriculaPrecisionB >80){
            m_MatriculasList << m_RegistroMatriculas[0].results[0].MatriculaB;
        }

        if (m_RegistroMatriculas[0].results[1].MatriculaPrecisionA >80){
            m_MatriculasList << m_RegistroMatriculas[0].results[1].MatriculaA;
        }

        if (m_RegistroMatriculas[0].results[1].MatriculaPrecisionB >80){
            m_MatriculasList << m_RegistroMatriculas[0].results[1].MatriculaB;
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
