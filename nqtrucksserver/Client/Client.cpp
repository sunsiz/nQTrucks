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

namespace nQTrucks {


Client::Client(nQTrucksEngine *_engine, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
    , engine(_engine)
{
    ui->setupUi(this);

    /** IO **/
    connect(engine,SIGNAL(SemaforoEstadoChanged(int)),this,SLOT(on_SemaforoEstadoChanged(int)));

    /** BASCULAS **/
    connect(engine ,SIGNAL(BasculaStatus(bool)),this,SLOT(on_BasculaConectada(bool)));
    connect(engine ,SIGNAL(BasculaChanged(Bascula)),this,SLOT(onBascula(Bascula)));

    /** DAEMON **/
    connect(engine,SIGNAL(daemonRegistroChanged(SimpleMatriculas)),this,SLOT(onDaemonRegistroChanged(SimpleMatriculas)));
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

void Client::onDaemonRegistroChanged(const SimpleMatriculas &_result)
{
    //_result.convertirFotos();
    ui->camara1->setPixmap(QPixmap::fromImage(_result.results[0].OrigenFotoQ));
    ui->camara2->setPixmap(QPixmap::fromImage(_result.results[1].OrigenFotoQ));

    if ( _result.results[0].MatriculaPrecisionA >= _result.results[1].MatriculaPrecisionA){
        ui->FotoMatriculaA->setPixmap(QPixmap::fromImage(_result.results[0].MatriculaFotoAQ));
        ui->MatriculaA->setText(_result.results[0].MatriculaA);
        ui->MatriculaPrecisionA->setText(_result.results[0].MatriculaPrecisionAs);
    }else{
        ui->FotoMatriculaA->setPixmap(QPixmap::fromImage(_result.results[1].MatriculaFotoAQ));
        ui->MatriculaA->setText(_result.results[1].MatriculaA);
        ui->MatriculaPrecisionA->setText(_result.results[1].MatriculaPrecisionAs);
    }

    if ( _result.results[0].MatriculaPrecisionB >= _result.results[1].MatriculaPrecisionB){
        ui->FotoMatriculaB->setPixmap(QPixmap::fromImage(_result.results[0].MatriculaFotoBQ));
        ui->MatriculaB->setText(_result.results[0].MatriculaB);
        ui->MatriculaPrecisionB->setText(_result.results[0].MatriculaPrecisionBs);
    }else{
        ui->FotoMatriculaB->setPixmap(QPixmap::fromImage(_result.results[1].MatriculaFotoBQ));
        ui->MatriculaB->setText(_result.results[1].MatriculaB);
        ui->MatriculaPrecisionB->setText(_result.results[1].MatriculaPrecisionBs);
    }

}




}
