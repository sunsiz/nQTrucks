#include "Bascula.h"
/** BASCULAS **************************************************************************************/
namespace nQTrucks{
        Bascula::Bascula(QObject *parent)
            : QObject(parent)
        {
            this->clear();
        }
        void Bascula::setBascula(const Bascula &value){
            this->setBEstado(        value.m_bEstado);
            this->setBEstadoAnterior(value.m_bEstadoAnterior);
            this->setIBruto(         value.m_iBruto);
            this->setITara(          value.m_iTara);
            this->setINeto(          value.m_iNeto);
        }
        void Bascula::clear(){
            this->setBEstado(        false);
            this->setBEstadoAnterior(false);
            this->setIBruto(         0);
            this->setITara(          0);
            this->setINeto(          0);
        }
} /** END NAMESPACE NQTRUCKS **/

/** END BASCULAS *****************************************************************************************************/

