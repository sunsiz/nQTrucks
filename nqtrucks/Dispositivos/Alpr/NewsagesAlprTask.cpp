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

    m_configroot = "Alpr";
    m_settings->beginGroup(m_configroot);

    setPlank(m_settings->value("planka1").toString(),
             m_settings->value("plankb1").toString(),
             m_settings->value("plankc1").toString(),
             m_settings->value("planka2").toString(),
             m_settings->value("plankb2").toString(),
             m_settings->value("plankc2").toString() );

    setPrewarp( m_settings->value("prewarp2").toString(),
                m_settings->value("prewarp1").toString() );

   m_settings->endGroup();


}

t_Plank NewsagesAlprTask::Plank() const
{
    return m_Plank;
}

void NewsagesAlprTask::setPlank(const QString &A1, const QString &B1, const QString &C1, const QString &A2, const QString &B2, const QString &C2){
  m_Plank.A1=A1.toInt();
  m_Plank.B1=B1.toInt();
  m_Plank.C1=C1.toInt();
  m_Plank.A2=A2.toInt();
  m_Plank.B2=B2.toInt();
  m_Plank.C2=C2.toInt();

  qDebug() << "plank1=" << m_Plank.A1 << "," << m_Plank.B1 << "," << m_Plank.C1;
  qDebug() << "plank2=" << m_Plank.A2 << "," << m_Plank.B2 << "," << m_Plank.C2;
}

QString NewsagesAlprTask::Prewarp1() const
{
    return m_prewarp1;
}

QString NewsagesAlprTask::Prewarp2() const
{
    return m_prewarp2;
}

