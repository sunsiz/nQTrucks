#ifndef NEWSAGESALPRTASK_H
#define NEWSAGESALPRTASK_H

#include <QObject>
#include <QImage>
#include <nqtglobal.h>

#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>





namespace nQTrucks {
namespace Devices {

class NewsagesAlprTask : public QObject{
    Q_OBJECT
public:
    NewsagesAlprTask(int nDevice=0, QImage _fotoCamara=QImage(), QSettings *_appsettings=0, QObject *parent = 0);
    ~NewsagesAlprTask();          

    //QImage getFotoCalibrada(){return m_FotoCalibrada;}


public slots:
    QImage FotoCamara() const {return m_FotoCamara;}
    cv::Mat FotoCamaraCV() const {return m_FotoCamaraCV;}
    void setFotoCamara();
private:
    QImage m_FotoCamara;
    cv::Mat m_FotoCamaraCV;
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
    /** END SETTINGS ****************************************************/



    /** CALIBRACION **************************************************/
public slots:
    QImage FotoCalibradaBlancos() const;
    void setFotoCalibradaBlancos();
private:
    QImage m_FotoCalibradaBlancos;
    cv::Mat m_FotoCalibradaBlancosCV;
    QImage m_FotoCalibradaRojos;
//    QThread *hiloCalibrarBlancas;
//    QThread *hiloCalibrarRojas;
public slots:
    void calibrarBlanco();
    void calibrarRojo();
signals:
    void ReplyOriginalFotoBlanca(const QImage &Foto);
    void ReplyOriginalFotoRoja(const QImage &Foto);
    void onCalibrar();
    /** END CALIBRACION ************************************************/

    /** PROCESAR **************************************************************/
public slots:
    void procesarBlancas();
    void procesarRojas();
signals:
    void ReplyMatriculaFoto        (const QString &matricula, const QString &confianza, const bool &detectada ,const QImage &Foto);
    void ReplyMatriculaFotoRemolque(const QString &matricula, const QString &confianza, const bool &detectada ,const QImage &Foto);
    /** END PROCESAR **************************************************************/

};

}
}
#endif // NEWSAGESALPRTASK_H
