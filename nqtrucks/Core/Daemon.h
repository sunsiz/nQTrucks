#ifndef DAEMON_H
#define DAEMON_H


#include <QObject>
#include "nqtglobal.h"


class Daemon : public QObject
{
    Q_OBJECT
public:
    explicit Daemon(QObject *parent=nullptr);
    void setIniciado(bool iniciado);

private:
    bool m_iniciado;

private slots:


};

#endif // DAEMON_H
