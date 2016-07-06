#include "Maestros.h"

namespace nQTrucks{
    namespace Maestros{
    Maestros::Maestros(QObject *parent)
        : QObject(parent)
    {
        crearMaestros();
    }

    void Maestros::crearMaestros(){
        m_RegistroPeso  = new RegistroPeso;
        m_Empresa       = new Empresa;
    }

    void Maestros::eliminarMaestros(){
        m_RegistroPeso->deleteLater();
        m_Empresa->deleteLater();
    }

    void Maestros::sincronizar(){
        m_RegistroPeso->syncTable();
        m_Empresa->syncTable();
    }

    }
}
