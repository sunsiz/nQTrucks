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


const int matriculaNewWidth = 520;
const int matriculaNewHeight = 110;
const cv::Size matriculaSize(matriculaNewWidth,matriculaNewHeight);
/** CONSTRUCTOR TAREAS **/
NewsagesAlprTask::NewsagesAlprTask(int _nDevice, cv::Mat _fotoCamara, QSettings *_appsettings, QObject *parent)
    : QObject(parent)
    , m_FotoCamara(_fotoCamara)
    , m_nDevice(_nDevice)
    , m_settings(_appsettings)
    , m_matricularesult(new t_MatriculaResults)
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<t_Plank>("t_Plank");
    qRegisterMetaType<MatriculaResults>("t_MatriculaResults");

    setlocale(LC_NUMERIC, "C");
    loadconfig();

//    bool detectada=false;
//    int b=0;
//    int c=0;
//    while( b!=m_retry_pankb && detectada!=true){
//        if (b==32) detectada=true;
//        while( c!=m_retry_pankb && detectada!=true){
//            if (c==32) detectada=true;
//            qDebug() << "retryC=" << c << " detectada=" << detectada;
//            c+=2;
//        }
//        qDebug() << "retryB=" << b << " detectada=" << detectada;
//        b+=2;
//    }


//    for (int b=0; b <= m_retry_pankb; b+=2){
//        for (int c=0; c<=m_retry_pankc; c+=2){
//            qDebug() << "retryC=" << c;
//        }
//        qDebug() << "retryB=" << b;

//    }


}

NewsagesAlprTask::~NewsagesAlprTask()
{
    m_FotoCamara.release();
    m_FotoCalibradaBlancosCV.release();
    m_FotoCalibradaRojosCV.release();
    this->deleteLater();
}
/** END CONSTRUCTOR *****************************************************************************/


/** SETTINGS **/
void NewsagesAlprTask::loadconfig()
{
    m_config_file = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("config/openalpr.conf");
    switch (m_nDevice) {
    case 0:
        setPlank(m_settings->value("planka1","0").toString(),
                 m_settings->value("plankb1","0").toString(),
                 m_settings->value("plankc1","0").toString());
        setPrewarp(m_settings->value("prewarp1","").toString());
        break;
    case 1:
        setPlank(m_settings->value("planka2","0").toString(),
                 m_settings->value("plankb2","0").toString(),
                 m_settings->value("plankc2","0").toString());
        setPrewarp(m_settings->value("prewarp2","").toString());
        break;
    }
}

t_Plank NewsagesAlprTask::getPlank() const{
    return m_plank;
}

void NewsagesAlprTask::setPlank(const QString &A, const QString &B, const QString &C){
  m_plank.A=A.toInt();
  m_plank.B=B.toInt();
  m_plank.C=C.toInt();
  qDebug() << "plank" << m_nDevice <<"=" << m_plank.A << "," << m_plank.B << "," << m_plank.C;
}

QString NewsagesAlprTask::getPrewarp() const{
    return m_prewarp;
}


void NewsagesAlprTask::setPrewarp(const QString &prewarp){
    m_prewarp=prewarp;
    qDebug() << "actual prewarp" << m_nDevice <<": " << m_prewarp;
}
/** END SETTINGS **/


/** CALIBRACION **********************************************************/
void NewsagesAlprTask::calibrarBlanco()
{
    setFotoCalibrada(0);
    emit workFinished();
}

void NewsagesAlprTask::calibrarRojo()
{
    setFotoCalibrada(1);
    emit workFinished();
}

