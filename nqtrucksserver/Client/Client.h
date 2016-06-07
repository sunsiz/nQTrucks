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
#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>

#include "nQTrucksEngine.h"
#include "nqtglobal.h"


namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(nQTrucks::nQTrucksEngine *_engine=nullptr, QWidget *parent = 0);
    ~Client();

private:
    Ui::Client *ui;
    bool m_running=false;

    /** nQTrucks Lib **/
private:
    nQTrucks::nQTrucksEngine *engine;
    /** END nQTrucks Lib **/

    /** NEWSAGES I/O  **/
private slots:
    void on_SemaforoEstadoChanged(int _color);
    /** END NEWSAGES I/O  **/

    /** BASCULAS **/
private slots:
    void on_BasculaConectada(bool conectada);
    void onBascula(t_Bascula _bascula);
    /** END BASCULAS **/


    /** ALPR **/
private:
    int m_alpr_count;
    QVector<Registros::t_MatriculaResults> m_matricularesults={};
    QVector<Registros::t_MatriculaResults>::iterator m_matricularesults_iterator;

private slots:
    void onAllMatriculaResults();
    void onReplyMatriculaResults1(const Registros::t_MatriculaResults &_result);
    void onReplyMatriculaResults2(const Registros::t_MatriculaResults &_result);
    /** END ALPR **/


    /** CONFIG **/
private:
    void loadconfig();
    /** END CONFIG **/

    static QImage convertMat2QImage(const cv::Mat &src);
};

#endif // CLIENT_H
