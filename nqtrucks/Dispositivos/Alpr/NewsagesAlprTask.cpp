#include "NewsagesAlprTask.h"


#include "alpr.h"
#include "state_detector.h"

#include <QByteArray>
#include <QBuffer>

#include <QDebug>

namespace nQTrucks {
    namespace Devices {
    using namespace alpr;

/** CONSTRUCTOR TAREAS **/
NewsagesAlprTask::NewsagesAlprTask(int _nDevice, QImage _fotoCamara, QSettings *_appsettings, QObject *parent)
    : QObject(parent)
    , m_nDevice(_nDevice)
    , m_FotoCamara(_fotoCamara)
    , m_settings(_appsettings)
{
    qRegisterMetaType<t_Plank>("t_Plank");
    qRegisterMetaType<MatriculaResults>("t_MatriculaResults");
    setlocale(LC_NUMERIC, "C");
}

NewsagesAlprTask::~NewsagesAlprTask()
{
    this->deleteLater();
}
/** END CONSTRUCTOR *****************************************************************************/


/** SETTINGS **/
void NewsagesAlprTask::setFotoCamara() {
    m_FotoCamaraCV = convertQImage2Mat(m_FotoCamara.copy());
}

void NewsagesAlprTask::loadconfig()
{
    switch (m_nDevice) {
    case 1:
        m_configroot = (QString(CAMARA1));
        break;
    case 2:
        m_configroot = (QString(CAMARA2));
        break;
    default:
        //m_configroot = (QString(CAMARA1));
        break;
    }

    m_settings->beginGroup(m_configroot);
    setPlank(m_settings->value("plankA","0").toString(),
             m_settings->value("plankB","20").toString(),
             m_settings->value("plankC","40").toString()
            );
    m_settings->endGroup();
    m_settings->sync();
}

void NewsagesAlprTask::setPlank(QString A, QString B, QString C)
{
  m_Plank.A=A.toInt();
  m_Plank.B=B.toInt();
  m_Plank.C=C.toInt();
}
/** END SETTINGS **/


/** PROCESAR *******************************************************************************/
    /** BLANCAS **/
void NewsagesAlprTask::procesarBlancas()
{
    Alpr *matricula;
    matricula = new Alpr("truck",  "config/openalpr.conf");
    matricula->setDefaultRegion("truck");
    matricula->setTopN(1);

    //Respuesta por defecto
    float confianza=0;
    double dconfianza=0;
    QString tconfianza="0";
    bool detectada= false;
    QString matriculadetected="";
    QImage image_matricula = QImage(300,200,QImage::Format_RGB888);
    image_matricula.fill(Qt::red);

    if (matricula->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        image_matricula = m_FotoCamara.copy();
        //CONVERSION de BLANCOS
        setFotoCalibrada(0);
        cv::Mat img = m_FotoCalibradaBlancosCV.clone();
        // RECONOCER
        std::vector<AlprRegionOfInterest> regionsOfInterest;
        AlprResults results = matricula->recognize(img.data, img.elemSize(), img.cols, img.rows,regionsOfInterest);

        for (int i = 0; i < results.plates.size(); i++){
            AlprPlateResult plate = results.plates[i];
            for (int k = 0; k < plate.topNPlates.size(); k++){
                AlprPlate candidate = plate.topNPlates[k];
                if (candidate.matches_template  && confianza < candidate.overall_confidence){
                    confianza = candidate.overall_confidence;
                    dconfianza = (double)candidate.overall_confidence;
                    tconfianza = QString::number(confianza,'g',6);
                    detectada = candidate.matches_template;
                    matriculadetected = QString::fromStdString(candidate.characters);
                    image_matricula = image_matricula.copy(QRect
                                                           (QPoint(plate.plate_points[0].x,plate.plate_points[0].y),
                                                            QPoint(plate.plate_points[2].x,plate.plate_points[2].y)));

                    qDebug() << "  blanca  - " << QString::fromStdString(candidate.characters) << "\t precision: " << confianza << "% " <<
                                candidate.matches_template << endl;
                }
            }
        }
    }

    emit ReplyMatriculaFoto(matriculadetected,tconfianza,detectada,image_matricula.copy());
    emit workFinished();
}


