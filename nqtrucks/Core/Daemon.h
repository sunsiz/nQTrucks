#ifndef DAEMON_H
#define DAEMON_H


#include <QObject>
#include "nqtglobal.h"

#include "Dispositivos/Basculas/nQSerialPortReader.h"
#include "Dispositivos/NewsagesIO.h"

#include "Dispositivos/CamaraIP.h"
#include "Dispositivos/Alpr/NewsagesAlpr.h"

#include "Db/DatabaseManager.h"

/* PUNTEROS **/
#include <memory>

namespace nQTrucks{
namespace Core{

class Daemon : public QObject
{
    Q_OBJECT
public:
    explicit Daemon(Devices::nQSerialPortReader *_bascula,
                    Devices::NewsagesIO         *_newsagesIO,
                    QVector<Devices::CamaraIP*> _camara,
                    QVector<Devices::NewsagesAlpr*> _alpr,
                    QObject *parent=nullptr);

private:
    Devices::nQSerialPortReader *m_bascula;
    Devices::NewsagesIO *m_newsagesIO;
    QVector<Devices::CamaraIP*> m_camara;
    QVector<Devices::NewsagesAlpr*> m_alpr;

public:
    void setInit(bool init);
signals:
    void initChanged(const bool &_init);
private slots:
    void onStartStop(const bool &_init);
private:
    bool m_init;
    bool m_registrando;
    bool m_saliendo;

/** REGRISTRO **/
private:
   SimpleMatriculas m_RegistroMatriculas;
signals:
    void RegistroChanged(const SimpleMatriculas &_results);
/** END REGISTRO **/


    /** PESO ****************************************************************************************************/
private slots:
    void onPesoNuevo(const Bascula &_nuevaPesada);
    void onBasculaChanged(const Bascula &_pesoRT);
    /** FIN PESO ***********************************************************************************************/

    /** CAMARAS **/
private:
    bool m_bfoto1;
    bool m_bfoto2;
    std::unique_ptr<QMetaObject::Connection> pcamaraconn1{new QMetaObject::Connection};
    QMetaObject::Connection &camaraconn1 = *pcamaraconn1;
    std::unique_ptr<QMetaObject::Connection> pcamaraconn2{new QMetaObject::Connection};
    QMetaObject::Connection &camaraconn2 = *pcamaraconn2;

    /** ALPRS **/
private:
    bool m_balpr1;
    bool m_balpr2;
    std::unique_ptr<QMetaObject::Connection> palprconn1{new QMetaObject::Connection};
    std::unique_ptr<QMetaObject::Connection> palprconn2{new QMetaObject::Connection};
    QMetaObject::Connection &alprconn1 = *palprconn1;
    QMetaObject::Connection &alprconn2 = *palprconn2;
    /** END ALPRS **/


/** BD **/
private:
    QThread *hiloDb;
    Db::DatabaseManager *tareaDb;
private slots:
    void onGuardarRegistroSimple();
    void onGuardarRegistroSimpleMatriculas();
/** END BD **/


};

}
}

#endif // DAEMON_H
