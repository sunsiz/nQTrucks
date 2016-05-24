#ifndef DAEMON_H
#define DAEMON_H


#include <QObject>
#include "nqtglobal.h"

#include "Dispositivos/Basculas/nQSerialPortReader.h"
#include "Dispositivos/NewsagesIO.h"

#include "Dispositivos/CamaraIP.h"
#include "Dispositivos/Alpr/NewsagesAlpr.h"

#include <QSqlDatabase>

namespace nQTrucks{
namespace Core{

class Daemon : public QObject
{
    Q_OBJECT
public:
    explicit Daemon(Devices::nQSerialPortReader *_bascula=nullptr,
                    Devices::NewsagesIO         *_newsagesIO=nullptr,
                    Devices::CamaraIP           *_camara1=nullptr,
                    Devices::CamaraIP           *_camara2=nullptr,
                    Devices::NewsagesAlpr       *_alpr1=nullptr,
                    Devices::NewsagesAlpr       *_alpr2=nullptr,
                    QObject *parent=nullptr);

private:
    Devices::nQSerialPortReader *m_bascula;
    Devices::NewsagesIO *m_newsagesIO;
    Devices::CamaraIP  *m_camara1;
    Devices::CamaraIP  *m_camara2;
    Devices::NewsagesAlpr *m_alpr1;
    Devices::NewsagesAlpr *m_alpr2;

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
    bool m_entrando;

/** REGRISTRO **/
private:
    Registro_Simple m_registro_simple;
    Registro_Simple m_registro_simple_vacio;
    Registro_Simple_Matriculas m_registro_simple_matriculas;
    Registro_Simple_Matriculas m_registro_simple_matriculas_vacio;
/** END REGISTRO **/


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


    //CONVERSORES//
private:
    QByteArray mat2ByteArray(const cv::Mat &image);
    cv::Mat byteArray2Mat(QByteArray &byteArray);
    /** END CAMARAS **/

    /** ALPRS **/
private:
    int m_alpr_numero;
private slots:
    void onReplyMatriculaResults1(t_MatriculaResults &_registro);
    void onReplyMatriculaResults2(t_MatriculaResults &_registro);
    /** END ALPRS **/

/** BD **/
private:
    QSqlDatabase db;
private slots:
    void onGuardarRegistroSimple(Registro_Simple &_registro);
    void onGuardarRegistroSimpleMatriculas(Registro_Simple_Matriculas &_registro);
/** END BD **/


};

}
}

#endif // DAEMON_H