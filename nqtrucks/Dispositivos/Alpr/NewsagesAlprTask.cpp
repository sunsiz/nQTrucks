#include "NewsagesAlprTask.h"

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
        m_config_file = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("config/trucks" + QString::number(m_nDevice+1) +  ".conf");
        setPlank(  m_settings->value(QString(ALPR) + "/planka"+QString::number(m_nDevice+1) + QString::number(m_nType+1),"0").toString(),
                   m_settings->value(QString(ALPR) + "/plankb"+QString::number(m_nDevice+1) + QString::number(m_nType+1),"0").toString(),
                   m_settings->value(QString(ALPR) + "/plankc"+QString::number(m_nDevice+1) + QString::number(m_nType+1),"0").toString());
        //setPrewarp(m_settings->value(QString(ALPR) + "/prewarp1","").toString());//default_prewarp).toString());
}

void NewsagesAlprTask::setPlank(const QString &_plankA, const QString &_plankB, const QString &_plankC){
    switch (m_nType) {
    case ALPR_PLANCK_BLANCO:
        m_plankB.A=_plankA.toInt();
        m_plankB.B=_plankB.toInt();
        m_plankB.C=_plankC.toInt();
        break;
    case ALPR_PLANCK_ROJO:
        m_plankR.A=_plankA.toInt();
        m_plankR.B=_plankB.toInt();
        m_plankR.C=_plankC.toInt();
        break;
    }
}

Planck NewsagesAlprTask::getPlank() const{
    switch (m_nType) {
    case ALPR_PLANCK_BLANCO:
        return  m_plankB;
        break;
    case ALPR_PLANCK_ROJO:
        return  m_plankB;
        break;
    }
    return  m_plankB;
}
/** END SETTINGS **********************************************************************************/


/** CALIBRACION ***********************************************************************************/
void NewsagesAlprTask::calibrar(){ calibrarPlancks();}
void NewsagesAlprTask::calibrarPlancks(){
    switch (m_nDevice) {
    case 0:
        m_prewarp = m_settings->value(QString(ALPR) + "/prewarp1","").toString();//default_prewarp).toString());
        break;
    case 1:
        m_prewarp = m_settings->value(QString(ALPR) + "/prewarp2","").toString();//default_prewarp).toString());
        break;
    }
    m_matricularesult->apply_prewarp(m_config_file,m_prewarp,m_nDevice);
    switch (getNType()) {
    case ALPR_PLANCK_BLANCO:
        //qDebug() << "Blancas Prewarp/Device: " << m_prewarp  << "    /    "  << m_nDevice;
        m_matricularesult->setPlanckBlanco(m_plankB);
        emit ReplyOriginalFotoBlanca(m_matricularesult->getOrigenFotoBlanca());
        break;
    case ALPR_PLANCK_ROJO:
        //qDebug() << "Rojas Prewarp/Device: " << m_prewarp  << "    /    "  << m_nDevice;
        m_matricularesult->setPlanckRojo(m_plankR);
        emit ReplyOriginalFotoRoja(  m_matricularesult->getOrigenFotoRoja());
        break;
    }
}

void NewsagesAlprTask::setFotoCalibrada(){
    switch (m_nDevice) {
    case 0:
        m_prewarp = m_settings->value(QString(ALPR) + "/prewarp1","").toString();//default_prewarp).toString());
        break;
    case 1:
        m_prewarp = m_settings->value(QString(ALPR) + "/prewarp2","").toString();//default_prewarp).toString());
        break;
    }
    m_matricularesult->apply_prewarp(m_config_file,m_prewarp,m_nDevice);
    switch (getNType()) {
    case ALPR_PLANCK_BLANCO:
        //qDebug() << "Blancas Prewarp/Device: " << m_prewarp  << "    /    "  << m_nDevice;
        m_matricularesult->setPlanckBlanco(getPlank());
        emit ReplyOriginalFotoBlanca(m_matricularesult->getOrigenFotoBlanca());
        break;
    case ALPR_PLANCK_ROJO:
        //qDebug() << "Rojas Prewarp/Device: " << m_prewarp  << "    /    "  << m_nDevice;
        m_matricularesult->setPlanckRojo(getPlank());
        emit ReplyOriginalFotoRoja(  m_matricularesult->getOrigenFotoRoja());
        break;
    }
}

