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

#include <opencv2/imgproc/imgproc.hpp>
#include "alpr.h"
#include "state_detector.h"


namespace nQTrucks {
namespace Devices {
    using namespace alpr;

class NewsagesAlprTask : public QObject{
    Q_OBJECT
public:
    NewsagesAlprTask(QObject *parent = 0);
    ~NewsagesAlprTask();
private:
    QImage *m_fotocamara;
    cv::Mat QImage2cvMat(QImage image);
private:
    Alpr *matricula;
    Alpr *remolque;

public slots:
    void setFotoCamara(const QImage &Foto){m_fotocamara = new QImage(Foto);}
    void procesarBlancas();
    void procesarRojas();
signals:
    void ReplyMatriculaFoto(const QImage &Foto);
    void ReplyMatriculaRemolqueFoto(const QImage &Foto);
    void workFinished();
};

class NewsagesAlpr : public QObject
{
    Q_OBJECT
public:
    explicit NewsagesAlpr(/*QImage *_fotocamara =0,*/ QObject *parent = 0);

signals:
    void ReplyOriginalFoto(const QImage &Foto);
    void ReplyMatriculaFoto(const QImage &Foto);
    void ReplyMatriculaRemolqueFoto(const QImage &Foto);

public slots:
    void ProcessFoto();

    /** Matriculas **/
private:
    QImage *m_fotocamara;

    /** Multi Tareas **/
    QThread *hilo1;
    NewsagesAlprTask *tarea1;

    QThread *hilo2;
    NewsagesAlprTask *tarea2;

};

} /** END NAMESPACE Devices  **/

} /** END NAMESPACE nQTrucks **/
#endif // NEWSAGESALPR_H
