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
    explicit NewsagesAlprTask(int _nDevice=0, cv::Mat _fotoCamara=cv::Mat(), QSettings *_appsettings=0, QObject *parent = 0);
    ~NewsagesAlprTask();

public slots:
    cv::Mat FotoCamara() const {return m_FotoCamara;}
    //void setFotoCamara();
private:
    cv::Mat m_FotoCamara;
signals:
    void workFinished();

    /** CONVERSORES ********************************************************/
private:
    static cv::Mat convertQImage2Mat(const QImage& img);
    static QImage convertMat2QImage(const cv::Mat& image);
    /** END CONVERSORES **********************************************************/

    /** SETTINGS *******************************************************/
private:
    QString   m_configroot;
    int       m_nDevice;
    void      loadconfig();
    QSettings *m_settings;
    t_Plank    m_Plank;
    void setPlank(QString A,QString B, QString C);
    QString  m_prewarp;
    void setPrewarp(QString prewarp);
    /** END SETTINGS ****************************************************/


    /** CALIBRACION **************************************************/
private:
    cv::Mat m_FotoCalibradaBlancosCV;
    cv::Mat m_FotoCalibradaRojosCV;
public slots:
    void calibrarBlanco();
    void calibrarRojo();
    cv::Mat FotoCalibrada(int n) const;
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
    void ReplyMatriculaFoto        (const QString &matricula, const QString &confianza, const bool &detectada ,const cv::Mat &Foto);
    void ReplyMatriculaFotoRemolque(const QString &matricula, const QString &confianza, const bool &detectada ,const cv::Mat &Foto);
    /** END PROCESAR **************************************************************/

};

}
}
#endif // NEWSAGESALPRTASK_H
