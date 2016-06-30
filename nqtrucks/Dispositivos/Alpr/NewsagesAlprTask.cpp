#include "NewsagesAlprTask.h"

#include "alpr.h"
#include "state_detector.h"
#include "prewarp.h"

#include <QByteArray>
#include <QBuffer>

#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QStringList>


namespace nQTrucks {
    namespace Devices {
    using namespace alpr;


/** CONSTRUCTOR TAREAS **/
NewsagesAlprTask::NewsagesAlprTask(int _nDevice, int _nType,Registros::MatriculaResults *_results, QSettings *_appsettings, QObject *parent)
    : QObject(parent)
    , m_settings(_appsettings)
    , m_matricularesult(_results)
{
    setlocale(LC_NUMERIC, "C");
    setNDevice(_nDevice);
    setNType(_nType);

    //m_matricularesult->convertirFotos();

    loadconfig();
}




/** END CONSTRUCTOR *****************************************************************************/


/** SETTINGS **/
void NewsagesAlprTask::loadconfig()
{
    m_config_file = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("config/openalpr.conf");

    switch (m_nDevice) {
    case 0:
        setPlank(  m_settings->value(QString(ALPR) + "/planka1","0").toString(),
                   m_settings->value(QString(ALPR) + "/plankb1","0").toString(),
                   m_settings->value(QString(ALPR) + "/plankc1","0").toString());
        setPrewarp(m_settings->value(QString(ALPR) + "/prewarp1",default_prewarp).toString());

        break;
    case 1:
        setPlank(  m_settings->value(QString(ALPR) + "/planka2","0").toString(),
                   m_settings->value(QString(ALPR) + "/plankb2","0").toString(),
                   m_settings->value(QString(ALPR) + "/plankc2","0").toString());
        setPrewarp(m_settings->value(QString(ALPR) + "/prewarp2",default_prewarp).toString());
        break;
    }
}



void NewsagesAlprTask::setPlank(const QString &_plankA, const QString &_plankB, const QString &_plankC){
  m_plank.A=_plankA.toInt();
  m_plank.B=_plankB.toInt();
  m_plank.C=_plankC.toInt();
}

/** END SETTINGS **/


/** CALIBRACION **********************************************************/
void NewsagesAlprTask::calibrar()
{
    setFotoCalibrada();

}

void NewsagesAlprTask::setFotoCalibrada()
{
    cv::Mat channel[3];
    switch (getNType()) {
    case ALPR_PLANCK_BLANCO:
        //m_matricularesult->setOrigenFotoBlanca(m_matricularesult->camara->getOrigenFoto().clone());
        //cv::add(m_matricularesult->getOrigenFotoBlanca(),cv::Scalar(getPlank().C,getPlank().B,getPlank().A),m_matricularesult->getOrigenFotoBlanca());
        //cv::split(m_matricularesult->getOrigenFotoBlanca(), channel);
        //m_matricularesult->setOrigenFotoBlanca(channel[2] - channel[1] -   channel[2] + channel[0]); /** MEMORY LEAK **/
        m_matricularesult->setPlanckBlanco(getPlank());
        emit ReplyOriginalFotoBlanca(m_matricularesult->getOrigenFotoBlanca());
        break;
    case ALPR_PLANCK_ROJO:
//        m_matricularesult->setOrigenFotoRoja(m_matricularesult->camara->getOrigenFoto().clone());
//        cv::add(m_matricularesult->getOrigenFotoRoja(),cv::Scalar(getPlank().A,getPlank().B,getPlank().C),m_matricularesult->getOrigenFotoRoja());
//        cv::split(m_matricularesult->getOrigenFotoRoja(), channel);
//        cv::add(channel[0], channel[1], m_matricularesult->getOrigenFotoRoja()); /** MEMORY LEAK **/
//        cv::subtract(channel[2], channel[1], m_matricularesult->getOrigenFotoRoja());
        m_matricularesult->setPlanckRojo(getPlank());
        emit ReplyOriginalFotoRoja(m_matricularesult->getOrigenFotoRoja());
        break;
    }
        //Limpiar
    channel[0].release();
    channel[1].release();
    channel[2].release();
}

void NewsagesAlprTask::guardarPlanK()
{
//    m_MutexWrite.lock();
    switch (m_nDevice) {
    case 0:
        m_settings->setValue(QString(ALPR) + "/planka1",QString::number(getPlank().A));
        m_settings->setValue(QString(ALPR) + "/plankb1",QString::number(getPlank().B));
        m_settings->setValue(QString(ALPR) + "/plankc1",QString::number(getPlank().C));
        break;
    case 1:
        m_settings->setValue(QString(ALPR) + "/planka2",QString::number(getPlank().A));
        m_settings->setValue(QString(ALPR) + "/plankb2",QString::number(getPlank().B));
        m_settings->setValue(QString(ALPR) + "/plankc2",QString::number(getPlank().C));
        break;
    }
//    m_MutexWrite.unlock();
}


cv::Mat NewsagesAlprTask::apply_prewarp(const cv::Mat &img){
    alpr::Config config("truck",m_config_file.toStdString());
    config.prewarp = getPrewarp().toStdString();
    config.setDebug(false);
    alpr::PreWarp prewarp(&config);
    cv::Mat imgprewarp=prewarp.warpImage(img.clone());
    prewarp.clear();
    return  imgprewarp;
}

/** PROCESAR *******************************************************************************/

void NewsagesAlprTask::procesar(){
    switch (getNType()) {
    case ALPR_PLANCK_BLANCO:
        procesarBlancas();
        break;
    case ALPR_PLANCK_ROJO:
        procesarRojas();
        break;
    }
}

