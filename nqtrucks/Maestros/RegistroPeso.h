#ifndef REGISTROPESO_H
#define REGISTROPESO_H

#include <Db/QueryModel.h>

namespace nQTrucks{
    namespace Maestros{

        class RegistroPeso : public Db::QueryModel {
            Q_OBJECT
        public:
           explicit RegistroPeso(QObject *parent=0);
        public slots:
            void syncTable() {setTable();}

        protected:
            void setTable();
            void refreshModel() {setTable();}
            void configQueries();            

        protected:
            QString m_DefaultQuery;// = "select * from " + QString(appServer_tablename);



        public:
        QDateTime getFechaRegistro(const long long &_id);





        };
    } /** End namespace Maestros **/
} /** End namespace nQTrucks **/

#endif // REGISTROPESO_H
