#ifndef EMPRESA_H
#define EMPRESA_H

#include <Db/QueryModel.h>
#include "nqtglobal.h"

namespace nQTrucks{
    namespace Maestros{

        class Empresa : public Db::QueryModel {
            Q_OBJECT
        public:
           explicit Empresa(QObject *parent=0);
        public slots:
            void syncTable() {setTable();}

        protected:
            void setTable();
            void refreshModel() {setTable();}
            void configQueries();

        protected:
            QString m_DefaultQuery;// = "select * from " + QString(appServer_tablename);

        public:
            void actualizarEmpresa(const QVector<QString> &_empresaVector);

        };
    } /** End namespace Maestros **/
} /** End namespace nQTrucks **/

#endif // EMPRESA_H
