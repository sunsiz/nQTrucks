#ifndef DAEMON_H
#define DAEMON_H


#include <QObject>
#include "nqtglobal.h"

#include "Dispositivos/Basculas/nQSerialPortReader.h"
#include "Dispositivos/NewsagesIO.h"

#include "Dispositivos/CamaraIP.h"

#include <QSqlDatabase>

namespace nQTrucks{
namespace Core{

class Daemon : public QObject
{
    Q_OBJECT
public:
    explicit Daemon(Devices::nQSerialPortReader *_bascula=nullptr,
                    Devices::NewsagesIO *_newsagesIO=nullptr,
                    Devices::CamaraIP *_camara1=nullptr,
                    Devices::CamaraIP *_camara2=nullptr,
                    QObject *parent=nullptr);

private:
    Devices::nQSerialPortReader *m_bascula;
    Devices::NewsagesIO *m_newsagesIO;
    Devices::CamaraIP  *m_camara1;
    Devices::CamaraIP  *m_camara2;

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
    Registro_Simple m_registro_simple;
    Registro_Simple m_registro_simple_vacio;
/** END REGISTRO


    /** PESO ****************************************************************************************************/
private slots:
    void onPesoNuevo(const t_Bascula &_nuevaPesada);
    void onBasculaChanged(const t_Bascula &_pesoRT);
    /** FIN PESO ***********************************************************************************************/

    /** CAMARAS **/
private:
    int m_foto_numero;
private slots:
    void onReplyCamaraIPFoto1(const QByteArray &_Reply);
    void onReplyCamaraIPFoto2(const QByteArray &_Reply);
    /** END CAMARAS **/



/** BD **/
private:
    QSqlDatabase db;
private slots:
    void onGuardarRegistroSimple(const Registro_Simple &_registro);
/** END BD **/


};

}
}

#endif // DAEMON_H
