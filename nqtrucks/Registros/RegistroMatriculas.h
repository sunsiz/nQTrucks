#ifndef NQTRUCKS_REGISTROMATRICULAS_H
#define NQTRUCKS_REGISTROMATRICULAS_H

#include "Bascula.h"
#include "MatriculaResults.h"
#include <QDateTime>

namespace nQTrucks{
        /** REPORTS **/
        class RegistroMatriculas{
        public:
            explicit RegistroMatriculas();
            void setRegistroMatriculas(const RegistroMatriculas &value);
            void setFechaRegistro(const QDateTime &value){FechaRegistro = value;}
            void setId(           long long        value){id            = value;}
            long long getId()            const {return id;  }
            QDateTime getFechaRegistro() const {return FechaRegistro;}

            Bascula          *m_bascula  = new Bascula;
            MatriculaResults *m_results0 = new MatriculaResults;
            MatriculaResults *m_results1 = new MatriculaResults;

            void clear();

        private:
            long long   id=0;
            QDateTime   FechaRegistro;
    };
}

#endif

