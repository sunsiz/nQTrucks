#ifndef NQTRUCKS_BASCULA_H
#define NQTRUCKS_BASCULA_H

#include <QObject>
#include <QMetaType>

/** BASCULAS **************************************************************************************/

namespace nQTrucks{
    namespace Registros{
        class Bascula : public QObject
        {
            Q_OBJECT
        public:
            explicit Bascula(QObject *parent=nullptr);
            Bascula* getBascula()       { return this; }
            const Bascula* getBascula() const { return this; }

//            Bascula& getBascula()       { return *this; }
//            const Basculs& getBasucula() const { return *this; }

//      // Return by value only needs one version.
//            Bascula getBascula() const { return *this; }

            //Bascula getBascula() const  { return *this; }
        private:
            bool  m_bEstado;
            bool  m_bEstadoAnterior;
            float m_iBruto;
            float m_iTara;
            float m_iNeto;
        public:
            void setBascula(const Bascula &value);
            void clear();

            bool getBEstado() const{ return this->m_bEstado;}
            void setBEstado(bool value){    this->m_bEstado = value;}

            bool getBEstadoAnterior() const{ return this->m_bEstadoAnterior;}
            void setBEstadoAnterior(bool value){    this->m_bEstadoAnterior = value;}

            float getIBruto() const{return this->m_iBruto;}
            void  setIBruto(float value){  this->m_iBruto = value;}

            float getITara() const{return this->m_iTara;}
            void  setITara(float value){  this->m_iTara = value; }

            float getINeto() const{return this->m_iNeto;}
            void  setINeto(float value){  this->m_iNeto = value; }

        };
    } /** END NAMESPACE Regristros **/
} /** END NAMESPACE NQTRUCKS **/
#endif // BASCULA_H

/** END BASCULAS *****************************************************************************************************/

