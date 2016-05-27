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
    QString Prewarp1() const;
    QString Prewarp2() const;
private:
    QString   m_configroot;
    int       m_nDevice;
    void      loadconfig();
    QSettings *m_settings;
    t_Plank    m_Plank;    
    void setPlank(const QString &A1, const QString &B1, const QString &C1,
                  const QString &A2, const QString &B2, const QString &C2);
    QString  m_prewarp1;
    QString  m_prewarp2;
    void setPrewarp(const QString &prewarp1, const QString &prewarp2);
    /** END SETTINGS ****************************************************/


    /** CALIBRACION **************************************************/
private:
    cv::Mat m_FotoCalibradaBlancosCV;
    cv::Mat m_FotoCalibradaRojosCV;
    cv::Mat apply_prewarp1(const cv::Mat &img);
    cv::Mat apply_prewarp2(const cv::Mat &img);
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
private:
   t_MatriculaResults *m_matricularesult;
    /** TODO PASAR A ESTRUCTURA t_MatriculaResults **/
signals:
    void ReplyMatriculaFoto        (const t_MatriculaResults &tempResults);
    void ReplyMatriculaFotoRemolque(const t_MatriculaResults &tempResultsRemolque);
    /** END PROCESAR **************************************************************/

};

}
}
#endif // NEWSAGESALPRTASK_H
