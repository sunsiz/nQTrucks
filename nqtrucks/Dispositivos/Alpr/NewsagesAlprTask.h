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
    explicit NewsagesAlprTask(int _nDevice=1, cv::Mat _fotoCamara=cv::Mat(), QSettings *_appsettings=0, QObject *parent = 0);
    ~NewsagesAlprTask();


public slots:
    cv::Mat FotoCamara() const {return m_FotoCamara;}
private:
    cv::Mat m_FotoCamara;
signals:
    void workFinished();

    /** CONVERSORES ********************************************************/
private:
    /*static*/ cv::Mat    convertQImage2Mat(const QImage &img);
    /*static*/ QImage     convertMat2QImage(const cv::Mat &image);
    /*static*/ QByteArray convertMat2Bytearray(const cv::Mat &img);
    /** END CONVERSORES **********************************************************/

    /** SETTINGS *******************************************************/
public:
    t_Plank Plank() const;
    QString prewarp() const;
private:
    QString   m_configroot;
    int       m_nDevice;
    void      loadconfig();
    QSettings *m_settings;
    t_Plank    m_Plank;    
    void setPlank(const QString &A, const QString &B, const QString &C);
    QString  m_prewarp;
    void setPrewarp(QString prewarp);
    /** END SETTINGS ****************************************************/


    /** CALIBRACION **************************************************/
private:
    cv::Mat m_FotoCalibradaBlancosCV;
    cv::Mat m_FotoCalibradaRojosCV;
    cv::Mat apply_prewarp(const cv::Mat &img);
    int m_count_pankb;
    int m_count_pankc;
public slots:
    void calibrarBlanco();
    void calibrarRojo();
    //cv::Mat FotoCalibrada(int n) const;
    void setFotoCalibrada(int n);
signals:
    void ReplyOriginalFotoBlanca(const cv::Mat &Foto);
    void ReplyOriginalFotoRoja(const cv::Mat &Foto);
    /** END CALIBRACION ************************************************/

    /** PROCESAR **************************************************************/
public slots:
    void procesarBlancas();
    void procesarRojas();
signals:
    void ReplyMatriculaFoto        (const QString &matricula, const QString &confianza, const bool &detectada ,const cv::Mat &Foto, const QByteArray &FotoByte);
    void ReplyMatriculaFotoRemolque(const QString &matricula, const QString &confianza, const bool &detectada ,const cv::Mat &Foto, const QByteArray &FotoByte);
    /** END PROCESAR **************************************************************/

};

}
}
#endif // NEWSAGESALPRTASK_H
