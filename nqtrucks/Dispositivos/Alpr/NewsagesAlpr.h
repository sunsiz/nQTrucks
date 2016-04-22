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


    /** SETTINGS **/
public slots:
    QImage FotoCamara() const {return m_FotoCamara; }
    void setFotoCamara(const QImage &Foto);
private:
    int         m_nDevice;
    QImage      m_FotoCamara;
    QSettings   *m_settings;
    t_MatriculaResults *m_matricularesults;
    /** END SETTINGS **/


signals:
    void ReplyOriginalFoto(const QImage &Foto);
    void ReplyMatriculaFoto        (const QString &matricula, const QString &confianza, const bool &detectada ,const QImage &Foto);
    void ReplyMatriculaFotoRemolque(const QString &matricula, const QString &confianza, const bool &detectada ,const QImage &Foto);



    /** CALIBRAR *****************************************************/
public slots:
    void calibrarFoto();
signals:
    void ReplyOriginalFotoRoja(const QImage &Foto);
    void ReplyOriginalFotoBlanca(const QImage &Foto);
    /** END CALIBRAR *******************************************************/


    /** Procesar Matriculas **/
public slots:
    void processFoto(const QImage &Foto);
signals:
    void ReplyMatriculaResults(const t_MatriculaResults &_MatriculaResults);
    //void setFotoCamara(const QImage &Foto){m_fotocamara = new QImage(Foto);}
    //void calibrarBlancas();
    //void calibrarRojas();

private:
    /** Multi Tareas **/
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