void NewsagesAlprTask::setFotoCalibrada(int n)
{
    // CALIBRACION —---»» PARAMETRIZAR :::::
    cv::Mat img = m_FotoCamara.clone();
    cv::Mat channel[3];
    switch (n) {
    case 0:
        cv::add(img.clone(),cv::Scalar(getPlank().C,getPlank().B,getPlank().A),img);
        cv::split(img, channel);
        img = channel[2] - channel[1] -   channel[2] + channel[0];
        m_FotoCalibradaBlancosCV = img.clone();
        emit ReplyOriginalFotoBlanca(apply_prewarp(m_FotoCalibradaBlancosCV.clone()));
        //Limpiar
        channel[0].release();
        channel[1].release();
        channel[2].release();
        break;
    case 1:
        cv::add(img.clone(),cv::Scalar(getPlank().A,getPlank().B,getPlank().C),img);
        cv::split(img, channel);
        cv::add(channel[0], channel[1], img);
        cv::subtract(channel[2], channel[1], img);
        m_FotoCalibradaRojosCV = img.clone();
        emit ReplyOriginalFotoRoja(apply_prewarp(m_FotoCalibradaRojosCV.clone()));
        //Limpiar
        channel[0].release();
        channel[1].release();
        channel[2].release();
        break;
    default:
        break;
    }
    //Limpiar
    img.release();

}

