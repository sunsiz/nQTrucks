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

#include <QDataWidgetMapper>
#include <QByteArray>
namespace nQTrucks {


Client::Client(nQTrucksEngine *_engine, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
    , engine(_engine)
{
    ui->setupUi(this);

    /** IO **/
    connect(engine,&nQTrucksEngine::SemaforoEstadoChanged,this,&Client::on_SemaforoEstadoChanged);

    /** BASCULAS **/
    connect(engine ,&nQTrucksEngine::BasculaStatus, this,&Client::on_BasculaConectada);
    connect(engine ,&nQTrucksEngine::BasculaChanged,this,&Client::onBascula);

    /** DAEMON **/
    connect(engine,&nQTrucksEngine::daemonRegistroChanged,this,&Client::onDaemonRegistroChanged);
    ui->BasculaLcd->display("-------");
}

Client::~Client(){
    delete ui;
}

/** NEWSAGES I/O  **/
void Client::on_SemaforoEstadoChanged(int _color){
    switch (_color) {
    case SEMAFORO_VERDE:
        //qDebug() << "SEMAFORO VERDE";
        ui->semaforoVerde->setChecked(true);
        break;
    case SEMAFORO_AMARILLO:
        //qDebug() << "SEMAFORO AMARILLO";
        ui->semaforoAmarillo->setChecked(true); /** MEMORY LEAK **/
        break;
    case SEMAFORO_ROJO:
        //qDebug() << "SEMAFORO ROJO";
        ui->semaforoRojo->setChecked(true); /** MEMORY LEAK **/
        break;
    default:
        //qDebug() << "SEMAFORO VERDE INDETERMINADO";
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

void Client::onBascula(const Bascula &_bascula){
    ui->BasculaLcd->display(_bascula.getIBruto());
    ui->BasculaEstable->setChecked(_bascula.getBEstado()); /** MEMORY LEAK **/
}

void Client::onDaemonRegistroChanged(RegistroMatriculas *_result)
{

    ui->FotoMatriculaA->clear();
    ui->FotoMatriculaB->clear();
    ui->MatriculaA->clear();
    ui->MatriculaB->clear();
    ui->MatriculaPrecisionA->clear();
    ui->MatriculaPrecisionB->clear();


    Tools *m_tools = new Tools(this);
    ui->camara1->setPixmap(QPixmap::fromImage(           m_tools->convertMat2QImage( _result->m_results0->getOrigenFoto())   ));
    ui->camara2->setPixmap(QPixmap::fromImage(           m_tools->convertMat2QImage( _result->m_results1->getOrigenFoto())   ));

    if (  _result->m_results0->getMatriculaPrecisionA()      >=                       _result->m_results1->getMatriculaPrecisionA()   ){

        ui->FotoMatriculaA->setPixmap(QPixmap::fromImage( m_tools->convertMat2QImage(_result->m_results0->getMatriculaFotoA())       ));
        ui->MatriculaA->setText(                                                     _result->m_results0->getMatriculaA()            );
        ui->MatriculaPrecisionA->setText(                                            _result->m_results0->getMatriculaPrecisionAs()  );

    }else{

        ui->FotoMatriculaA->setPixmap(QPixmap::fromImage( m_tools->convertMat2QImage(_result->m_results1->getMatriculaFotoA())       ));
        ui->MatriculaA->setText(                                                     _result->m_results1->getMatriculaA());
        ui->MatriculaPrecisionA->setText(                                            _result->m_results1->getMatriculaPrecisionAs()  );
    }

    if ( _result->m_results0->getMatriculaPrecisionB()       >=                       _result->m_results1->getMatriculaPrecisionB()   ){

        ui->FotoMatriculaB->setPixmap(QPixmap::fromImage( m_tools->convertMat2QImage(_result->m_results0->getMatriculaFotoB())       ));
        ui->MatriculaB->setText(                                                     _result->m_results0->getMatriculaB()            );
        ui->MatriculaPrecisionB->setText(                                            _result->m_results0->getMatriculaPrecisionBs()  );

    }else{

        ui->FotoMatriculaB->setPixmap(QPixmap::fromImage( m_tools->convertMat2QImage(_result->m_results1->getMatriculaFotoB())       ));
        ui->MatriculaB->setText(                                                     _result->m_results1->getMatriculaB()            );
        ui->MatriculaPrecisionB->setText(                                            _result->m_results1->getMatriculaPrecisionBs()  );
    }
    m_tools->deleteLater();
    //engine->setSemaforoStatus(SEMAFORO_VERDE);

}




}

void nQTrucks::Client::on_semaforoVerde_clicked()
{
   engine->setSemaforoStatus(SEMAFORO_VERDE);
}

void nQTrucks::Client::on_semaforoAmarillo_clicked()
{
    engine->setSemaforoStatus(SEMAFORO_AMARILLO);

}

void nQTrucks::Client::on_semaforoRojo_clicked()
{
    engine->setSemaforoStatus(SEMAFORO_ROJO);

}
