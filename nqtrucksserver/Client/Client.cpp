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
#include "Client.h"
#include "ui_Client.h"

#include <QDebug>
#include <QDesktopWidget>

#include <QDir>
#include <QUrl>

#include "alpr.h"
#include "prewarp.h"


Client::Client(nQTrucks::nQTrucksEngine *_engine, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
    , engine(_engine)
{
    ui->setupUi(this);
    m_matricularesults.resize(2);

    /** IO **/
    connect(engine,SIGNAL(SemaforoEstadoChanged(int)),this,SLOT(on_SemaforoEstadoChanged(int)));

    /** BASCULAS **/
    connect(engine ,SIGNAL(BasculaStatus(bool)),this,SLOT(on_BasculaConectada(bool)));
    connect(engine ,SIGNAL(BasculaChanged(Bascula)),this,SLOT(onBascula(Bascula)));

    /** ALPR **/
    connect(engine, SIGNAL(ReplyMatriculaResults1(Registros::MatriculaResults)),this,SLOT(onReplyMatriculaResults1(Registros::MatriculaResults)));
    connect(engine, SIGNAL(ReplyMatriculaResults2(Registros::MatriculaResults)),this,SLOT(onReplyMatriculaResults2(Registros::MatriculaResults)));
    m_alpr_count = 0;
}

Client::~Client()
{
    delete ui;
}

/** NEWSAGES I/O  **/
void Client::on_SemaforoEstadoChanged(int _color){
    switch (_color) {
    case SEMAFORO_VERDE:
        ui->semaforoVerde->setChecked(true);
        break;
    case SEMAFORO_AMARILLO:
        ui->semaforoAmarillo->setChecked(true);
        break;
    case SEMAFORO_ROJO:
        ui->semaforoRojo->setChecked(true);
        break;
    default:
        ui->semaforoVerde->setChecked(true);
        break;
    }
}
/** END NEWSAGES I/O **/

/** BASCULAS **/
void Client::on_BasculaConectada(bool conectada){
    if (!conectada){
        ui->BasculaLcd->display("-------");
    }
}

void Client::onBascula(Bascula _bascula){
    ui->BasculaLcd->display(_bascula.iBruto);
    ui->BasculaEstable->setChecked(_bascula.bEstado);
}

void Client::onAllMatriculaResults()
{
    ui->camara1->setPixmap(QPixmap::fromImage(convertMat2QImage(m_matricularesults[0].OrigenFotoResize.clone())));
    ui->camara2->setPixmap(QPixmap::fromImage(convertMat2QImage(m_matricularesults[1].OrigenFotoResize.clone())));

    if ( m_matricularesults[0].MatriculaPrecisionA >= m_matricularesults[1].MatriculaPrecisionA){
        ui->FotoMatriculaA->setPixmap(QPixmap::fromImage(convertMat2QImage(m_matricularesults[0].MatriculaFotoA.clone())));
        ui->MatriculaA->setText(m_matricularesults[0].MatriculaA);
        ui->MatriculaPrecisionA->setText(m_matricularesults[0].MatriculaPrecisionAs);
    }else{
        ui->FotoMatriculaA->setPixmap(QPixmap::fromImage(convertMat2QImage(m_matricularesults[1].MatriculaFotoA.clone())));
        ui->MatriculaA->setText(m_matricularesults[1].MatriculaA);
        ui->MatriculaPrecisionA->setText(m_matricularesults[1].MatriculaPrecisionAs);
    }

    if ( m_matricularesults[0].MatriculaPrecisionB >= m_matricularesults[1].MatriculaPrecisionB){
        ui->FotoMatriculaB->setPixmap(QPixmap::fromImage(convertMat2QImage(m_matricularesults[0].MatriculaFotoB.clone())));
        ui->MatriculaB->setText(m_matricularesults[0].MatriculaB);
        ui->MatriculaPrecisionB->setText(m_matricularesults[0].MatriculaPrecisionBs);
    }else{
        ui->FotoMatriculaB->setPixmap(QPixmap::fromImage(convertMat2QImage(m_matricularesults[1].MatriculaFotoB.clone())));
        ui->MatriculaB->setText(m_matricularesults[1].MatriculaB);
        ui->MatriculaPrecisionB->setText(m_matricularesults[1].MatriculaPrecisionBs);
    }
    m_alpr_count=0;
}
/** END BASCULAS **/


void Client::onReplyMatriculaResults1(const Registros::MatriculaResults &_result){
    m_matricularesults[0] = _result;
    m_alpr_count++;
    if (m_alpr_count==2){
        onAllMatriculaResults();
    }
}

void Client::onReplyMatriculaResults2(const Registros::MatriculaResults &_result){
    m_matricularesults[1] = _result;
    m_alpr_count++;
    if (m_alpr_count==2){
        onAllMatriculaResults();
    }
}

/** CONVERSORES ***********************************/
QImage Client::convertMat2QImage(const cv::Mat& src)
{
    QImage qtImg;
    if( !src.empty() && src.depth() == CV_8U ){
        if(src.channels() == 1){
            qtImg = QImage( (const unsigned char *)(src.data),
                            src.cols,
                            src.rows,
                            QImage::Format_Indexed8 );
        }
        else{
            cv::cvtColor( src, src, CV_BGR2RGB );
            qtImg = QImage( (const unsigned char *)(src.data),
                            src.cols,
                            src.rows,
                            src.step,
                            QImage::Format_RGB888 );
        }
    }
    return qtImg.copy();
}
/** END CONVERSORES ***********************************/

