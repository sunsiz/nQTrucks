#ifndef REGISTROPESO_H
#define REGISTROPESO_H

#include <Db/QueryModel.h>
#include "nqtglobal.h"

namespace nQTrucks{
    namespace Maestros{

        class RegistroPeso : public Db::QueryModel {
            Q_OBJECT
        public:
           explicit RegistroPeso(QObject *parent=0);
        public slots:
            void syncTable() {setTable();}

            QSqlDatabase getDb() const;
        protected:
            void setTable();
            void refreshModel() {setTable();}
            void configQueries();            

        protected:
            QString m_DefaultQuery;// = "select * from " + QString(appServer_tablename);



        public:
        QDateTime getFechaRegistro(const long long &_id);
        bool guardarRegistroSimpleMatriculas(SimpleMatriculas &RegistroMatriculas);
        bool eliminaFotosCamara(const long long &_id);
        bool buscarPareja(QVector<SimpleMatriculas> &RegistrosMatriculas, const QString &_matricula);
        bool actualizarPareja(QVector<SimpleMatriculas> &RegistrosMatriculas);
        };
    } /** End namespace Maestros **/
} /** End namespace nQTrucks **/

#endif // REGISTROPESO_H