void NewsagesAlprTask::guardarPlanK(int _device, int _type, const Planck &_planck){
    m_settings->setValue(QString(ALPR) + "/planka"+QString::number(_device+1) + QString::number(_type+1),QString::number(_planck.A));
    m_settings->setValue(QString(ALPR) + "/plankb"+QString::number(_device+1) + QString::number(_type+1),QString::number(_planck.B));
    m_settings->setValue(QString(ALPR) + "/plankc"+QString::number(_device+1) + QString::number(_type+1),QString::number(_planck.C));
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
    /** BLANCAS **************************************************************************************/
bool NewsagesAlprTask::encontrarMatricula(Alpr *matricula, const cv::Mat &Foto, const cv::Mat &FototoCrop,const int alprType){
    bool encontrada=false;
    switch (alprType) {
    case ALPR_PLANCK_BLANCO:
        //qDebug() << "Blancas Planck: "  << getPlank().A << "/" << getPlank().B << "/" << getPlank().C;
        break;
    case ALPR_PLANCK_ROJO:
        //qDebug() << "Rojas Planck: "  << getPlank().A << "/" << getPlank().B << "/" << getPlank().C;
        break;
    }



    std::vector<AlprRegionOfInterest> regionsOfInterest={};
    AlprResults results = matricula->recognize(Foto.data,
                                               Foto.elemSize(),
                                               Foto.cols,
                                               Foto.rows,
                                               regionsOfInterest);
    for (uint i = 0; i < results.plates.size(); i++){
        AlprPlateResult plate = results.plates[i];
        for (uint k = 0; k < plate.topNPlates.size(); k++){
            AlprPlate candidate = plate.topNPlates[k];
            switch (alprType) {
            case ALPR_PLANCK_BLANCO:
                if (candidate.matches_template  && m_matricularesult->getMatriculaPrecisionA() < candidate.overall_confidence){
                    m_matricularesult->setMatriculaDetectedA(  candidate.matches_template);
                    m_matricularesult->setMatriculaPrecisionA( candidate.overall_confidence);
                    m_matricularesult->setMatriculaA(          QString::fromStdString(candidate.characters));
                    cv::Rect rect = cv::Rect(plate.plate_points[0].x , plate.plate_points[0].y,
                                             plate.plate_points[2].x - plate.plate_points[0].x,
                                             plate.plate_points[2].y - plate.plate_points[0].y);
                    cv::Mat _resize;
                    cv::resize(cv::Mat(FototoCrop,rect),_resize,MatriculaSize);
                    m_matricularesult->setMatriculaFotoA(_resize);
                    _resize.release();
                    emit ReplyOriginalFotoBlanca(Foto);
                }
                break;
            case ALPR_PLANCK_ROJO:
                if (candidate.matches_template  && m_matricularesult->getMatriculaPrecisionB() < candidate.overall_confidence){
                    m_matricularesult->setMatriculaDetectedB(  candidate.matches_template);
                    m_matricularesult->setMatriculaPrecisionB( candidate.overall_confidence);
                    m_matricularesult->setMatriculaB(          QString::fromStdString(candidate.characters));
                    cv::Rect rect = cv::Rect(plate.plate_points[0].x , plate.plate_points[0].y,
                                             plate.plate_points[2].x - plate.plate_points[0].x,
                                             plate.plate_points[2].y - plate.plate_points[0].y);
                    cv::Mat _resize;
                    cv::resize(cv::Mat(FototoCrop,rect),_resize,MatriculaSize);
                    m_matricularesult->setMatriculaFotoB(_resize);
                    _resize.release();
                    emit ReplyOriginalFotoRoja(Foto);
                }
                break;
            }
            encontrada=candidate.matches_template;
        }
    }
    regionsOfInterest.clear();


    return encontrada;

}

void NewsagesAlprTask::procesarBlancas(){   
    Alpr *matricula;
    matricula = new Alpr("truckB", m_config_file.toStdString());
    matricula->setDefaultRegion("truckB");
    matricula->setTopN(1);    
    if (matricula->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        bool encontrada=false;

        if(m_nDevice==0){
            encontrada=encontrarMatricula(matricula,
                                          m_matricularesult->getOrigenFoto(),
                                          m_matricularesult->getOrigenFoto(),
                                          ALPR_PLANCK_BLANCO);
            qDebug()  << "Buscar Blanca con la foto original:\t|Resultado:" << encontrada;

            if(!encontrada){
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoBlanca(),
                                              m_matricularesult->getOrigenFoto(),
                                              ALPR_PLANCK_BLANCO);
                qDebug() << "Buscar Blanca con Planck:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C;
            }

            if(!encontrada){
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoBlancaPrewarp(),
                                              m_matricularesult->getOrigenFotoPrewarp(),
                                              ALPR_PLANCK_BLANCO);
                qDebug() << "Buscar Blanca con Planck y Prewarp:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C << "\t|Prewarp:" << m_prewarp;
            }

            if(!encontrada){
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoPrewarp(),
                                              m_matricularesult->getOrigenFotoPrewarp(),
                                              ALPR_PLANCK_BLANCO);
                qDebug() <<"Buscar Blanca con Prewarp:\t|Resultado:" << encontrada;
            }
        }

        if (m_nDevice>0){
            if(!encontrada){
                setPlank("90","140","100");
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoBlanca(),
                                              m_matricularesult->getOrigenFoto(),
                                              ALPR_PLANCK_BLANCO);
                qDebug() << "Buscar Blanca con Planck Newsages1:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C;;
            }

            if(!encontrada){
                setPlank("90","140","100");
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoBlancaPrewarp(),
                                              m_matricularesult->getOrigenFotoPrewarp(),
                                              ALPR_PLANCK_BLANCO);
                qDebug() << "Buscar Blanca con Planck newsages1 y Prewarp:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C << "\t|Prewarp:" << m_prewarp;
            }

            if(!encontrada){
                setPlank("90","5","28");
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoBlanca(),
                                              m_matricularesult->getOrigenFoto(),
                                              ALPR_PLANCK_BLANCO);
                qDebug() << "Buscar Blanca con Planck Newsages2:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C;;
            }

            if(!encontrada){
                setPlank("90","5","28");
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoBlancaPrewarp(),
                                              m_matricularesult->getOrigenFotoPrewarp(),
                                              ALPR_PLANCK_BLANCO);
                qDebug() << "Buscar Blanca con Planck newsages2 y Prewarp:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C << "\t|Prewarp:" << m_prewarp;
            }
            if(!encontrada){
                setPlank("90","0","28");
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoBlanca(),
                                              m_matricularesult->getOrigenFoto(),
                                              ALPR_PLANCK_BLANCO);
                qDebug() << "Buscar Blanca con Planck Newsages3:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C;
            }

            if(!encontrada){
                setPlank("90","0","28");
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoBlancaPrewarp(),
                                              m_matricularesult->getOrigenFotoPrewarp(),
                                              ALPR_PLANCK_BLANCO);
                qDebug() << "Buscar Blanca con Planck newsages3 y Prewarp:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C << "\t|Prewarp:" << m_prewarp;
            }
        }

        delete matricula;
        if(encontrada){
            guardarPlanK(m_nDevice,m_nType,m_plankB);
        }
    }

    emit ReplyMatriculaFoto();
}

    /** ROJAS **/
