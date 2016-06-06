#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QImage>
#include <nqtglobal.h>

#include <QSqlDatabase>

namespace nQTrucks {
namespace Db {

class DatabaseManager : public QObject{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent=nullptr);

signals:
    void workFinished();

    /** DB **/
private:
    QSqlDatabase db;
    Registros::Simple_Matriculas m_registro_simple_matriculas;
//        QByteArray resizeFoto(QByteArray _ByteArray);
//        cv::Mat byteArray2Mat(QByteArray &byteArray);
//        QImage convertMat2QImage(const cv::Mat &src);
//        QByteArray convertMat2ByteArray(const cv::Mat &img);
public slots:
    void guardarRegistroSimpleMatriculas();
    void setRegistroSimpleMatriculas(const Registros::Simple_Matriculas &_registro);

    /** END DB **/
};

}
}
#endif // DATABASEMANAGER_H
