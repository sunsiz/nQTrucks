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
    connect(engine,&nQTrucksEngine::SemaforoEstadoChanged,this,&Client::on_SemaforoEstadoChanged);

    /** BASCULAS **/
    connect(engine ,&nQTrucksEngine::BasculaStatus, this,&Client::on_BasculaConectada);
    connect(engine ,&nQTrucksEngine::BasculaChanged,this,&Client::onBascula);

    /** DAEMON **/
    connect(engine,&nQTrucksEngine::daemonRegistroChanged,this,&Client::onDaemonRegistroChanged);
}

Client::~Client(){
    delete ui;
}

/** NEWSAGES I/O  **/
void Client::on_SemaforoEstadoChanged(int _color){
    switch (_color) {
    case SEMAFORO_VERDE:
        ui->semaforoVerde->setChecked(true);
        break;
    case SEMAFORO_AMARILLO:
        ui->semaforoAmarillo->setChecked(true); /** MEMORY LEAK **/
        break;
    case SEMAFORO_ROJO:
        ui->semaforoRojo->setChecked(true);     /** MEMORY LEAK **/
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

void Client::onBascula(const Bascula &_bascula){
    ui->BasculaLcd->display(_bascula.getIBruto());
    ui->BasculaEstable->setChecked(_bascula.getBEstado()); /** MEMORY LEAK **/
}

void Client::onDaemonRegistroChanged(const RegistroMatriculas &_result)
{

    RegistroMatriculas *m_regs= new RegistroMatriculas;
    m_regs->setRegistroMatriculas(_result);
    Tools *m_tools = new Tools;

    m_regs->m_results0->camara->setOrigenFotoQ(m_tools->convertMat2QImage(_result.m_results0->camara->getOrigenFoto()));
    m_regs->m_results1->camara->setOrigenFotoQ(m_tools->convertMat2QImage(_result.m_results1->camara->getOrigenFoto()));
    ui->camara1->setPixmap(QPixmap::fromImage(m_regs->m_results0->camara->getOrigenFotoQ()));
    ui->camara2->setPixmap(QPixmap::fromImage(m_regs->m_results1->camara->getOrigenFotoQ()));
    m_regs->m_results0->camara->getOrigenFotoQ().detach();
    m_regs->m_results1->camara->getOrigenFotoQ().detach();

    if ( m_regs->m_results0->getMatriculaPrecisionA() >= m_regs->m_results1->getMatriculaPrecisionA()){

        m_regs->m_results0->setMatriculaFotoAQ(m_tools->convertMat2QImage(m_regs->m_results0->getMatriculaFotoA()));
        ui->FotoMatriculaA->setPixmap(QPixmap::fromImage(m_regs->m_results0->getMatriculaFotoAQ()));
        m_regs->m_results0->getMatriculaFotoAQ().detach();

        ui->MatriculaA->setText(_result.m_results0->getMatriculaA());/** MEMORY LEAK **/
        ui->MatriculaPrecisionA->setText(_result.m_results0->getMatriculaPrecisionAs());/** MEMORY LEAK **/

    }else{

        m_regs->m_results1->setMatriculaFotoAQ(m_tools->convertMat2QImage(m_regs->m_results1->getMatriculaFotoA()));
        ui->FotoMatriculaA->setPixmap(QPixmap::fromImage(m_regs->m_results1->getMatriculaFotoAQ()));
        m_regs->m_results1->getMatriculaFotoAQ().detach();

        ui->MatriculaA->setText(_result.m_results1->getMatriculaA());
        ui->MatriculaPrecisionA->setText(_result.m_results1->getMatriculaPrecisionAs());
    }

    if ( m_regs->m_results0->getMatriculaPrecisionB() >= m_regs->m_results1->getMatriculaPrecisionB()){

        m_regs->m_results0->setMatriculaFotoBQ(m_tools->convertMat2QImage(m_regs->m_results0->getMatriculaFotoB()));
        ui->FotoMatriculaB->setPixmap(QPixmap::fromImage(m_regs->m_results0->getMatriculaFotoBQ()));
        m_regs->m_results0->getMatriculaFotoBQ().detach();

        ui->MatriculaB->setText(m_regs->m_results0->getMatriculaB());/** MEMORY LEAK **/
        ui->MatriculaPrecisionB->setText(m_regs->m_results0->getMatriculaPrecisionBs());/** MEMORY LEAK **/

    }else{

        m_regs->m_results1->setMatriculaFotoBQ(m_tools->convertMat2QImage(m_regs->m_results1->getMatriculaFotoB()));
        ui->FotoMatriculaB->setPixmap(QPixmap::fromImage(m_regs->m_results1->getMatriculaFotoBQ()));
        m_regs->m_results1->getMatriculaFotoBQ().detach();

        ui->MatriculaB->setText(m_regs->m_results1->getMatriculaB());
        ui->MatriculaPrecisionB->setText(m_regs->m_results1->getMatriculaPrecisionBs());
    }

    delete m_tools;
    delete m_regs;

}




}