    /** ROJAS **/
void NewsagesAlprTask::procesarRojas()
{
    Alpr *remolque;
    remolque  = new Alpr("eur", "config/openalpr.conf");
    remolque->setTopN(1);
    remolque->setDefaultRegion("eur");

    //Respuesta por defecto
    float confianza=0;
    double dconfianza=0;
    QString tconfianza="0";
    bool detectada= false;
    QString matriculadetected="";
    QImage image_matricula = QImage(300,200,QImage::Format_RGB888);
    image_matricula.fill(Qt::red);

    if (remolque->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        image_matricula = m_FotoCamara.copy();
        //CONVERSION de ROJA
        setFotoCalibrada(1);
        cv::Mat img = m_FotoCalibradaRojosCV.clone();
        // RECONOCER
        std::vector<AlprRegionOfInterest> regionsOfInterest;
        AlprResults results = remolque->recognize(img.data, img.elemSize(), img.cols, img.rows,regionsOfInterest);
        for (int i = 0; i < results.plates.size(); i++){
            AlprPlateResult plate = results.plates[i];
            for (int k = 0; k < plate.topNPlates.size(); k++){
                AlprPlate candidate = plate.topNPlates[k];
                 if (candidate.matches_template  && confianza < candidate.overall_confidence){
                     float confianza = candidate.overall_confidence;
                     confianza = candidate.overall_confidence;
                     dconfianza = (double)candidate.overall_confidence;
                     tconfianza = QString::number(confianza,'g',6);
                     detectada = candidate.matches_template;
                     matriculadetected = QString::fromStdString(candidate.characters);
                     image_matricula = image_matricula.copy(QRect(
                                                                QPoint(plate.plate_points[0].x,plate.plate_points[0].y),
                                                                QPoint(plate.plate_points[2].x,plate.plate_points[2].y)));


                     qDebug() << "  remolque  - " << QString::fromStdString(candidate.characters) << "\t precision: " << confianza << "% " <<
                                 candidate.matches_template << endl;

                 }
            }
        }
    }
    emit ReplyMatriculaFotoRemolque(matriculadetected,tconfianza,detectada,image_matricula.copy());
    emit workFinished();
}
/** END PROCESAR ************************************************************************/

/** CALIBRACION **********************************************************/
QImage NewsagesAlprTask::FotoCalibrada(int n) const
{
    switch (n) {
    case 0:
        return m_FotoCalibradaBlancos.copy();
        break;
    case 1:
        return m_FotoCalibradaRojos.copy();
        break;
    default:
        break;
    }
}

void NewsagesAlprTask::setFotoCalibrada(int n)
{
    // CALIBRACION —---»» PARAMETRIZAR :::::
    loadconfig();
    setFotoCamara();
    cv::Mat img = m_FotoCamaraCV.clone();
    cv::Mat channel[3];
    switch (n) {
    case 0:
        cv::add(img,cv::Scalar(m_Plank.C,m_Plank.B,m_Plank.A),img);
        cv::split(img, channel);
        img = channel[2] - channel[1] -   channel[2] + channel[0];
        m_FotoCalibradaBlancosCV = img.clone();
        m_FotoCalibradaBlancos = convertMat2QImage(img.clone());
        emit ReplyOriginalFotoBlanca(m_FotoCalibradaBlancos.copy());
        break;
    case 1:
        cv::add(img,cv::Scalar(m_Plank.A,m_Plank.B,m_Plank.C),img);
        cv::split(img, channel);
        cv::add(channel[0], channel[1], img);
        cv::subtract(channel[2], channel[1], img);
        m_FotoCalibradaRojosCV = img.clone();
        m_FotoCalibradaRojos = convertMat2QImage(img.clone());
        emit ReplyOriginalFotoRoja(m_FotoCalibradaRojos.copy());
        break;
    default:
        break;
    }
}

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


/** CONVERSORES *************************************************************************/
cv::Mat NewsagesAlprTask::convertQImage2Mat(const QImage& img){
    QByteArray baScene; // byte array with data
    QBuffer buffer(&baScene);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer,"PNG");

    const char* begin = reinterpret_cast<char*>(baScene.data());
    const char* end = begin + baScene.size();
    std::vector<char> pic(begin, end);
    cv::Mat mat = cv::imdecode(pic,CV_LOAD_IMAGE_COLOR);
    buffer.close();
    return mat.clone();
}

QImage NewsagesAlprTask::convertMat2QImage(const cv::Mat& src)
{
    QImage qtImg;
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
                            QImage::Format_RGB888 );
        }
    }
    return qtImg.copy();
}
/** END CONVERSORES ***********************************************************/
} /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/




