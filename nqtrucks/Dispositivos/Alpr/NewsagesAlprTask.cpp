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
NewsagesAlprTask::NewsagesAlprTask(int _nDevice, int _nType, cv::Mat _fotoCamara, QSettings *_appsettings, QObject *parent)
    : QObject(parent)
    , m_settings(_appsettings)
    , m_matricularesult(new t_MatriculaResults)
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<t_Plank>("t_Plank");
    qRegisterMetaType<MatriculaResults>("t_MatriculaResults");
    setlocale(LC_NUMERIC, "C");

    setFotoCamara(_fotoCamara);
    setNDevice(_nDevice);
    setNType(_nType);

    //Respuesta por defecto
    m_matricularesult->OrigenFoto=getFotoCamara();
    cv::resize(m_matricularesult->OrigenFoto,m_matricularesult->OrigenFotoResize,fotoSize);

    if (m_matricularesult->OrigenFoto.cols < fotoWidth){
        cv::resize(m_matricularesult->OrigenFoto,m_matricularesult->OrigenFoto,fotoSize);
    }
    if (m_matricularesult->OrigenFoto.rows < fotoHeight){
        cv::resize(m_matricularesult->OrigenFoto,m_matricularesult->OrigenFoto,fotoSize);
    }
    m_matricularesult->OrigenFotoResizeByte = convertMat2ByteArray(m_matricularesult->OrigenFotoResize);
    m_matricularesult->MatriculaFotoAByte   = convertMat2ByteArray(m_matricularesult->MatriculaFotoA);
    m_matricularesult->MatriculaFotoBByte   = convertMat2ByteArray(m_matricularesult->MatriculaFotoB);

    loadconfig();
}

