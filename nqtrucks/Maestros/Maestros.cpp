#include "Maestros.h"

namespace nQTrucks{
    namespace Maestros{
    Maestros::Maestros(QObject *parent)
        : QObject(parent)
    {
        crearMaestros();
    }

    void Maestros::crearMaestros(){
        m_RegistroPeso = new RegistroPeso(this);
    }

    void Maestros::eliminarMaestros(){
        m_RegistroPeso->deleteLater();
    }

    void Maestros::sincronizar(){
        m_RegistroPeso->syncTable();
    }

    }
}
