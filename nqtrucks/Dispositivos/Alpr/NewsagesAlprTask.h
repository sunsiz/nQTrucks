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
    explicit NewsagesAlprTask(int _nDevice=0, QImage _fotoCamara=QImage(), QSettings *_appsettings=0, QObject *parent = 0);
    ~NewsagesAlprTask();          

    //QImage getFotoCalibrada(){return m_FotoCalibrada;}



public slots:
    //QImage FotoCamara() const {return m_FotoCamara;}
    //void setFotoCamara();//QImage *FotoCamara);
    QImage FotoCamara() const {return m_FotoCamara;}
    void setFotoCamara();
    cv::Mat FotoCamaraCV() const {return m_FotoCamaraCV;}
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
private:
    QImage m_FotoCalibradaBlancos;
    QImage m_FotoCalibradaRojos;
    cv::Mat m_FotoCalibradaBlancosCV;
    cv::Mat m_FotoCalibradaRojosCV;
public slots:
    void calibrarBlanco();
    void calibrarRojo();
    QImage FotoCalibrada(int n) const;
    void setFotoCalibrada(int n);
signals:
    void ReplyOriginalFotoBlanca(const QImage &Foto);
    void ReplyOriginalFotoRoja(const QImage &Foto);
    //void onCalibrar();
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
