#ifndef NQTRUCKS_REGISTROMATRICULAS_H
#define NQTRUCKS_REGISTROMATRICULAS_H

#include "Bascula.h"
#include "MatriculaResults.h"
#include <QDateTime>

namespace nQTrucks{
        /** REPORTS **/
        class RegistroMatriculas : public QObject
        {
            Q_OBJECT
        public:
            explicit RegistroMatriculas(QObject *parent = nullptr);
            RegistroMatriculas(const RegistroMatriculas &other){this->setRegistroMatriculas(other);}
            RegistroMatriculas& operator=( const RegistroMatriculas& ) { return *this;  }
            void setRegistroMatriculas(const RegistroMatriculas &value);
            void setFechaRegistro(const QDateTime &value){FechaRegistro = value;}
            void setId(           long long        value){id            = value;}
            long long getId()            const {return id;  }
            QDateTime getFechaRegistro() const {return FechaRegistro;}

            Bascula          *m_bascula  = new Bascula(this);
            MatriculaResults *m_results0 = new MatriculaResults(this);
            MatriculaResults *m_results1 = new MatriculaResults(this);

            void clear();

        private:
            long long   id=0;
            QDateTime   FechaRegistro;
    };
}

#endif

