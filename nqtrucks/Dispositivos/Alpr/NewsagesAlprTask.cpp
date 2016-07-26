#include "NewsagesAlprTask.h"

#include "alpr.h"
//#include "state_detector.h"
#include "prewarp.h"


#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QStringList>


namespace nQTrucks {
    namespace Devices {
    using namespace alpr;


/** CONSTRUCTOR TAREAS **/
NewsagesAlprTask::NewsagesAlprTask(int _nDevice, int _nType,MatriculaResults *_results, QSettings *_appsettings, QObject *parent)
    : QObject(parent)
    , m_settings(_appsettings)
    , m_matricularesult(_results)
{
    setlocale(LC_NUMERIC, "C");
    setNDevice(_nDevice);
    setNType(_nType);
    loadconfig();
}
/** END CONSTRUCTOR *****************************************************************************/


/** SETTINGS ************************************************************************************/
void NewsagesAlprTask::loadconfig(){
    switch (m_nDevice) {
    case 0:
        m_config_file = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("config/trucks" + QString::number(m_nDevice+1) +  ".conf");
        setPlank(  m_settings->value(QString(ALPR) + "/planka1","0").toString(),
                   m_settings->value(QString(ALPR) + "/plankb1","0").toString(),
                   m_settings->value(QString(ALPR) + "/plankc1","0").toString());
        //setPrewarp(m_settings->value(QString(ALPR) + "/prewarp1","").toString());//default_prewarp).toString());

        break;
    case 1:
        m_config_file = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("config/trucks" + QString::number(m_nDevice+1) +  ".conf");
        setPlank(  m_settings->value(QString(ALPR) + "/planka2","0").toString(),
                   m_settings->value(QString(ALPR) + "/plankb2","0").toString(),
                   m_settings->value(QString(ALPR) + "/plankc2","0").toString());
        //setPrewarp(m_settings->value(QString(ALPR) + "/prewarp2","").toString());//default_prewarp).toString());
        break;
    }
}

void NewsagesAlprTask::setPlank(const QString &_plankA, const QString &_plankB, const QString &_plankC){
  m_plank.A=_plankA.toInt();
  m_plank.B=_plankB.toInt();
  m_plank.C=_plankC.toInt();
}
/** END SETTINGS **********************************************************************************/


/** CALIBRACION ***********************************************************************************/
void NewsagesAlprTask::calibrar(){ setFotoCalibrada();}

void NewsagesAlprTask::setFotoCalibrada(){
    switch (m_nDevice) {
    case 0:
        m_prewarp = m_settings->value(QString(ALPR) + "/prewarp1","").toString();//default_prewarp).toString());
        break;
    case 1:
        m_prewarp = m_settings->value(QString(ALPR) + "/prewarp2","").toString();//default_prewarp).toString());
        break;
    }
    //qDebug() << "Prewarp/Device: " << m_prewarp  << "    /    "  << m_nDevice;
    m_matricularesult->apply_prewarp(m_config_file,m_prewarp,m_nDevice);
    switch (getNType()) {
    case ALPR_PLANCK_BLANCO:
        m_matricularesult->setPlanckBlanco(getPlank());
        emit ReplyOriginalFotoBlanca(m_matricularesult->getOrigenFotoBlanca());
        break;
    case ALPR_PLANCK_ROJO:
        m_matricularesult->setPlanckRojo(getPlank());
        emit ReplyOriginalFotoRoja(  m_matricularesult->getOrigenFotoRoja());
        break;
    }
}

void NewsagesAlprTask::guardarPlanK(){
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
void NewsagesAlprTask::procesarBlancas(){   
    Alpr *matricula;
    matricula = new Alpr("truckB", m_config_file.toStdString());
    matricula->setDefaultRegion("truckB");
    matricula->setTopN(1);
    if (matricula->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
//        //Algoritmo Plank Blancas
        int c=0;
        int ccount=0;
        do{
            int b=0;
            int bcount=0;
            do{
                if (bcount == m_retry_panks && ccount==m_retry_panks)
                {
                    setPlank("0","140","100");
                    calibrar();
                }
                //CONVERSION de BLANCOS

                // RECONOCER
                std::vector<AlprRegionOfInterest> regionsOfInterest={};
                if (bcount ==0 && ccount==0){
                    AlprResults results = matricula->recognize(m_matricularesult->getOrigenFoto().data,
                                                               m_matricularesult->getOrigenFoto().elemSize(),
                                                               m_matricularesult->getOrigenFoto().cols,
                                                               m_matricularesult->getOrigenFoto().rows,
                                                               regionsOfInterest);
                    for (uint i = 0; i < results.plates.size(); i++){
                        AlprPlateResult plate = results.plates[i];
                        for (uint k = 0; k < plate.topNPlates.size(); k++){
                            AlprPlate candidate = plate.topNPlates[k];
                            //Auto ajuste Plank BLANCAS
                            if (candidate.matches_template  && m_matricularesult->getMatriculaPrecisionA() < candidate.overall_confidence){
                                m_matricularesult->setMatriculaDetectedA(  candidate.matches_template);
                                m_matricularesult->setMatriculaPrecisionA( candidate.overall_confidence);
                                m_matricularesult->setMatriculaA(          QString::fromStdString(candidate.characters));
                                cv::Rect rect = cv::Rect(plate.plate_points[0].x , plate.plate_points[0].y,
                                                         plate.plate_points[2].x - plate.plate_points[0].x,
                                                         plate.plate_points[2].y - plate.plate_points[0].y);
                                cv::Mat _resize;
                                cv::resize(cv::Mat(m_matricularesult->getOrigenFoto(),rect),_resize,MatriculaSize);
                                m_matricularesult->setMatriculaFotoA(_resize);
                                _resize.release();
                            }
                        }
                    }


                }else{
                    calibrar();
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
                                m_matricularesult->setMatriculaA(          QString::fromStdString(candidate.characters));
                                cv::Rect rect = cv::Rect(plate.plate_points[0].x , plate.plate_points[0].y,
                                                         plate.plate_points[2].x - plate.plate_points[0].x,
                                                         plate.plate_points[2].y - plate.plate_points[0].y);
                                cv::Mat _resize;
                                cv::resize(cv::Mat(m_matricularesult->getOrigenFotoPrewarp(),rect),_resize,MatriculaSize);
                                m_matricularesult->setMatriculaFotoA(_resize);
                                _resize.release();
                            }
                        }
                    }
                }
               regionsOfInterest.clear();
               m_plank.B=b;
               b=b+64;
               qDebug() << "blancas bcount: " << bcount;
               bcount++;
               emit ReplyOriginalFotoBlanca(m_matricularesult->getOrigenFotoBlanca());
           }while( bcount<=m_retry_panks && m_matricularesult->getMatriculaDetectedA()!=true);
           m_plank.C=c;
           c=c+64;
           qDebug() << "blancas ccount: " << ccount;
           ccount++;
       }while( ccount<=m_retry_panks &&  m_matricularesult->getMatriculaDetectedA()!=true);
    }
    delete matricula;
    //guardarPlanK();
    emit ReplyOriginalFotoBlanca(m_matricularesult->getOrigenFotoBlanca());
    emit ReplyMatriculaFoto();
}

    /** ROJAS **/