void NewsagesAlprTask::setPrewarp(const QString &prewarp1, const QString &prewarp2){
    m_prewarp1=prewarp1;
    m_prewarp2=prewarp2;
  qDebug() << "actual prewarp1: " << m_prewarp1;
  qDebug() << "actual prewarp2: " << m_prewarp2;
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
        loadconfig();
        cv::add(img.clone(),cv::Scalar(Plank().C1,Plank().B1,Plank().A1),img);
        cv::split(img, channel);
        img = channel[2] - channel[1] -   channel[2] + channel[0];
        m_FotoCalibradaBlancosCV = img.clone();
        emit ReplyOriginalFotoBlanca(apply_prewarp1(m_FotoCalibradaBlancosCV.clone()));
        //Limpiar
        channel[0].release();
        channel[1].release();
        channel[2].release();
        break;
    case 1:
        loadconfig();
        cv::add(img.clone(),cv::Scalar(Plank().A2,Plank().B2,Plank().C2),img);
        cv::split(img, channel);
        cv::add(channel[0], channel[1], img);
        cv::subtract(channel[2], channel[1], img);
        m_FotoCalibradaRojosCV = img.clone();
        emit ReplyOriginalFotoRoja(apply_prewarp2(m_FotoCalibradaRojosCV.clone()));
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

cv::Mat NewsagesAlprTask::apply_prewarp1(const cv::Mat &img){
    loadconfig();
    alpr::Config config("truck");
    config.prewarp = Prewarp1().toStdString();
    config.setDebug(false);
    alpr::PreWarp prewarp(&config);
    cv::Mat imgprewarp=prewarp.warpImage(img.clone());
    prewarp.clear();
    return  imgprewarp;
}

cv::Mat NewsagesAlprTask::apply_prewarp2(const cv::Mat &img){
    loadconfig();
    alpr::Config config("eur");
    config.prewarp = Prewarp2().toStdString();
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
    QString filename = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("config/openalpr.conf");
    Alpr *matricula;
    matricula = new Alpr("truck", filename.toStdString());
    matricula->setDefaultRegion("truck");
    matricula->setTopN(1);
    matricula->setPrewarp(Prewarp1().toStdString());
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
                    m_matricularesult->MatriculaPrecisionA = candidate.overall_confidence;
                    m_matricularesult->MatriculaPrecisionAs = QString::number(m_matricularesult->MatriculaPrecisionA,'g',6);
                    m_matricularesult->MatriculaDetectedA = candidate.matches_template;
                    m_matricularesult->MatriculaA  = QString::fromStdString(candidate.characters);
                    cv::Rect rect = cv::Rect(plate.plate_points[0].x , plate.plate_points[0].y,
                                             plate.plate_points[2].x - plate.plate_points[0].x,
                                             plate.plate_points[2].y - plate.plate_points[0].y);
                    //cv::Mat img_resize = m_matricularesult->OrigenFoto.clone();

                    cv::resize(cv::Mat(m_matricularesult->OrigenFoto,rect)
                               ,m_matricularesult->MatriculaFotoA,
                               matriculaSize);

                    qDebug() << "  blanca  - " << QString::fromStdString(candidate.characters)
                             << "\t precision: "
                             << m_matricularesult->MatriculaPrecisionA << "% "
                             << candidate.matches_template << endl;
                    //img_resize.release();
                }else{
                //Algoritmo PlankC blancas
                qDebug() << "plank1=" << m_Plank.A1 << "," << m_Plank.B1 << "," << m_Plank.C1;
                }
            }
        }
       regionsOfInterest.clear();
    }
    emit ReplyMatriculaFoto(*m_matricularesult);
    emit workFinished();

    //limpiar
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
    remolque->setPrewarp(Prewarp2().toStdString());
    //Respuesta por defecto
    m_matricularesult->OrigenFoto = m_FotoCamara.clone();
    m_matricularesult->MatriculaFotoB = cv::Mat::zeros( matriculaSize, CV_8UC3 );
    m_matricularesult->MatriculaFotoB = cv::Scalar( 0, 0, 255 );
    m_matricularesult->MatriculaB="";
    m_matricularesult->MatriculaPrecisionB=0;
    m_matricularesult->MatriculaPrecisionBs="0%";
    if (remolque->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        //CONVERSION de ROJA
        setFotoCalibrada(1);
        m_matricularesult->OrigenFotoRoja = m_FotoCalibradaRojosCV.clone();
        // RECONOCER
        std::vector<AlprRegionOfInterest> regionsOfInterest;
        AlprResults results = remolque->recognize(m_matricularesult->OrigenFotoRoja.data,
                                                  m_matricularesult->OrigenFotoRoja.elemSize(),
                                                  m_matricularesult->OrigenFotoRoja.cols,
                                                  m_matricularesult->OrigenFotoRoja.rows,
                                                  regionsOfInterest);
        //Auto ajuste PlankC ROJAS
        if(results.plates.size()>0){
            for (uint i = 0; i < results.plates.size(); i++){
                AlprPlateResult plate = results.plates[i];
                for (uint k = 0; k < plate.topNPlates.size(); k++){
                    AlprPlate candidate = plate.topNPlates[k];
                     if (candidate.matches_template  && m_matricularesult->MatriculaPrecisionB < candidate.overall_confidence){
                         m_matricularesult->MatriculaPrecisionB  = candidate.overall_confidence;
                         m_matricularesult->MatriculaPrecisionBs = QString::number(m_matricularesult->MatriculaPrecisionB,'g',6);
                         m_matricularesult->MatriculaDetectedB   = candidate.matches_template;
                         m_matricularesult->MatriculaB.fromStdString(candidate.characters);

                         cv::Rect rect = cv::Rect(plate.plate_points[0].x  ,plate.plate_points[0].y,
                                                  plate.plate_points[2].x - plate.plate_points[0].x,
                                                  plate.plate_points[2].y - plate.plate_points[0].y);

                         cv::resize(cv::Mat(m_matricularesult->OrigenFoto,rect),
                                    m_matricularesult->MatriculaFotoB,matriculaSize);

                         qDebug() << "  remolque  - " << QString::fromStdString(candidate.characters)
                                  << "\t precision: " << m_matricularesult->MatriculaPrecisionB << "% " <<
                                     candidate.matches_template << endl;
                     }
                }
            }
        }else{
            //Algoritmo PlankC ROJAS
            qDebug() << "plank=" << m_Plank.A2 << "," << m_Plank.B2 << "," << m_Plank.C2;
        }

        regionsOfInterest.clear();
    }
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




