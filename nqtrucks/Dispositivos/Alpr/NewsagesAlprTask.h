#ifndef NEWSAGESALPRTASK_H
#define NEWSAGESALPRTASK_H

#include <QObject>
#include "Registros/MatriculaResults.h"
#include "alpr.h"

namespace nQTrucks {
namespace Devices {

class NewsagesAlprTask : public QObject{
    Q_OBJECT
public:
    explicit NewsagesAlprTask(int _nDevice=0, int _nType=0, MatriculaResults *_results=0,  QSettings *_appsettings=0, QObject *parent = 0);
signals:
    void workFinished();

    /** SETTINGS *******************************************************/
private:
    QSettings *m_settings;
    QString    m_config_file;
    void       loadconfig();

private:
    int     m_nDevice;
    void    setNDevice(  int nDevice) {m_nDevice = nDevice;}
    int     getNDevice() const {return m_nDevice;}

    int     m_nType;
    void    setNType(  int nType)   {m_nType = nType;}
    int     getNType() const {return m_nType;}

private:
    //Planck m_plank;
    Planck m_plankB;
    Planck m_plankR;

    void   setPlank(  const QString &_plankA, const QString &_plankB, const QString &_plankC);
    Planck getPlank() const;

    QString  m_prewarp;
//    void    setPrewarp(  const QString &prewarp){m_prewarp=prewarp;}
//    QString getPrewarp() const{return m_prewarp;}
    /** END SETTINGS ****************************************************/


    /** CALIBRACION **************************************************/
private:
    void setFotoCalibrada();
    //cv::Mat apply_prewarp(const cv::Mat &img);
    /** Algoritmo AUTOCALIBRACION **/
    int  m_retry_panks=2;
    void guardarPlanK(int _device, int _type, const Planck &_planck);
    /** END ALGORITMO AUTOCALIBRACION**/

public slots:
    void calibrar();
signals:
    void ReplyOriginalFotoBlanca(const cv::Mat &Foto);
    void ReplyOriginalFotoRoja(  const cv::Mat &Foto);
    /** END CALIBRACION ************************************************/

    /** PROCESAR **************************************************************/
public slots:
    void procesar();
private:
   MatriculaResults *m_matricularesult;
   void procesarBlancas();
   bool encontrarMatricula(alpr::Alpr * matricula, const cv::Mat &Foto, const cv::Mat &FototoCrop, const int alprType);
   bool procesarBlancasPlanck(alpr::Alpr *matricula);
   bool procesarBlancasPlanckPrewarp(alpr::Alpr *matricula);

   void procesarRojas();
   void calibrarPlancks();
signals:
   void ReplyMatriculaFoto();
    /** END PROCESAR **************************************************************/
};

}
}
#endif // NEWSAGESALPRTASK_H
