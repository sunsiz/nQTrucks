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
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<t_Plank>("t_Plank");
    qRegisterMetaType<MatriculaResults>("t_MatriculaResults");

    setlocale(LC_NUMERIC, "C");
    loadconfig();
}

NewsagesAlprTask::~NewsagesAlprTask()
{
    m_FotoCamara.release();
    m_FotoCalibradaBlancosCV.release();
    m_FotoCalibradaRojosCV.release();
    //m_settings->clear();
    this->deleteLater();
}
/** END CONSTRUCTOR *****************************************************************************/


/** SETTINGS **/
void NewsagesAlprTask::loadconfig()
{
    m_configroot = (QString(ALPR));
    m_settings->beginGroup(m_configroot);
    setPlank(QString::number(m_settings->value(QString("planka"+QString::number(m_nDevice))).toInt()),
             QString::number(m_settings->value(QString("plankb"+QString::number(m_nDevice))).toInt()),
             QString::number(m_settings->value(QString("plankc"+QString::number(m_nDevice))).toInt()));
    setPrewarp(m_settings->value(QString("prewarp"+QString::number(m_nDevice))).toString());
    m_settings->endGroup();
}

t_Plank NewsagesAlprTask::Plank() const
{

    return m_Plank;
}

void NewsagesAlprTask::setPlank(const QString &A,const QString &B, const QString &C){
  m_Plank.A=A.toInt();
  m_Plank.B=B.toInt();
  m_Plank.C=C.toInt();
  qDebug() << "plank=" << m_Plank.A << "," << m_Plank.B << "," << m_Plank.C;
}