    /** BLANCAS **/
void NewsagesAlprTask::procesarBlancas()
{
    Alpr *matricula;
    matricula = new Alpr("truck", m_config_file.toStdString());
    matricula->setDefaultRegion("truck");
    matricula->setTopN(1);
    //matricula->setPrewarp(getPrewarp().toStdString());
    if (matricula->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        //Algoritmo Plank Blancas
        int c=0;
        do{
            int b=0;
            do{
                //CONVERSION de BLANCOS
                calibrar();
                // RECONOCER
                std::vector<AlprRegionOfInterest> regionsOfInterest={};
                AlprResults results = matricula->recognize(m_matricularesult->getOrigenFotoBlanca().data,
                                                           m_matricularesult->getOrigenFotoBlanca().elemSize(),
                                                           m_matricularesult->getOrigenFotoBlanca().cols,
                                                           m_matricularesult->getOrigenFotoBlanca().rows,
                                                           regionsOfInterest);
                for (uint i = 0; i < results.plates.size(); i++){
                    AlprPlateResult plate = results.plates[i];
                    for (uint k = 0; k < plate.topNPlates.size(); k++){
                        AlprPlate candidate = plate.topNPlates[k];
                        //Auto ajuste Plank BLANCAS
                        if (candidate.matches_template  && m_matricularesult->getMatriculaPrecisionA() < candidate.overall_confidence){
                            m_matricularesult->setMatriculaDetectedA(  candidate.matches_template);
                            m_matricularesult->setMatriculaPrecisionA( candidate.overall_confidence);
                            m_matricularesult->setMatriculaPrecisionAs(QString::number(m_matricularesult->getMatriculaPrecisionA(),'g',6));
                            m_matricularesult->setMatriculaA(          QString::fromStdString(candidate.characters));
                            cv::Rect rect = cv::Rect(plate.plate_points[0].x , plate.plate_points[0].y,
                                                     plate.plate_points[2].x - plate.plate_points[0].x,
                                                     plate.plate_points[2].y - plate.plate_points[0].y);
                            cv::Mat _resize;
                            cv::resize(cv::Mat(m_matricularesult->camara->getOrigenFoto(),rect),_resize,matriculaSize);
                            m_matricularesult->setMatriculaFotoA(_resize);
                            _resize.release();
                        }
                    }
                }
               regionsOfInterest.clear();
               m_plank.B=b;
               b++;
           }while( b<=m_retry_panks && m_matricularesult->getMatriculaDetectedA()!=true);
           m_plank.C=c;
           c++;
       }while( c<=m_retry_panks &&  m_matricularesult->getMatriculaDetectedA()!=true);
    }
    delete matricula;
    guardarPlanK();
    emit ReplyOriginalFotoBlanca(m_matricularesult->getOrigenFotoBlanca());
    //m_matricularesult->convertirFotos();
    emit ReplyMatriculaFoto();
}


    /** ROJAS **/
void NewsagesAlprTask::procesarRojas()
{
    Alpr *remolque;
    remolque  = new Alpr("eur", m_config_file.toStdString());
    remolque->setTopN(1);
    remolque->setDefaultRegion("eur");
    //remolque->setPrewarp(getPrewarp().toStdString());
    if (remolque->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        //Algoritmo Plank ROJAS
        int c=0;
        do{
            int b=0;
            do{
                //CONVERSION de ROJA
                calibrar();
                // RECONOCER
                std::vector<AlprRegionOfInterest> regionsOfInterest={};
                AlprResults results = remolque->recognize(m_matricularesult->getOrigenFotoRoja().data,
                                                          m_matricularesult->getOrigenFotoRoja().elemSize(),
                                                          m_matricularesult->getOrigenFotoRoja().cols,
                                                          m_matricularesult->getOrigenFotoRoja().rows,
                                                          regionsOfInterest);
                if(results.plates.size()>0){
                    for (uint i = 0; i < results.plates.size(); i++){
                        AlprPlateResult plate = results.plates[i];
                        for (uint k = 0; k < plate.topNPlates.size(); k++){
                            AlprPlate candidate = plate.topNPlates[k];

                            if (candidate.matches_template  && m_matricularesult->getMatriculaPrecisionB() < candidate.overall_confidence){
                                m_matricularesult->setMatriculaDetectedB   ( candidate.matches_template);
                                m_matricularesult->setMatriculaPrecisionB  ( candidate.overall_confidence);
                                m_matricularesult->setMatriculaPrecisionBs ( QString::number(m_matricularesult->getMatriculaPrecisionB(),'g',6));
                                m_matricularesult->setMatriculaB           ( QString::fromStdString(candidate.characters));
                                cv::Rect rect = cv::Rect(plate.plate_points[0].x  ,plate.plate_points[0].y,
                                                         plate.plate_points[2].x - plate.plate_points[0].x,
                                                         plate.plate_points[2].y - plate.plate_points[0].y);
                                cv::Mat _resize;
                                cv::resize(cv::Mat(m_matricularesult->camara->getOrigenFoto(),rect),_resize,matriculaSize);
                                m_matricularesult->setMatriculaFotoB(_resize);
                            }
                       }
                    }
                }
                regionsOfInterest.clear();
                m_plank.B=b;
                b++;
            }while( b<=m_retry_panks && m_matricularesult->getMatriculaDetectedB()!=true);
            m_plank.C=c;
            c++;
        }while( c<=m_retry_panks &&  m_matricularesult->getMatriculaDetectedB()!=true);
    }
    delete remolque;
    guardarPlanK();
    emit ReplyOriginalFotoRoja(m_matricularesult->getOrigenFotoRoja());
    //m_matricularesult->convertirFotos();
    emit ReplyMatriculaFoto();

}
/** END PROCESAR ************************************************************************/
} /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/




