#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QImage>
#include <nqtglobal.h>

#include <QSqlDatabase>

#include "ReportManager.h"

namespace nQTrucks {
namespace Db {

class DatabaseManager : public QObject{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent=nullptr);
    ~DatabaseManager();

signals:
    void workFinished();

    /** DB **/
private:
    void initDb();
    QSqlDatabase m_db;
    Registros::SimpleMatriculas m_registro_simple_matriculas;
    ReportManager m_report_manager;
//        QByteArray resizeFoto(QByteArray _ByteArray);
//        cv::Mat byteArray2Mat(QByteArray &byteArray);
//        QImage convertMat2QImage(const cv::Mat &src);
//        QByteArray convertMat2ByteArray(const cv::Mat &img);
public slots:
    void guardarRegistroSimpleMatriculas();
    void setRegistroSimpleMatriculas(const Registros::SimpleMatriculas &_registro);

    /** END DB **/
};

}
}
#endif // DATABASEMANAGER_H
