#include "RegistroPeso.h"

namespace nQTrucks{
    namespace Maestros{

        RegistroPeso::RegistroPeso(QObject *parent)
            : QueryModel(parent)
        {
            loadconfig();
            configQueries();
            setTable();
        }

        RegistroPeso::loadconfig(){

        }

        void RegistroPeso::configQueries(){
            m_DefaultQuery = "select * from " ;//+ QString(appServer_tablename);
            setCurrentQuery(m_DefaultQuery);
        }

        void RegistroPeso::setTable(){
                if (initDB()){
                    if (setQuery(getCurrentQuery())){     //,db))
                        while(canFetchMore() ) {fetchMore(); }
                    }
                }
        }






    } /** End namespace Maestros **/
} /** End namespace nQTrucks **/


