/***************************************************************************
 *   This file is part of the nQTrucks project                             *
 *   Copyright (C) 2015 by Efraím Pérez                                    *
 *   newsages2014@gmail.com                                                *
 *                                                                         *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/
#ifndef NQTRUCKSENGINE_H
#define NQTRUCKSENGINE_H


#include <QObject>

#include <nqtglobal.h>

/** REPORTS **/
#include <QApplication>
#include <QDir>
#include <QTreeWidgetItem>

/** MAESTROS **/
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QDataWidgetMapper>

///** REGISTROS **/
//#include "Registros/Bascula.h"
//#include "Registros/Camara.h"
//#include "Registros/MatriculaResults.h"
//#include "Registros/RegistroMatriculas.h"


namespace nQTrucks {
class nQTrucksEnginePrivate;

class NQTRUCKSLIBSHARED_EXPORT nQTrucksEngine : public QObject
{
    Q_OBJECT
public:
    explicit nQTrucksEngine(QObject *parent=0);
    ~nQTrucksEngine();

    /** CONFIG **/
public:
    static void setSettings(QSettings *value){m_settings=value;}
    void setAppConfig(QSettings *value);
    QSettings *appConfig();
    /** END CONFIG **/

    /** Camaras **/
public:
    QStringList getTiposCamaras();
    void getCamaraFoto(int _ncamara);
signals:
    void CamaraIP1(const MatriculaResults &_camara);
    void CamaraIPWeb1(const QString &url);
    void CamaraIP2(const MatriculaResults &_camara);
    void CamaraIPWeb2(const QString &url);
    /** END Camaras **/

    /** NEWSAGES I/O **/
public:
    //QStringList getIODevices();
public slots:
    void setSemaforoDevicesConnect(const bool &_value);
    void setSemaforoStatus(const int &_color);
signals:
    void SemaforoEstadoChanged(const int &color);
    void SemaforoConnectChanged(const bool &_value);
    /** END NEWSAGES I/O **/

    /** BASCULAS **/
public:
    QStringList getTiposBasculas();
    QStringList getSerialDevices();
public slots:
    void setBasculaConnect(bool _value);
signals:
    void BasculaStatus(const bool &status);
    void BasculaChanged(const Bascula &bascula);
    /** END BASCULAS **/

    /** NEWSAGES ALPR **/
public:
    void calibrarFoto(const int &_device, const MatriculaResults &_camara);
    void getFotoMatricula(const int &_device, const MatriculaResults &_camara);

    /** ALPR 1 **/
signals:
    void ReplyOriginalFoto1(const MatriculaResults &_camara);
    void ReplyMatriculaCalibrationResults1(const MatriculaResults  &_calibration_results);
    void ReplyMatriculaResults1(const MatriculaResults  &_results);

    /** ALPR 2 **/
signals:
    void ReplyOriginalFoto2(const MatriculaResults &_camara);
    void ReplyMatriculaCalibrationResults2(const MatriculaResults  &_calibration_results);
    void ReplyMatriculaResults2(const MatriculaResults &_results);

    /** END NEWSAGES ALPR **/        

    /** CORE ***************************************/
    /** DAEMON **/
signals:
    void daemonRegistroChanged(nQTrucks::RegistroMatriculas *_results);
public slots:
    void setInitDaemon(const bool &_init);

    /** END CORE ************************************/

    /** REPORTS **/
private:

public:
    template< typename T >
    void report_buildReportsTree(T* parentItem, const QString &path=QApplication::applicationDirPath()+"/reports/")
    {
        QDir reportsDir(path);
        QStringList items = reportsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach( QString dir, items){
            QTreeWidgetItem* listItem = new QTreeWidgetItem(parentItem);
            listItem->setText(0,dir);
            listItem->setIcon(0,QIcon(":/temp/icons/system/devices/folder-new.png"));
            report_buildReportsTree(listItem,reportsDir.path() + "/" +dir );
        }
        QStringList nameFilters;
        nameFilters <<"*.lrxml";
        items = reportsDir.entryList(nameFilters, QDir::Files);
        foreach( QString file, items){
            QStringList fileonly = file.toUpper().split(".");
            QTreeWidgetItem* listItem = new QTreeWidgetItem(parentItem);
            listItem->setIcon(0,QIcon(":/temp/icons/system/devices/libreoffice3-impress2.png"));
            listItem->setText(0,fileonly[0]);
            listItem->setData(0,Qt::UserRole,reportsDir.path()+"/"+file);
        }
    }
    void report_desingReport();
public slots:
    void report_loadFromFile(const QString &_file);
    /** END REPORTS **/


    /** MAESTROS **/
public:
    QSqlQueryModel    *RegistrosPesos;
    QSqlTableModel    *RegistrosPesosTable;
    QSqlTableModel    *Empresa;
    QDataWidgetMapper *Empresa_Mapper;
public slots:
    void sincronizar_maestros();
    //void updateEmpresa(const QVector<QString> _empresaVector);
    QSqlDatabase getCurrentDatabase();
private slots:
    void reloadMaestros();
signals:
    void rangoFechasChanged(const QVector<QDate> &_fechaMinMax);
    void registrandoChanged(const bool &_registrando);
    /** END MAESTROS **/

    /** Report Manager **/
public slots:
    void printReport(const int &_ReportId, const QVector<long long> &_vectorRows);
    /** END Report Manager **/


    /** nQTrucksEnginePrivate **/
protected:
    nQTrucksEnginePrivate * const d_ptr;
    nQTrucksEngine(nQTrucksEnginePrivate &dd, QObject * parent=0);
private:
    Q_DECLARE_PRIVATE(nQTrucksEngine)
    static QSettings* m_settings;
    /** FIN nQTrucksEnginePrivate **/
};

}
#endif // NQTRUCKSENGINE_H