QString NewsagesAlprTask::prewarp() const
{
    return m_prewarp;
}
void NewsagesAlprTask::setPrewarp(QString prewarp){
    m_prewarp=prewarp;
  qDebug() << "actual prewarp: " << m_prewarp;
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


//cv::Mat NewsagesAlprTask::FotoCalibrada(int n) const
//{
//    switch (n) {
//    case 0:
//        return m_FotoCalibradaBlancosCV;
//        break;
//    case 1:
//        return m_FotoCalibradaRojosCV;
//        break;
//    }
//    return m_FotoCalibradaBlancosCV;
//}

void NewsagesAlprTask::setFotoCalibrada(int n)
{
    // CALIBRACION —---»» PARAMETRIZAR :::::
    cv::Mat img = m_FotoCamara.clone();
    cv::Mat channel[3];
    switch (n) {
    case 0:
        loadconfig();
        cv::add(img.clone(),cv::Scalar(Plank().C,Plank().B,Plank().A),img);
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
        loadconfig();
        cv::add(img.clone(),cv::Scalar(Plank().A,Plank().B,Plank().C),img);
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

cv::Mat NewsagesAlprTask::apply_prewarp(const cv::Mat &img){
    loadconfig();
    alpr::Config config("truck");
    config.prewarp = prewarp().toStdString();
    config.setDebug(false);
    alpr::PreWarp prewarp(&config);
    cv::Mat imgprewarp = prewarp.warpImage(img.clone());

    //Limpiar
    prewarp.clear();

    return  imgprewarp;
}

/** PROCESAR *******************************************************************************/
    /** BLANCAS **/
void NewsagesAlprTask::procesarBlancas()
{
    QString filename = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("config/openalpr.conf");
    Alpr *matricula;
    matricula = new Alpr("truck", filename.toStdString());
    matricula->setDefaultRegion("truck");
    matricula->setTopN(1);
    matricula->setPrewarp(prewarp().toStdString());
    //Respuesta por defecto
    float confianza=0;
    //double dconfianza=0;
    QString tconfianza="0";
    bool detectada= false;
    QString matriculadetected="";
    cv::Mat image_matricula = cv::Mat::zeros( matriculaSize, CV_8UC3 );
    image_matricula = cv::Scalar( 0, 0, 255 );

    if (matricula->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        image_matricula = m_FotoCamara.clone();
        //CONVERSION de BLANCOS
        setFotoCalibrada(0);
        cv::Mat img = m_FotoCalibradaBlancosCV.clone();
        // RECONOCER
        std::vector<AlprRegionOfInterest> regionsOfInterest;
        AlprResults results = matricula->recognize(img.data, img.elemSize(), img.cols, img.rows,regionsOfInterest);
        for (uint i = 0; i < results.plates.size(); i++){
            AlprPlateResult plate = results.plates[i];
            for (uint k = 0; k < plate.topNPlates.size(); k++){
                AlprPlate candidate = plate.topNPlates[k];
                //Auto ajuste Plank BLANCAS
                if (candidate.matches_template  && confianza < candidate.overall_confidence){
                    confianza = candidate.overall_confidence;
                    //dconfianza = (double)candidate.overall_confidence;
                    tconfianza = QString::number(confianza,'g',6);
                    detectada = candidate.matches_template;
                    matriculadetected = QString::fromStdString(candidate.characters);
                    cv::Rect rect = cv::Rect(plate.plate_points[0].x,plate.plate_points[0].y,
                                             plate.plate_points[2].x - plate.plate_points[0].x,
                                             plate.plate_points[2].y - plate.plate_points[0].y);
                    cv::resize(cv::Mat(image_matricula,rect),image_matricula,matriculaSize);

//                    qDebug() << "  blanca  - " << QString::fromStdString(candidate.characters) << "\t precision: " << confianza << "% " <<
//                                candidate.matches_template << endl;
                }else{
                //Algoritmo PlankC blancas
                qDebug() << "plank=" << m_Plank.A << "," << m_Plank.B << "," << m_Plank.C;
                }
            }
        }
       regionsOfInterest.clear();
       img.release();
    }
    emit ReplyMatriculaFoto(matriculadetected,tconfianza,detectada,image_matricula.clone(),convertMat2Bytearray(image_matricula.clone()));
    emit workFinished();

    //limpiar
    image_matricula.release();
    delete matricula;
}


    /** ROJAS **/
void NewsagesAlprTask::procesarRojas()
{
    QString filename = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("config/openalpr.conf");
    Alpr *remolque;
    remolque  = new Alpr("eur", filename.toStdString());
    remolque->setTopN(1);
    remolque->setDefaultRegion("eur");
    remolque->setPrewarp(prewarp().toStdString());
    //Respuesta por defecto
    float confianza=0;
    //double dconfianza=0;
    QString tconfianza="0";
    bool detectada= false;
    QString matriculadetected="";
    cv::Mat image_matricula = cv::Mat::zeros( matriculaSize, CV_8UC3 );
    image_matricula = cv::Scalar( 0, 0, 255 );

    if (remolque->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        image_matricula = m_FotoCamara.clone();
        //CONVERSION de ROJA
        setFotoCalibrada(1);
        cv::Mat img = m_FotoCalibradaRojosCV.clone();
        // RECONOCER
        std::vector<AlprRegionOfInterest> regionsOfInterest;
        AlprResults results = remolque->recognize(img.data, img.elemSize(), img.cols, img.rows,regionsOfInterest);
        //Auto ajuste PlankC ROJAS
        if(results.plates.size()>0){
            for (uint i = 0; i < results.plates.size(); i++){
                AlprPlateResult plate = results.plates[i];
                for (uint k = 0; k < plate.topNPlates.size(); k++){
                    AlprPlate candidate = plate.topNPlates[k];
                     if (candidate.matches_template  && confianza < candidate.overall_confidence){
                         float confianza = candidate.overall_confidence;
                         confianza = candidate.overall_confidence;
                         //dconfianza = (double)candidate.overall_confidence;
                         tconfianza = QString::number(confianza,'g',6);
                         detectada = candidate.matches_template;
                         matriculadetected = QString::fromStdString(candidate.characters);
                         cv::Rect rect = cv::Rect(plate.plate_points[0].x,plate.plate_points[0].y,
                                                  plate.plate_points[2].x - plate.plate_points[0].x,
                                                  plate.plate_points[2].y - plate.plate_points[0].y);

                         //prewarp.setTransform(1280.000000,720.000000,-0.000350,-0.000350,-0.060000,0.750000,1.240000,1.0,1.0);
                         //curWarpedImage = prewarp.warpImage(image_matricula);
                         //image_matricula= cv::Mat(image_matricula,rect);
                         cv::resize(cv::Mat(image_matricula,rect),image_matricula,matriculaSize);

                         qDebug() << "  remolque  - " << QString::fromStdString(candidate.characters) << "\t precision: " << confianza << "% " <<
                                     candidate.matches_template << endl;
                     }
                }
            }
        }else{
            //Algoritmo PlankC ROJAS
            qDebug() << "plank=" << m_Plank.A << "," << m_Plank.B << "," << m_Plank.C;
        }

        regionsOfInterest.clear();
        img.release();
    }
    emit ReplyMatriculaFotoRemolque(matriculadetected,tconfianza,detectada,image_matricula.clone(),convertMat2Bytearray(image_matricula.clone()));
    emit workFinished();
    //limpiar
    image_matricula.release();
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
    //cv::Mat mat = cv::imdecode(pic,CV_LOAD_IMAGE_COLOR);
    buffer.close();
    baScene.clear();
    return cv::imdecode(pic,CV_LOAD_IMAGE_COLOR);
    //pic.clear();
    //return mat;
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




