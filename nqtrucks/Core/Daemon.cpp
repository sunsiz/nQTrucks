#include "Daemon.h"

Daemon::Daemon(QObject *parent)
    : QObject(parent)
    , m_iniciado(false)
{


}

void Daemon::setIniciado(bool iniciado)
{
    m_iniciado = iniciado;
}

