#ifndef NEWSAGESALPRTASK_H
#define NEWSAGESALPRTASK_H

#include <QObject>
#include <QImage>
#include <nqtglobal.h>

namespace nQTrucks {
namespace Devices {

class NewsagesAlprTask : public QObject{
    Q_OBJECT
public:
    explicit NewsagesAlprTask(int _nDevice=0, int _nType=0, cv::Mat _fotoCamara=cv::Mat(), Registros::t_MatriculaResults *_results={},  QSettings *_appsettings=0, QObject *parent = 0);
    ~NewsagesAlprTask();
signals:
    void workFinished();

    /** SETTINGS *******************************************************/
private:
    QSettings *m_settings;
    QString    m_config_file;
    void       loadconfig();
private:
    cv::Mat m_FotoCamara;
    void    setFotoCamara(const cv::Mat &FotoCamara){ m_FotoCamara = FotoCamara;}
    cv::Mat getFotoCamara() const {return m_FotoCamara;}

    int     m_nDevice;
    void    setNDevice(int nDevice) {m_nDevice = nDevice;}
    int     getNDevice() const {return m_nDevice;}

    int     m_nType;
    void    setNType(int nType) {m_nType = nType;}
    int     getNType() const {return m_nType;}

private:
    t_Plank    m_plank;
    void setPlank(const QString &_plankA, const QString &_plankB, const QString &_plankC);
    t_Plank getPlank() const{return m_plank;}

    QString  m_prewarp;
    void setPrewarp(const QString &prewarp){m_prewarp=prewarp;}
    QString getPrewarp() const{return m_prewarp;}
    /** END SETTINGS ****************************************************/


    /** CALIBRACION **************************************************/
private:
    void setFotoCalibrada();
    cv::Mat apply_prewarp(const cv::Mat &img);
    /** Algoritmo AUTOCALIBRACION **/
    int m_retry_panks=5;
    void guardarPlanK();
    /** END ALGORITMO AUTOCALIBRACION**/

public slots:
    void calibrar();
signals:
    void ReplyOriginalFotoBlanca(const cv::Mat &Foto);
    void ReplyOriginalFotoRoja(const cv::Mat &Foto);
    /** END CALIBRACION ************************************************/

    /** PROCESAR **************************************************************/
public slots:
    void procesar();
private:
   Registros::t_MatriculaResults *m_matricularesult;
   void procesarBlancas();
   void procesarRojas();
signals:
   void ReplyMatriculaFoto();
    /** END PROCESAR **************************************************************/

    /** TOOLS **/
private:
    /*static*/ QImage     convertMat2QImage(   const cv::Mat    &_cvimage);
    /*static*/ cv::Mat    convertQImage2Mat(   const QImage     &_qimage);
    /*static*/ QByteArray convertMat2ByteArray(const cv::Mat    &_cvimage);
    /** END TOOLS **/

};

}
}
#endif // NEWSAGESALPRTASK_H
