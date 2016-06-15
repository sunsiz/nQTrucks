#ifndef REGISTROPESO_H
#define REGISTROPESO_H

#include <Db/QueryModel.h>

namespace nQTrucks{
    namespace Maestros{

        class RegistroPeso : public Db::QueryModel {
            Q_OBJECT
        public:
            RegistroPeso(QObject *parent=0);

            /** SETTINGS **********************************************************************************************************/
        protected:
            void      loadconfig();

        protected:
            void setTable();
            bool login_ok();
            void refreshModel();
            void configQueries();


        public:
            QString getNewQuery() const;
            void setNewQuery(const QString &query);
            QString getCurrentQuery() const;
            void setCurrentQuery(const QString &query);
        private:
            QString m_CurrentQuery;// = "select * from " + QString(appServer_tablename);
            QString m_NewQuery;
        protected:
            QString m_DefaultQuery;// = "select * from " + QString(appServer_tablename);
        };
    } /** End namespace Maestros **/
} /** End namespace nQTrucks **/

#endif // REGISTROPESO_H
