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
    NewsagesAlprTask(int nDevice=0, QSettings *_appsettings=0,QObject *parent = 0);
    ~NewsagesAlprTask();          

    QImage getFotoCalibrada(){return m_FotoCalibrada;}
public slots:
    QImage FotoCamara() const {return m_FotoCamara;}
    void setFotoCamara(const QImage &FotoCamara) {m_FotoCamara = FotoCamara;}

private:
    QImage m_FotoCamara;
    QImage m_FotoCalibrada;
signals:
    void workFinished();

    /** CONVERSORES ********************************************************/
private:
    cv::Mat QImage2cvMat(const QImage  &image);
    QImage  cvMat2QImage(const cv::Mat &image);
    /** END CONVERSORES **********************************************************/


    /** CALIBRACION **************************************************/
public slots:
    void calibrarBlanco();
    void calibrarRojo();
signals:
    void onCalibrar();
    /** END CALIBRACION ************************************************/

    /** SETTINGS *******************************************************/
private:
    QString   m_configroot;
    int       m_nDevice;
    void      loadconfig();
    QSettings *m_settings;
    t_Plank    m_Plank;
    void setPlank(QString A,QString B, QString C);
    /** END SETTINGS ****************************************************/








/** PROCESAR **************************************************************/
public slots:
    void procesarBlancas();
    void procesarRojas();

signals:
    void ReplyMatriculaFoto        (const QString &matricula, const QString &confianza, const bool &detectada ,const QImage &Foto);
    void ReplyMatriculaFotoRemolque(const QString &matricula, const QString &confianza, const bool &detectada ,const QImage &Foto);
    //void workFinished();
};

}
}
#endif // NEWSAGESALPRTASK_H