void NewsagesAlprTask::guardarPlanK()
{
    switch (m_nDevice) {
    case 0:
        m_settings->beginGroup(ALPR);
        m_settings->setValue("planka1",QString::number(getPlank().A));
        m_settings->setValue("plankb1",QString::number(getPlank().B));
        m_settings->setValue("plankc1",QString::number(getPlank().C));
        m_settings->endGroup();
        break;
    case 1:
        m_settings->beginGroup(ALPR);
        m_settings->setValue("planka2",QString::number(getPlank().A));
        m_settings->setValue("plankb2",QString::number(getPlank().B));
        m_settings->setValue("plankc2",QString::number(getPlank().C));
        m_settings->endGroup();
        break;
    }
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
    /** BLANCAS **/
void NewsagesAlprTask::procesarBlancas()
{

    Alpr *matricula;
    matricula = new Alpr("truck", m_config_file.toStdString());
    matricula->setDefaultRegion("truck");
    matricula->setTopN(1);
    matricula->setPrewarp(getPrewarp().toStdString());
    //Respuesta por defecto
    m_matricularesult->OrigenFoto=m_FotoCamara.clone();
    m_matricularesult->MatriculaFotoA = cv::Mat::zeros( matriculaSize, CV_8UC3 );
    m_matricularesult->MatriculaFotoA = cv::Scalar( 0, 0, 255 );
    m_matricularesult->MatriculaA="";
    m_matricularesult->MatriculaPrecisionA=0;
    m_matricularesult->MatriculaPrecisionAs="0%";
    if (matricula->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        //Algoritmo Plank Blancas
        int c=0;
        do{
            int b=0;
            do{
                //CONVERSION de BLANCOS
                setFotoCalibrada(0);
                m_matricularesult->OrigenFotoBlanca=m_FotoCalibradaBlancosCV.clone();
                // RECONOCER
                std::vector<AlprRegionOfInterest> regionsOfInterest;
                AlprResults results = matricula->recognize(m_matricularesult->OrigenFotoBlanca.data,
                                                           m_matricularesult->OrigenFotoBlanca.elemSize(),
                                                           m_matricularesult->OrigenFotoBlanca.cols,
                                                           m_matricularesult->OrigenFotoBlanca.rows,
                                                           regionsOfInterest);
                for (uint i = 0; i < results.plates.size(); i++){
                    AlprPlateResult plate = results.plates[i];
                    for (uint k = 0; k < plate.topNPlates.size(); k++){
                        AlprPlate candidate = plate.topNPlates[k];
                        //Auto ajuste Plank BLANCAS
                        if (candidate.matches_template  && m_matricularesult->MatriculaPrecisionA < candidate.overall_confidence){
                            cv::Rect rect = cv::Rect(plate.plate_points[0].x , plate.plate_points[0].y,
                                                     plate.plate_points[2].x - plate.plate_points[0].x,
                                                     plate.plate_points[2].y - plate.plate_points[0].y);
                            //cv::Mat img_resize = m_matricularesult->OrigenFoto.clone();

                            cv::resize(cv::Mat(m_matricularesult->OrigenFoto,rect)
                                       ,m_matricularesult->MatriculaFotoA,
                                       matriculaSize);

                            m_matricularesult->MatriculaDetectedA   = candidate.matches_template;
                            m_matricularesult->MatriculaA           = QString::fromStdString(candidate.characters);
                            m_matricularesult->MatriculaPrecisionA  = candidate.overall_confidence;
                            m_matricularesult->MatriculaPrecisionAs = QString::number(m_matricularesult->MatriculaPrecisionA,'g',6);

                            if(m_matricularesult->MatriculaDetectedA){
                                m_matricularesult->OrigenFotoBlanca = apply_prewarp(m_FotoCalibradaBlancosCV.clone());
                                guardarPlanK();
                            }
                            qDebug() << "  blanca  - " << QString::fromStdString(candidate.characters)
                                     << "\t precision: "
                                     << m_matricularesult->MatriculaPrecisionA << "% "
                                     << candidate.matches_template << endl;
                        }
                    }
                }
               regionsOfInterest.clear();
               qDebug() << "BLANCAS retry (B/C)= (" << getPlank().B << "/" << getPlank().C << ") detectada=" << m_matricularesult->MatriculaDetectedB;
               emit ReplyOriginalFotoBlanca(apply_prewarp(m_FotoCalibradaBlancosCV.clone()));
               m_plank.B=b;
               b++;
           }while( b<=m_retry_panks && m_matricularesult->MatriculaDetectedA!=true);
           m_plank.C=c;
           c++;
       }while( c<=m_retry_panks &&  m_matricularesult->MatriculaDetectedA!=true);
    }
    emit ReplyMatriculaFoto(*m_matricularesult);
    emit workFinished();

    //limpiar
    delete matricula;
}


    /** ROJAS **/
void NewsagesAlprTask::procesarRojas()
{
    Alpr *remolque;
    remolque  = new Alpr("eur", m_config_file.toStdString());
    remolque->setTopN(1);
    remolque->setDefaultRegion("eur");
    remolque->setPrewarp(getPrewarp().toStdString());


    //Respuesta por defecto
    m_matricularesult->OrigenFoto = m_FotoCamara.clone();
    m_matricularesult->MatriculaFotoB = cv::Mat::zeros( matriculaSize, CV_8UC3 );
    m_matricularesult->MatriculaFotoB = cv::Scalar( 0, 0, 255 );
    m_matricularesult->MatriculaB="";
    m_matricularesult->MatriculaPrecisionB=0;
    m_matricularesult->MatriculaPrecisionBs="0%";
    m_matricularesult->MatriculaDetectedB=false;
    if (remolque->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        //Algoritmo Plank ROJAS
        int c=0;
        do{
            int b=0;
            do{
//                //CONVERSION de ROJA
                setFotoCalibrada(1);
                m_matricularesult->OrigenFotoRoja = m_FotoCalibradaRojosCV.clone();
                // RECONOCER
                std::vector<AlprRegionOfInterest> regionsOfInterest;
                AlprResults results = remolque->recognize(m_matricularesult->OrigenFotoRoja.data,
                                                          m_matricularesult->OrigenFotoRoja.elemSize(),
                                                          m_matricularesult->OrigenFotoRoja.cols,
                                                          m_matricularesult->OrigenFotoRoja.rows,
                                                          regionsOfInterest);
                if(results.plates.size()>0){
                    for (uint i = 0; i < results.plates.size(); i++){
                        AlprPlateResult plate = results.plates[i];
                        for (uint k = 0; k < plate.topNPlates.size(); k++){
                            AlprPlate candidate = plate.topNPlates[k];
                            if (candidate.matches_template  && m_matricularesult->MatriculaPrecisionB < candidate.overall_confidence){

                                cv::Rect rect = cv::Rect(plate.plate_points[0].x  ,plate.plate_points[0].y,
                                                         plate.plate_points[2].x - plate.plate_points[0].x,
                                                         plate.plate_points[2].y - plate.plate_points[0].y);

                                cv::resize(cv::Mat(m_matricularesult->OrigenFoto,rect),
                                           m_matricularesult->MatriculaFotoB,matriculaSize);

                                m_matricularesult->MatriculaDetectedB   = candidate.matches_template;
                                m_matricularesult->MatriculaB           = QString::fromStdString(candidate.characters);
                                m_matricularesult->MatriculaPrecisionB  = candidate.overall_confidence;
                                m_matricularesult->MatriculaPrecisionBs = QString::number(m_matricularesult->MatriculaPrecisionB,'g',6);

                                if(m_matricularesult->MatriculaDetectedB){
                                    m_matricularesult->OrigenFotoRoja = apply_prewarp(m_FotoCalibradaRojosCV.clone());
                                    guardarPlanK();
                                }
                                qDebug() << "  remolque  - " << QString::fromStdString(candidate.characters)
                                         << "\t precision: " << m_matricularesult->MatriculaPrecisionB << "% " <<
                                            candidate.matches_template << endl;
                            }
                       }
                    }
                }
                regionsOfInterest.clear();
                qDebug() << "ROJAS retry (B/C)= (" << getPlank().B << "/" << getPlank().C << ") detectada=" << m_matricularesult->MatriculaDetectedB;
                emit ReplyOriginalFotoRoja(apply_prewarp(m_FotoCalibradaRojosCV.clone()));
                m_plank.B=b;
                b++;
                //c+=2;
            }while( b<=m_retry_panks && m_matricularesult->MatriculaDetectedB!=true);
            m_plank.C=c;
            c++;
        }while( c<=m_retry_panks &&  m_matricularesult->MatriculaDetectedB!=true);
    }
    //*END ALGORITMO PLANKS

    emit ReplyMatriculaFotoRemolque(*m_matricularesult);
    emit workFinished();
    //limpiar
    delete remolque;
}
/** END PROCESAR ************************************************************************/



/** CONVERSORES *************************************************************************/
cv::Mat NewsagesAlprTask::convertQImage2Mat(const QImage &img){
    QByteArray baScene; // byte array with data
    QBuffer buffer(&baScene);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer,"JPG");

    const char* begin = reinterpret_cast<char*>(baScene.data());
    const char* end = begin + baScene.size();
    std::vector<char> pic(begin, end);
    buffer.close();
    baScene.clear();
    return cv::imdecode(pic,CV_LOAD_IMAGE_COLOR);
}

QImage NewsagesAlprTask::convertMat2QImage(const cv::Mat &src)
{
    QImage qtImg= QImage();
    if( !src.empty() && src.depth() == CV_8U ){
        if(src.channels() == 1){
            qtImg = QImage( (const unsigned char *)(src.data),
                            src.cols,
                            src.rows,
                            QImage::Format_Indexed8 );
        }
        else{
            cv::cvtColor( src, src, CV_BGR2RGB );
            qtImg = QImage( (const unsigned char *)(src.data),
                            src.cols,
                            src.rows,
                            src.step,
                            QImage::Format_RGB888 );
        }
    }
    return qtImg;
}

QByteArray NewsagesAlprTask::convertMat2Bytearray(const cv::Mat &img){

    QImage qtImg = convertMat2QImage(img);
    QByteArray baScene; // byte array with data
    QBuffer buffer(&baScene);
    buffer.open(QIODevice::WriteOnly);
    qtImg.save(&buffer,"JPG");
    buffer.close();
    return baScene;

}

/** END CONVERSORES ***********************************************************/
} /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/




