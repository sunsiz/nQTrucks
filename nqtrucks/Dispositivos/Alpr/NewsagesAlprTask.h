#ifndef NEWSAGESALPRTASK_H
#define NEWSAGESALPRTASK_H

#include <QObject>
#include <QImage>
#include <nqtglobal.h>

#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "alpr.h"
#include "state_detector.h"

namespace nQTrucks {
namespace Devices {
    using namespace alpr;

class NewsagesAlprTask : public QObject{
    Q_OBJECT
public:
    NewsagesAlprTask(int nDevice=0, QSettings *_appsettings=0,QObject *parent = 0);
    ~NewsagesAlprTask();

    /** SETTINGS **/
private:
    QString   m_configroot;
    int       m_nDevice;
    void      loadconfig();
    QSettings *m_settings;
    t_Plank    m_Plank;
    void setPlank(QString A,QString B, QString C);

public slots:
    void setFotoCamara(const QImage &Foto){m_fotocamara = new QImage(Foto);}
    /** END SETTINGS **/

private:
    QImage *m_fotocamara;
    cv::Mat QImage2cvMat(QImage image);
    QImage  cvMat2QImage(const cv::Mat &image);
private:
    Alpr *matricula;
    Alpr *remolque;

public slots:
    void procesarBlancas();
    void procesarRojas();
signals:
    void ReplyMatriculaFoto(const QImage &Foto);
    void ReplyMatriculaFotoRemolque(const QImage &Foto);
    void workFinished();
};

}
}
#endif // NEWSAGESALPRTASK_H
