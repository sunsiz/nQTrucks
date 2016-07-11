#include "Maestros.h"

namespace nQTrucks{
    namespace Maestros{
    Maestros::Maestros(QObject *parent)
        : QObject(parent)
    {
        crearMaestros();
    }

    void Maestros::crearMaestros(){
        m_RegistroPeso  = new RegistroPeso(this);

        m_Empresa = new QSqlTableModel(this,m_RegistroPeso->getCurrentDb());
        m_Empresa->setTable("empresa");
        m_Empresa->select();

        m_Empresa_Mapper = new QDataWidgetMapper(this);
        m_Empresa_Mapper->setOrientation(Qt::Horizontal);
        m_Empresa_Mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
        m_Empresa_Mapper->setModel(m_Empresa);
        m_Empresa_Mapper->setItemDelegate(new QSqlRelationalDelegate(this));

    }

    void Maestros::eliminarMaestros(){
        m_RegistroPeso->deleteLater();
        m_Empresa->deleteLater();
        m_Empresa_Mapper->deleteLater();
    }

    void Maestros::sincronizar(){
        m_RegistroPeso->syncTable();
        m_Empresa->select();
    }

    }
}
