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

#ifndef NEWSAGESALPR_H
#define NEWSAGESALPR_H

#include <QObject>
#include <QImage>
#include <QThread>
#include <nqtglobal.h>
#include "NewsagesAlprTask.h"

namespace nQTrucks {
namespace Devices {

class NewsagesAlpr : public QObject
{
    Q_OBJECT
public:
    explicit NewsagesAlpr(int nDevice=0, QSettings *_appsettings=0, QObject *parent = 0);
    ~NewsagesAlpr();

    /** SETTINGS **/
public slots:
    void setFotoCamara(const Registros::Camara &_camara);
private:
    int         m_nDevice;
    QSettings   *m_settings;
    QVector<Registros::MatriculaResults*> m_results;
    /** END SETTINGS **/

signals:
    void ReplyOriginalFoto(         const Registros::Camara &_camara);
    void ReplyMatriculaFoto        (const Registros::MatriculaResults &_tempResults);
    void ReplyMatriculaFotoRemolque(const Registros::MatriculaResults &_tempResultsRemolque);

    /** CALIBRAR *****************************************************/
private:
    QThread *hiloCalibrar1;
    NewsagesAlprTask *tareaCalibrar1;
    bool bhiloCalibrar1;

    QThread *hiloCalibrar2;
    NewsagesAlprTask *tareaCalibrar2;
    bool bhiloCalibrar2;
public slots:
    void calibrarFoto(const Registros::Camara &_camara);
signals:
    //void ReplyOriginalFotoRoja(const cv::Mat &Foto);
    //void ReplyOriginalFotoBlanca(const cv::Mat &Foto);
    void ReplyMatriculaCalibrationResults(const Registros::MatriculaResults  &_calibration_results);
private slots:
    void onCalibrarFotoFinished();
    /** END CALIBRAR *******************************************************/


    /** Procesar Matriculas **/
public slots:
    void processFoto(const Registros::Camara &_camara);
signals:
    void ReplyMatriculaResults(const Registros::MatriculaResults &_MatriculaResults);
protected slots:
    void onProcesarFotoFinished();
private:
    QThread *hilo1;
    NewsagesAlprTask *tarea1;
    bool bhilo1;

    QThread *hilo2;
    NewsagesAlprTask *tarea2;
    bool bhilo2;

};

} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/
#endif // NEWSAGESALPR_H
