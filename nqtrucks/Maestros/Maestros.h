#ifndef MAESTROS_H
#define MAESTROS_H

#include <QObject>

#include "Maestros/RegistroPeso.h"

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
    signals:

    public slots:
        void sincronizar();
    };


    }
}

#endif // MAESTROS_H
