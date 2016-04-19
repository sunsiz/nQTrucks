#include "NewsagesAlprTask.h"

#include <QByteArray>
#include <QBuffer>
#include <QDebug>

namespace nQTrucks {
    namespace Devices {
    using namespace alpr;

    /** TAREAS **/

NewsagesAlprTask::NewsagesAlprTask(int nDevice, QSettings *_appsettings, QObject *parent)
    : QObject(parent)
    , m_nDevice(nDevice)
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


/** SETTINGS **/
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

void NewsagesAlprTask::calibrarBlancas()
{
    loadconfig();
    cv::Mat img = QImage2cvMat(*m_fotocamara);
    cv::add(img,cv::Scalar(m_Plank.C,m_Plank.B,m_Plank.A),img);
    cv::Mat channel[3];
    cv::split(img, channel);
    img = channel[2] - channel[1] -   channel[2] + channel[0];
    emit ReplyOriginalFotoBlanca(cvMat2QImage(img));
}

void NewsagesAlprTask::calibrarRojas()
{
    cv::Mat img = QImage2cvMat(*m_fotocamara);
    cv::add(img,cv::Scalar(m_Plank.A,m_Plank.B,m_Plank.C),img);
    cv::Mat channel[3];
    cv::split(img, channel);
    cv::add(channel[0], channel[1], img);
    cv::subtract(channel[2], channel[1], img);
    emit ReplyOriginalFotoRoja(cvMat2QImage(img));
}

cv::Mat NewsagesAlprTask::QImage2cvMat(QImage image)
{
    /** CONVERSOR QImage to std::vector<char>) **/
    QByteArray baScene; // byte array with data
    QBuffer buffer(&baScene);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer,"PNG");

    const char* begin = reinterpret_cast<char*>(baScene.data());
    const char* end = begin + baScene.size();
    std::vector<char> pic(begin, end);
    cv::Mat img = cv::imdecode(pic,CV_LOAD_IMAGE_COLOR);
    return img;

}

QImage NewsagesAlprTask::cvMat2QImage(const cv::Mat &image)
{
    QImage qtImg;
    if( !image.empty() && image.depth() == CV_8U ){
        if(image.channels() == 1){
            qtImg = QImage( (const unsigned char *)(image.data),
                            image.cols,
                            image.rows,
                            QImage::Format_Indexed8 );
        }
        else{
            cvtColor( image, image, CV_BGR2RGB );
            qtImg = QImage( (const unsigned char *)(image.data),
                            image.cols,
                            image.rows,
                            QImage::Format_RGB888 );
        }
    }
    //emit ReplyMatriculaFoto(qtImg);
    return qtImg;
    /** CONVERSOR QImage to std::vector<char>) **/
}



void NewsagesAlprTask::procesarBlancas()
{
    //Alpr matricula("eu", "config/openalpr.conf");
    matricula = new Alpr("truck",  "config/openalpr.conf");
    matricula->setDefaultRegion("truck");
    matricula->setTopN(1);

    //matricula->setPrewarp("");

    if (matricula->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        //Implementado:
        cv::Mat img = QImage2cvMat(*m_fotocamara);
        QImage image_matricula = QImage(*m_fotocamara);

        //CONVERSION de BLANCOS
         // CALIBRACION —---»» PARAMETRIZAR :::::
            cv::add(img,cv::Scalar(40,20,0),img);
        cv::Mat channel[3];
        cv::split(img, channel);
        img = channel[2] - channel[1] -   channel[2] + channel[0];
        //emit ReplyMatriculaFoto(cvMat2QImage(img));
        // RECONOCER
        std::vector<AlprRegionOfInterest> regionsOfInterest;
        AlprResults results = matricula->recognize(img.data, img.elemSize(), img.cols, img.rows,regionsOfInterest);
        for (int i = 0; i < results.plates.size(); i++)
        {
            AlprPlateResult plate = results.plates[i];
            for (int k = 0; k < plate.topNPlates.size(); k++)
            {
                AlprPlate candidate = plate.topNPlates[k];
                float confianza = candidate.overall_confidence;
                qDebug() << "  blanca  - " << QString::fromStdString(candidate.characters) << "\t precision: " << confianza << "% " <<
                            candidate.matches_template << endl;
                image_matricula = image_matricula.copy(QRect(QPoint(plate.plate_points[0].x,plate.plate_points[0].y),
                                                     QPoint(plate.plate_points[2].x,plate.plate_points[2].y)));

                emit ReplyMatriculaFoto(image_matricula);
            }
        }
    }
}

void NewsagesAlprTask::procesarRojas()
{
    //Alpr remolque("eur", "config/openalpr.conf");
    remolque  = new Alpr("eur", "config/openalpr.conf");
    remolque->setTopN(1);
    remolque->setDefaultRegion("eur");
    //openalpr.setPrewarp("");

    if (remolque->isLoaded() == false){
        qDebug() << "Error loading OpenALPR" << endl;
    }else{
        //Implementado:
        cv::Mat img = QImage2cvMat(*m_fotocamara);
        QImage image_matricula = QImage(*m_fotocamara);

        //CONVERSION de ROJA
        // CALIBRACION —---»» PARAMETRIZAR :::::
            cv::add(img,cv::Scalar(0,20,40),img);
        cv::Mat channel[3];
        cv::split(img, channel);
        cv::add(channel[0], channel[1], img);
        cv::subtract(channel[2], channel[1], img);

        //cv::imshow("ROJOS",img);

        // RECONOCER
        std::vector<AlprRegionOfInterest> regionsOfInterest;
        AlprResults results = remolque->recognize(img.data, img.elemSize(), img.cols, img.rows,regionsOfInterest);
        for (int i = 0; i < results.plates.size(); i++)
        {
            AlprPlateResult plate = results.plates[i];
            for (int k = 0; k < plate.topNPlates.size(); k++)
            {
                AlprPlate candidate = plate.topNPlates[k];
                float confianza = candidate.overall_confidence;
                qDebug() << "  remolque  - " << QString::fromStdString(candidate.characters) << "\t precision: " << confianza << "% " <<
                            candidate.matches_template << endl;
                image_matricula = image_matricula.copy(QRect(QPoint(plate.plate_points[0].x,plate.plate_points[0].y),
                                                     QPoint(plate.plate_points[2].x,plate.plate_points[2].y)));
                emit ReplyMatriculaFotoRemolque(image_matricula);
            }
        }
    }
}


} /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/




