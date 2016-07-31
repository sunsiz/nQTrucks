#ifndef MAESTROS_H
#define MAESTROS_H

#include <QObject>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>

#include "Maestros/RegistroPeso.h"
//#include "Maestros/Empresa.h"

namespace nQTrucks{
    namespace Maestros{
    class Maestros : public QObject
    {
        Q_OBJECT
    public:
        explicit Maestros(QObject *parent = nullptr);
    private:
        void crearMaestros();
        void eliminarMaestros();
    public:
        RegistroPeso *m_RegistroPeso;
        QSqlTableModel      *m_RegistroPesoTable;
        QSqlTableModel      *m_Empresa;
        QDataWidgetMapper   *m_Empresa_Mapper;
    signals:

    public slots:
        void sincronizar();
    };


    }
}

#endif // MAESTROS_H
