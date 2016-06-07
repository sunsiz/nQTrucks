#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H
#include <QObject>
#include <nqtglobal.h>
#include <LimeReport>

namespace nQTrucks {
namespace Db {

class ReportManager : public QObject{
   Q_OBJECT
public:
    explicit ReportManager(QObject *parent=nullptr);

private:
    LimeReport::ReportEngine m_report;
private:

};

}
}
#endif // REPORTMANAGER_H
