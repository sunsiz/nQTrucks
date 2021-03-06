#ifndef NQTRUCKS_BASCULA_H
#define NQTRUCKS_BASCULA_H

#include <QObject>
#include <QSettings>
#include <QMetaType>
#include <math.h>

/** BASCULAS **************************************************************************************/

namespace nQTrucks{
        #define BASCULA    "BASCULA"
        class Bascula : public QObject
        {
            Q_OBJECT
        public:
            explicit Bascula(QObject *parent=nullptr);
            Bascula& getBascula()       { return *this; }
            const Bascula& getBascula() const { return *this; }
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
            void  setINeto(float value){  this->m_iNeto = fabs(value); }

        };
} /** END NAMESPACE NQTRUCKS **/
#endif // BASCULA_H

/** END BASCULAS *****************************************************************************************************/