void NewsagesAlprTask::procesarRojas(){
    Alpr *remolque;
    remolque  = new Alpr("truckR", m_config_file.toStdString());
    remolque->setTopN(1);
    remolque->setDefaultRegion("truckR");
    if (remolque->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        //Algoritmo Plank ROJAS
        if (m_nDevice==0) m_retry_panks=0;
        int c=0;
        int ccount=0;
        do{
            int b=0;
            int bcount=0;
            do{
                //CONVERSION de ROJA
                if (bcount == m_retry_panks && ccount==m_retry_panks)
                {
                    setPlank("90","0","254");
                    calibrar();
                }

                // RECONOCER
                std::vector<AlprRegionOfInterest> regionsOfInterest={};
                if (bcount ==0 && ccount==0){
                    AlprResults results = remolque->recognize(m_matricularesult->getOrigenFoto().data,
                                                              m_matricularesult->getOrigenFoto().elemSize(),
                                                              m_matricularesult->getOrigenFoto().cols,
                                                              m_matricularesult->getOrigenFoto().rows,
                                                              regionsOfInterest);
                    if(results.plates.size()>0){
                        for (uint i = 0; i < results.plates.size(); i++){
                            AlprPlateResult plate = results.plates[i];
                            for (uint k = 0; k < plate.topNPlates.size(); k++){
                                AlprPlate candidate = plate.topNPlates[k];

                                if (candidate.matches_template  && m_matricularesult->getMatriculaPrecisionB() < candidate.overall_confidence){
                                    m_matricularesult->setMatriculaDetectedB   ( candidate.matches_template);
                                    m_matricularesult->setMatriculaPrecisionB  ( candidate.overall_confidence);
                                    m_matricularesult->setMatriculaB           ( QString::fromStdString(candidate.characters));
                                    cv::Rect rect = cv::Rect(plate.plate_points[0].x  ,plate.plate_points[0].y,
                                                             plate.plate_points[2].x - plate.plate_points[0].x,
                                                             plate.plate_points[2].y - plate.plate_points[0].y);
                                    cv::Mat _resize;
                                    cv::resize(cv::Mat(m_matricularesult->getOrigenFoto(),rect),_resize,MatriculaSize);
                                    m_matricularesult->setMatriculaFotoB(_resize);
                                }
                           }
                        }
                    }

                } else{
                    calibrar();
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
                                    m_matricularesult->setMatriculaB           ( QString::fromStdString(candidate.characters));
                                    cv::Rect rect = cv::Rect(plate.plate_points[0].x  ,plate.plate_points[0].y,
                                                             plate.plate_points[2].x - plate.plate_points[0].x,
                                                             plate.plate_points[2].y - plate.plate_points[0].y);
                                    cv::Mat _resize;
                                    cv::resize(cv::Mat(m_matricularesult->getOrigenFotoPrewarp(),rect),_resize,MatriculaSize);
                                    m_matricularesult->setMatriculaFotoB(_resize);
                                }
                           }
                        }
                    }
                }
                regionsOfInterest.clear();
                m_plank.B=b;
                b=b+64;
                bcount++;
                qDebug() << "rojas bcount: " << bcount;
                emit ReplyOriginalFotoRoja(m_matricularesult->getOrigenFotoRoja());
            }while( bcount<=m_retry_panks && m_matricularesult->getMatriculaDetectedB()!=true);
            m_plank.C=c;
            c=c+64;
            ccount++;
            qDebug() << "rojas ccount: " << ccount;
        }while( ccount<=m_retry_panks &&  m_matricularesult->getMatriculaDetectedB()!=true);
    }
    delete remolque;
    //guardarPlanK();
    //m_matricularesult->convertirFotos();
    emit ReplyOriginalFotoRoja(m_matricularesult->getOrigenFotoRoja());
    emit ReplyMatriculaFoto();
}
/** END PROCESAR ************************************************************************/
} /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/




