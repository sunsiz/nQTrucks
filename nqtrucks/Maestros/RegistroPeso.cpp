#include "RegistroPeso.h"

namespace nQTrucks{
    namespace Maestros{

        RegistroPeso::RegistroPeso(QObject *parent)
            : QueryModel(parent)
        {
            configQueries();
            setTable();
        }


        void RegistroPeso::configQueries(){
            m_DefaultQuery = "select * from registros_matriculas ;" ;//+ QString(appServer_tablename);
        }

        void RegistroPeso::setTable(){
            beginResetModel();
            if(initDB()){

                if (setQuery(m_DefaultQuery)){
                    while(canFetchMore() ) {
                        fetchMore();
                    }
                }
            }
            endResetModel();
        }



    } /** End namespace Maestros **/
} /** End namespace nQTrucks **/


