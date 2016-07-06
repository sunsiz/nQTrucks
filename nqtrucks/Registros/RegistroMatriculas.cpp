#include "RegistroMatriculas.h"

namespace nQTrucks {
    /** REPORTS **/
    RegistroMatriculas::RegistroMatriculas(QObject *parent)
        :QObject(parent)
    {
        qRegisterMetaType<RegistroMatriculas>             ("RegistroMatriculas");
    }

    void RegistroMatriculas::setRegistroMatriculas(const RegistroMatriculas &value){
        clear();
        setId(value.getId());
        setFechaRegistro(value.getFechaRegistro());
        m_bascula->setBascula(value.m_bascula->getBascula());

        m_results0->setMatriculaResults(value.m_results0->getMatriculaResults());
        m_results1->setMatriculaResults(value.m_results1->getMatriculaResults());
        }

        void RegistroMatriculas::clear(){
            m_bascula->deleteLater();
            m_results0->deleteLater();
            m_results1->deleteLater();
        }
} /** end namespace **/

