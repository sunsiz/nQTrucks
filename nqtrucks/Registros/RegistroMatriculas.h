#ifndef NQTRUCKS_REGISTROMATRICULAS_H
#define NQTRUCKS_REGISTROMATRICULAS_H

#include "Bascula.h"
#include "MatriculaResults.h"
#include <QDateTime>

namespace nQTrucks{
    namespace Registros{
        /** REPORTS **/
        class RegistroMatriculas{
        public:
        //RegistroMatriculas(QObject *parent=nullptr):QObject(parent){}
        explicit RegistroMatriculas();
        void setRegistroMatriculas(const RegistroMatriculas &value);
        void clear();

        Bascula          *m_bascula;
        MatriculaResults *m_results0;// = QVector<Registros::MatriculaResults>(2);
        MatriculaResults *m_results1;// = QVector<Registros::MatriculaResults>(2);

        long long getId() const;
        void setId(long long value);

        QDateTime getFechaRegistro() const;
        void setFechaRegistro(const QDateTime &value);

    private:
        long long   id ;//                  =0;                                                     //id fuente de captura de foto
        QDateTime   FechaRegistro;

    };

} // namespace Registros /** END REPORTS **/
}

#endif