void NewsagesAlprTask::procesarRojas(){
    Alpr *matricula;
    matricula  = new Alpr("truckR", m_config_file.toStdString());
    matricula->setTopN(1);
    matricula->setDefaultRegion("truckR");
    if (matricula->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        bool encontrada=false;

        encontrada=encontrarMatricula(matricula,
                                      m_matricularesult->getOrigenFoto(),
                                      m_matricularesult->getOrigenFoto(),
                                      ALPR_PLANCK_ROJO);
        qDebug()  << "Buscar Roja con la foto original:\t|Resultado:" << encontrada;

        if (m_nDevice>0){
            if(!encontrada){
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoRoja(),
                                              m_matricularesult->getOrigenFoto(),
                                              ALPR_PLANCK_ROJO);
                qDebug() << "Buscar Roja con Planck:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C;
            }

            if(!encontrada){
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoRojaPrewarp(),
                                              m_matricularesult->getOrigenFotoPrewarp(),
                                              ALPR_PLANCK_ROJO);
                qDebug() << "Buscar Roja con Planck y Prewarp:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C << "\t|Prewarp:" << m_prewarp;
            }

            if(!encontrada){
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoPrewarp(),
                                              m_matricularesult->getOrigenFotoPrewarp(),
                                              ALPR_PLANCK_ROJO);
                qDebug() <<"Buscar Rojo con Prewarp:\t|Resultado:" << encontrada;
            }

            if(!encontrada){
                setPlank("90","66","66");
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoRoja(),
                                              m_matricularesult->getOrigenFoto(),
                                              ALPR_PLANCK_ROJO);
                qDebug() << "Buscar Roja con Planck Newsages1:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C;;
            }

            if(!encontrada){
                setPlank("90","66","66");
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoRojaPrewarp(),
                                              m_matricularesult->getOrigenFotoPrewarp(),
                                              ALPR_PLANCK_ROJO);
                qDebug() << "Buscar Roja con Planck newsages1 y Prewarp:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C << "\t|Prewarp:" << m_prewarp;
            }

            if(!encontrada){
                setPlank("90","66","28");
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoRoja(),
                                              m_matricularesult->getOrigenFoto(),
                                              ALPR_PLANCK_ROJO);
                qDebug() << "Buscar Roja con Planck Newsages2:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C;;
            }

            if(!encontrada){
                setPlank("90","66","28");
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoRojaPrewarp(),
                                              m_matricularesult->getOrigenFotoPrewarp(),
                                              ALPR_PLANCK_ROJO);
                qDebug() << "Buscar Roja con Planck newsages2 y Prewarp:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C << "\t|Prewarp:" << m_prewarp;
            }
            if(!encontrada){
                setPlank("90","90","90");
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoRoja(),
                                              m_matricularesult->getOrigenFoto(),
                                              ALPR_PLANCK_ROJO);
                qDebug() << "Buscar Roja con Planck Newsages3:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C;
            }

            if(!encontrada){
                setPlank("90","90","90");
                calibrar();
                encontrada=encontrarMatricula(matricula,
                                              m_matricularesult->getOrigenFotoRojaPrewarp(),
                                              m_matricularesult->getOrigenFotoPrewarp(),
                                              ALPR_PLANCK_ROJO);
                qDebug() << "Buscar Roja con Planck newsages3 y Prewarp:\t|Resultado:" << encontrada << "\t|Planck:" << getPlank().A << "/" << getPlank().B << "/" << getPlank().C << "\t|Prewarp:" << m_prewarp;
            }
        }
        delete matricula;
        if(encontrada){
            guardarPlanK(m_nDevice,m_nType,m_plankR);
        }
    }
    emit ReplyMatriculaFoto();
}

/** END PROCESAR ************************************************************************/
} /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/




