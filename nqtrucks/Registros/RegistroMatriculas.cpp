#include "RegistroMatriculas.h"

namespace nQTrucks {
    /** REPORTS **/
        RegistroMatriculas::RegistroMatriculas()
            : m_bascula (new Bascula)
            , m_results0  (new MatriculaResults)
            , m_results1  (new MatriculaResults)
        {
        }

        void RegistroMatriculas::setRegistroMatriculas(const RegistroMatriculas &value){
            clear();
            m_bascula   = new Bascula(value.m_bascula);
            m_results0  = new MatriculaResults;
            m_results1  = new MatriculaResults;
            setId(value.getId());
            setFechaRegistro(value.getFechaRegistro());
        }

        void RegistroMatriculas::clear()    {
            delete m_bascula;
            delete m_results0;
            delete m_results1;

        }

        long long RegistroMatriculas::getId() const
        {
            return id;
        }

        void RegistroMatriculas::setId(long long value)
        {
            id = value;
        }

        QDateTime RegistroMatriculas::getFechaRegistro() const
        {
            return FechaRegistro;
        }

        void RegistroMatriculas::setFechaRegistro(const QDateTime &value)
        {
            FechaRegistro = value;
        }


} /** end namespace **/

