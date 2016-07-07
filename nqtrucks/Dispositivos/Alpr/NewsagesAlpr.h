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
private:
    int                m_nDevice;
    QSettings         *m_settings;
    //MatriculaResults  *m_results0;
    MatriculaResults  *m_results1;
    MatriculaResults  *m_results2;
    /** END SETTINGS **/

    /** CALIBRAR *****************************************************/
private:
    bool bhiloCalibrar1 = false;
    bool bhiloCalibrar2 = false;
    QThread *hiloCalibrar1;
    QThread *hiloCalibrar2;
    NewsagesAlprTask *tareaCalibrar1;
    NewsagesAlprTask *tareaCalibrar2;

signals:
    void ReplyOriginalFoto(const MatriculaResults &_camara);

public slots:
    void calibrarFoto(const MatriculaResults &_camara);
signals:
    void ReplyMatriculaCalibrationResults(const MatriculaResults  &_calibration_results);
private slots:
    void onCalibrarFotoFinished();
    /** END CALIBRAR *******************************************************/


    /** Procesar Matriculas **/
public slots:
    //void setFotoCamara(const Camara &_camara);
    void processFoto(const MatriculaResults &_camara);
signals:
    void ReplyMatriculaResults(const MatriculaResults &_MatriculaResults);
protected slots:
    void onProcesarFotoFinished();
private:
    bool bhilo1 = false;
    bool bhilo2 = false;
    QThread *hilo1;
    QThread *hilo2;
    NewsagesAlprTask *tarea1;
    NewsagesAlprTask *tarea2;


};

} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/
#endif // NEWSAGESALPR_H
