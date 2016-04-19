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
    using namespace alpr;

class NewsagesAlpr : public QObject
{
    Q_OBJECT
public:
    explicit NewsagesAlpr(int nDevice=0, QSettings *_appsettings=0, QObject *parent = 0);

    /** SETTINGS **/
public slots:
    void setFoto(QImage *_fotocamara){m_fotocamara=_fotocamara;}
private:
    //QString   m_configroot;
    int       m_nDevice;
    QSettings *m_settings;
    t_MatriculaResults *m_matricularesults;
    /** END SETTINGS **/


signals:
    void ReplyOriginalFoto(const QImage &Foto);

    void ReplyOriginalFotoRoja(const QImage &Foto);
    void ReplyOriginalFotoBlanca(const QImage &Foto);

    void ReplyMatriculaFoto(const QImage &Foto);
    void ReplyMatriculaFotoRemolque(const QImage &Foto);

    void ReplyMatriculaResults(const t_MatriculaResults &_MatriculaResults);

public slots:
    void ProcessFoto();

public slots:
    //void setFotoCamara(const QImage &Foto){m_fotocamara = new QImage(Foto);}
    //void calibrarBlancas();
    //void calibrarRojas();

    /** Matriculas **/
private:
    QImage *m_fotocamara;

    /** Multi Tareas **/
    QThread *hilo1;
    NewsagesAlprTask *tarea1;

    QThread *hilo2;
    NewsagesAlprTask *tarea2;

    QThread *hilo3;
    NewsagesAlprTask *tarea3;

    QThread *hilo4;
    NewsagesAlprTask *tarea4;

    //cv::Mat QImage2cvMat(QImage image);
    //QImage cvMat2QImage(const cv::Mat &image);
};

} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/
#endif // NEWSAGESALPR_H
