#include "RegistroMatriculas.h"

namespace nQTrucks {
    /** REPORTS **/
    RegistroMatriculas::RegistroMatriculas(){
        qRegisterMetaType<RegistroMatriculas>             ("RegistroMatriculas");
    }

    void RegistroMatriculas::setRegistroMatriculas(const RegistroMatriculas &value){
        clear();
        setId(value.getId());
        setFechaRegistro(value.getFechaRegistro());
        m_bascula   = new Bascula;
        m_bascula->setBascula(value.m_bascula->getBascula());

        m_results0  = new MatriculaResults;
        m_results0->setMatriculaResults(value.m_results0->getMatriculaResults());
        m_results1  = new MatriculaResults;
        m_results1->setMatriculaResults(value.m_results1->getMatriculaResults());
        }

        void RegistroMatriculas::clear(){
            delete m_bascula;
            delete m_results0;
            delete m_results1;
        }
} /** end namespace **/

