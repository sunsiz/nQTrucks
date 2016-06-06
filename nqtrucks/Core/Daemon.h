#ifndef DAEMON_H
#define DAEMON_H


#include <QObject>
#include "nqtglobal.h"

#include "Dispositivos/Basculas/nQSerialPortReader.h"
#include "Dispositivos/NewsagesIO.h"

#include "Dispositivos/CamaraIP.h"
#include "Dispositivos/Alpr/NewsagesAlpr.h"

//#include <QSqlDatabase>
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
    Registros::Simple m_registro_simple;
    Registros::Simple m_registro_simple_vacio;
    Registros::Simple_Matriculas m_registro_simple_matriculas;
    Registros::Simple_Matriculas m_registro_simple_matriculas_vacio;
/** END REGISTRO **/


    /** PESO ****************************************************************************************************/
private slots:
    void onPesoNuevo(const t_Bascula &_nuevaPesada);
    void onBasculaChanged(const t_Bascula &_pesoRT);
    /** FIN PESO ***********************************************************************************************/

    /** CAMARAS **/
private:
    int m_foto_numero;    
    std::unique_ptr<QMetaObject::Connection> pcamaraconn1{new QMetaObject::Connection};
    QMetaObject::Connection &camaraconn1 = *pcamaraconn1;
    std::unique_ptr<QMetaObject::Connection> pcamaraconn2{new QMetaObject::Connection};
    QMetaObject::Connection &camaraconn2 = *pcamaraconn2;
    std::unique_ptr<QMetaObject::Connection> pcamaraconncv1{new QMetaObject::Connection};
    QMetaObject::Connection &camaraconncv1 = *pcamaraconncv1;
    std::unique_ptr<QMetaObject::Connection> pcamaraconncv2{new QMetaObject::Connection};
    QMetaObject::Connection &camaraconncv2 = *pcamaraconncv2;


private slots:
    void onReplyCamaraIPFoto1(const QByteArray &_Reply);
    void onReplyCamaraIPFotoCV1(const cv::Mat &_cvimage);
    void onReplyCamaraIPFoto2(const QByteArray &_Reply);
    void onReplyCamaraIPFotoCV2(const cv::Mat &_cvimage);


    /** TOOLS **/
private:
    nQTrucks::Tools m_tools;
    /** END CAMARAS **/

    /** ALPRS **/
private:
    int m_alpr_numero;
    std::unique_ptr<QMetaObject::Connection> palprconn1{new QMetaObject::Connection};
    std::unique_ptr<QMetaObject::Connection> palprconn2{new QMetaObject::Connection};
    QMetaObject::Connection &alprconn1 = *palprconn1;
    QMetaObject::Connection &alprconn2 = *palprconn2;
private slots:
    void onReplyMatriculaResults1(const Registros::t_MatriculaResults &_registro);
    void onReplyMatriculaResults2(const Registros::t_MatriculaResults &_registro);
    /** END ALPRS **/


/** CONVERSORES **/
private:
    QByteArray resizeFoto(QByteArray _ByteArray);
    cv::Mat byteArray2Mat(QByteArray &byteArray);
    QImage convertMat2QImage(const cv::Mat &src);
    QByteArray convertMat2ByteArray(const cv::Mat &img);
/** END CONVERSORES **/

/** BD **/
private:
    QThread *hiloDb;
    Db::DatabaseManager *tareaDb;
private slots:
    void onGuardarRegistroSimple(Registros::Simple &_registro);
    void onGuardarRegistroSimpleMatriculas(Registros::Simple_Matriculas &_registro);
/** END BD **/


};

}
}

#endif // DAEMON_H