NewsagesAlprTask::~NewsagesAlprTask()
{
//    m_matricularesult.MatriculaA.detach();
//    m_matricularesult.MatriculaDetectedA=false;
//    m_matricularesult.MatriculaFotoA.release();
//    m_matricularesult.MatriculaFotoAByte.detach();
//    m_matricularesult.MatriculaPrecisionA=0;
//    m_matricularesult.MatriculaPrecisionAs.detach();
//    m_matricularesult.OrigenFoto.release();
//    m_matricularesult.OrigenFotoBlanca.release();
//    m_matricularesult.OrigenFotoPrewarp.release();
//    m_matricularesult.OrigenFotoResize.release();
//    m_matricularesult.OrigenFotoResizeByte.detach();

//    m_matricularesult.MatriculaB.detach();
//    m_matricularesult.MatriculaDetectedB=false;
//    m_matricularesult.MatriculaFotoB.release();
//    m_matricularesult.MatriculaFotoBByte.detach();
//    m_matricularesult.MatriculaPrecisionB=0;
//    m_matricularesult.MatriculaPrecisionBs.detach();
//    m_matricularesult.OrigenFoto.release();
//    m_matricularesult.OrigenFotoRoja.release();
//    m_matricularesult.OrigenFotoPrewarp.release();
//    m_matricularesult.OrigenFotoResize.release();
//    m_matricularesult.OrigenFotoResizeByte.detach();
    m_FotoCamara.release();
    //this->deleteLater();

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



void NewsagesAlprTask::setPlank(const QString &A, const QString &B, const QString &C){
  m_plank.A=A.toInt();
  m_plank.B=B.toInt();
  m_plank.C=C.toInt();
}

/** END SETTINGS **/


/** CALIBRACION **********************************************************/
void NewsagesAlprTask::calibrar()
{
    setFotoCalibrada();
    //qDebug() << "plank" << m_nDevice <<"=" << m_plank.A << "," << m_plank.B << "," << m_plank.C;
    //emit workFinished();
}

void NewsagesAlprTask::setFotoCalibrada()
{
    cv::Mat channel[3];
    switch (getNType()) {
    case ALPR_PLANCK_BLANCO:
        m_matricularesult->OrigenFotoBlanca = m_matricularesult->OrigenFoto.clone();
        cv::add(m_matricularesult->OrigenFotoBlanca,cv::Scalar(getPlank().C,getPlank().B,getPlank().A),m_matricularesult->OrigenFotoBlanca);
        cv::split(m_matricularesult->OrigenFotoBlanca, channel);
        m_matricularesult->OrigenFotoBlanca = channel[2] - channel[1] -   channel[2] + channel[0];
        emit ReplyOriginalFotoBlanca(m_matricularesult->OrigenFotoBlanca);
        //Limpiar
        channel[0].release();
        channel[1].release();
        channel[2].release();
        break;
    case ALPR_PLANCK_ROJO:
        m_matricularesult->OrigenFotoRoja = m_matricularesult->OrigenFoto.clone();
        cv::add(m_matricularesult->OrigenFotoRoja,cv::Scalar(getPlank().A,getPlank().B,getPlank().C),m_matricularesult->OrigenFotoRoja);
        cv::split(m_matricularesult->OrigenFotoRoja, channel);
        cv::add(channel[0], channel[1], m_matricularesult->OrigenFotoRoja);
        cv::subtract(channel[2], channel[1], m_matricularesult->OrigenFotoRoja);
        emit ReplyOriginalFotoRoja(m_matricularesult->OrigenFotoRoja);
        //Limpiar
        channel[0].release();
        channel[1].release();
        channel[2].release();
        break;
    default:
        m_matricularesult->OrigenFotoBlanca = m_matricularesult->OrigenFoto.clone();
        m_matricularesult->OrigenFotoRoja = m_matricularesult->OrigenFoto.clone();
        break;
    }
}

void NewsagesAlprTask::guardarPlanK()
{
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

//    switch (getNType()) {
//    case ALPR_PLANCK_BLANCO:
//        qDebug() << "  blanca  - " << m_matricularesult.MatriculaDetectedA
//                 << "\t precision: " << m_matricularesult.MatriculaPrecisionA << "% "
//                 << m_matricularesult.MatriculaDetectedA << endl;
//        break;
//    case ALPR_PLANCK_ROJO:
//        qDebug() << "  remolque  - " << m_matricularesult.MatriculaDetectedB
//                 << "\t precision: " << m_matricularesult.MatriculaPrecisionB << "% "
//                 << m_matricularesult.MatriculaDetectedB << endl;
//        break;
//    }

    //emit ReplyMatriculaFoto(*m_matricularesult);
    //emit workFinished();
    //this->deleteLater();
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

                            cv::resize(cv::Mat(m_matricularesult->OrigenFoto,rect),m_matricularesult->MatriculaFotoA,matriculaSize);
                            m_matricularesult->MatriculaFotoAByte   = convertMat2ByteArray(m_matricularesult->MatriculaFotoA);
                            m_matricularesult->MatriculaDetectedA   = candidate.matches_template;
                            m_matricularesult->MatriculaA           = QString::fromStdString(candidate.characters);
                            m_matricularesult->MatriculaPrecisionA  = candidate.overall_confidence;
                            m_matricularesult->MatriculaPrecisionAs = QString::number(m_matricularesult->MatriculaPrecisionA,'g',6);

                            if(m_matricularesult->MatriculaDetectedA){
                                guardarPlanK();
                                emit ReplyOriginalFotoBlanca(m_matricularesult->OrigenFotoBlanca);
                            }
                        }
                    }
                }
               regionsOfInterest.clear();
               m_plank.B=b;
               b++;
           }while( b<=m_retry_panks && m_matricularesult->MatriculaDetectedA!=true);
           m_plank.C=c;
           c++;
       }while( c<=m_retry_panks &&  m_matricularesult->MatriculaDetectedA!=true);
    }
    emit ReplyMatriculaFoto(*m_matricularesult);
    workFinished();
    delete matricula;
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

                                cv::resize(cv::Mat(m_matricularesult->OrigenFoto,rect),m_matricularesult->MatriculaFotoB,matriculaSize);
                                m_matricularesult->MatriculaFotoBByte   = convertMat2ByteArray(m_matricularesult->MatriculaFotoB.clone());
                                m_matricularesult->MatriculaDetectedB   = candidate.matches_template;
                                m_matricularesult->MatriculaB           = QString::fromStdString(candidate.characters);
                                m_matricularesult->MatriculaPrecisionB  = candidate.overall_confidence;
                                m_matricularesult->MatriculaPrecisionBs = QString::number(m_matricularesult->MatriculaPrecisionB,'g',6);

                                if(m_matricularesult->MatriculaDetectedB){
                                    guardarPlanK();
                                    emit ReplyOriginalFotoRoja(m_matricularesult->OrigenFotoRoja);
                                }
                            }
                       }
                    }
                }
                regionsOfInterest.clear();
                m_plank.B=b;
                b++;
            }while( b<=m_retry_panks && m_matricularesult->MatriculaDetectedB!=true);
            m_plank.C=c;
            c++;
        }while( c<=m_retry_panks &&  m_matricularesult->MatriculaDetectedB!=true);
    }
    //*END ALGORITMO PLANKS
    emit ReplyMatriculaFoto(*m_matricularesult);
    workFinished();
    //limpiar
    delete remolque;
}
/** END PROCESAR ************************************************************************/



///** CONVERSORES *************************************************************************/
cv::Mat NewsagesAlprTask::convertQImage2Mat(const QImage &img){
    QByteArray baScene; // byte array with data
    QBuffer buffer(&baScene);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer,"PNG");

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

QByteArray NewsagesAlprTask::convertMat2ByteArray(const cv::Mat &img){

    QImage qtImg = convertMat2QImage(img.clone());
    QByteArray baScene; // byte array with data
    QBuffer buffer(&baScene);
    buffer.open(QIODevice::WriteOnly);
    qtImg.save(&buffer,"PNG");
    buffer.close();
    return baScene;

}

///** END CONVERSORES ***********************************************************/
} /** END NAMESPACE Devices  **/
} /** END NAMESPACE nQTrucks **/




