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
#include "Configuracion.h"
#include "ui_Configuracion.h"

#include <QDebug>
#include <QDesktopWidget>

#include <QDataWidgetMapper>
#include <QByteArray>
#include  <QVariant>

//#include "alpr.h"
//#include "prewarp.h"

namespace nQTrucks{

Configuracion::Configuracion(nQTrucksEngine *_engine, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Configuracion)
    , engine(_engine)
{
    m_matricularesults.resize(2);

    /** CAMARAS **/
    connect(engine,&nQTrucksEngine::CamaraIP1,this,&Configuracion::onGetFoto1);
    connect(engine,&nQTrucksEngine::CamaraIP2,this,&Configuracion::onGetFoto2);


    /** IO **/
    connect(engine,&nQTrucksEngine::SemaforoConnectChanged,this,&Configuracion::on_SemaforoConnect);
    connect(engine,&nQTrucksEngine::SemaforoEstadoChanged, this,&Configuracion::on_SemaforoEstadoChanged);

    /** BASCULAS **/
    connect(engine ,&nQTrucksEngine::BasculaStatus, this,&Configuracion::on_BasculaConectada);
    connect(engine ,&nQTrucksEngine::BasculaChanged,this,&Configuracion::onBascula);

    /** ALPR 1 **/
    connect(engine ,&nQTrucksEngine::ReplyMatriculaResults1,           this,&Configuracion::onReplyMatriculaResults1);
    connect(engine ,&nQTrucksEngine::ReplyOriginalFoto1,               this,&Configuracion::onGetOriginalMatricula1);
    connect(engine ,&nQTrucksEngine::ReplyMatriculaCalibrationResults1,this,&Configuracion::onGetCalibrationResult1);
    /** ALPR 2 **/
    connect(engine, &nQTrucksEngine::ReplyMatriculaResults2,           this,&Configuracion::onReplyMatriculaResults2);
    connect(engine ,&nQTrucksEngine::ReplyOriginalFoto2,               this,&Configuracion::onGetOriginalMatricula2);
    connect(engine ,&nQTrucksEngine::ReplyMatriculaCalibrationResults2,this,&Configuracion::onGetCalibrationResult2);

    ui->setupUi(this);
    loadconfig();
    //setFixedSize(1024,768);
    //this->showFullScreen();
}

Configuracion::~Configuracion(){
    m_matricularesults[0]->deleteLater();
    m_matricularesults[1]->deleteLater();
    m_matricularesults.detach();
    m_mapper->deleteLater();
    delete ui;
}


void Configuracion::updateGui(){
    ui->camaraLabel->clear();

    Tools *m_tools = new Tools(this);
    switch (ui->CamaraSelect->currentIndex()) {
    case 0:
        ui->camaraLabel->setPixmap(QPixmap::fromImage( m_tools->convertMat2QImage(m_matricularesults[0]->getOrigenFoto()) ));
        break;
    case 1:
        ui->camaraLabel->setPixmap(QPixmap::fromImage( m_tools->convertMat2QImage(m_matricularesults[1]->getOrigenFoto()) ));
        break;
    }
    m_tools->deleteLater();
}



/** SETTINGS **/
void Configuracion::loadconfig()
{

    /** DEFAULTS UI **/
    m_matricularesults[0] = new MatriculaResults(this);
    m_matricularesults[1] = new MatriculaResults(this);
    /** END DEFAULT UIS **/

    /** CAMARAS **/
    ui->CamaraSelect->addItem(CAMARA1);
    ui->CamaraSelect->addItem(CAMARA2);
    /** END CAMARAS **/

    /** BASCULAS **/
    ui->BasculaLcd->display("-------");
    ui->BasculaLcd2->display("-------");
    ui->BasculaLcd3->display("-------");
    ui->BasculaEstable->setChecked(false);
    ui->BasculaEstable->setAutoExclusive(false);

    QStringList l_BasculaTipo = engine->getTiposBasculas();
    ui->BasculaTipoComboBox->addItems(l_BasculaTipo);

    QStringList l_BasculaDevices = engine->getSerialDevices();
    ui->BasculaDevicesComboBox->addItems(l_BasculaDevices);

    ui->BasculaTipoComboBox->setCurrentIndex(engine->appConfig()->value(QString(BASCULA) + "/tipo","0").toInt());
    ui->BasculaErrorMinimo->setValue(        engine->appConfig()->value(QString(BASCULA) + "/tolerancia_minima","10").toInt());
    ui->BasculaFactorEstable->setValue(      engine->appConfig()->value(QString(BASCULA) + "/factor_estable","1").toInt());
    /** END BASCULAS **/

    /** ALPR  **/
    ui->calibracionSelect->addItem(ALPR1);
    ui->calibracionSelect->addItem(ALPR2);   

    /** REPORTS **/
    engine->report_buildReportsTree(ui->reportsTreeWidget);
    /** END REPORTS **/

    updateGui();
    updateCalibracionGui();


    /** EMPRESAS **/
    m_mapper->setOrientation(Qt::Horizontal);
    m_mapper->setModel(engine->Empresa);
    reloadEmpresa();
}
/** END SETTINGS **/


/** CAMARAS *************************************************************************/
    /** CAMARA1 **/
void Configuracion::onGetFoto1(const MatriculaResults &_camara){
    m_matricularesults[0]->setMatriculaResults(_camara);
    updateGui();
    updateCalibracionGui();
}
    /** CAMARA2 **/
void Configuracion::onGetFoto2(const MatriculaResults &_camara){
    m_matricularesults[1]->setMatriculaResults(_camara);
    updateGui();
    updateCalibracionGui();
}

void Configuracion::on_CamaraSelect_currentIndexChanged(int index)
{
    QStringList l_Camaras = engine->getTiposCamaras();
    ui->camaraTipo->clear();
    ui->camaraTipo->addItems(l_Camaras);

    switch (index) {
    case 0:
        ui->camaraTipo->setCurrentIndex(engine->appConfig()->value( QString(CAMARA1) + "/tipo","0").toInt());
        ui->hostLineEdit->setText(engine->appConfig()->value(       QString(CAMARA1) + "/host").toString());
        ui->puertoLineEdit->setText(engine->appConfig()->value(     QString(CAMARA1) + "/port").toString());
        ui->usuarioLineEdit->setText(engine->appConfig()->value(    QString(CAMARA1) + "/user").toString());
        ui->passwdLineEdit->setText(engine->appConfig()->value(     QString(CAMARA1) + "/pass").toString());
        break;
    case 1:
        ui->camaraTipo->setCurrentIndex(engine->appConfig()->value( QString(CAMARA2) + "/tipo","0").toInt());
        ui->hostLineEdit->setText(engine->appConfig()->value(       QString(CAMARA2) + "/host").toString());
        ui->puertoLineEdit->setText(engine->appConfig()->value(     QString(CAMARA2) + "/port").toString());
        ui->usuarioLineEdit->setText(engine->appConfig()->value(    QString(CAMARA2) + "/user").toString());
        ui->passwdLineEdit->setText(engine->appConfig()->value(     QString(CAMARA2) + "/pass").toString());
        break;
    }
    updateGui();
}



void Configuracion::on_GuardarCamara_clicked()
{
    switch (ui->CamaraSelect->currentIndex()) {
    case 0:
        engine->appConfig()->setValue(QString(CAMARA1) + "/tipo",QString::number(ui->camaraTipo->currentIndex()));
        engine->appConfig()->setValue(QString(CAMARA1) + "/host",ui->hostLineEdit->text());
        engine->appConfig()->setValue(QString(CAMARA1) + "/port",ui->puertoLineEdit->text());
        engine->appConfig()->setValue(QString(CAMARA1) + "/user",ui->usuarioLineEdit->text());
        engine->appConfig()->setValue(QString(CAMARA1) + "/pass",ui->passwdLineEdit->text());
        engine->getCamaraFoto(0);
        break;
    case 1:
        engine->appConfig()->setValue(QString(CAMARA2) + "/tipo",QString::number(ui->camaraTipo->currentIndex()));
        engine->appConfig()->setValue(QString(CAMARA2) + "/host",ui->hostLineEdit->text());
        engine->appConfig()->setValue(QString(CAMARA2) + "/port",ui->puertoLineEdit->text());
        engine->appConfig()->setValue(QString(CAMARA2) + "/user",ui->usuarioLineEdit->text());
        engine->appConfig()->setValue(QString(CAMARA2) + "/pass",ui->passwdLineEdit->text());
        engine->getCamaraFoto(1);
        break;
    }

}
/** END CAMARAS ************************************************************************/



/** NEWSAGES I/O  **/
void Configuracion::on_conectarSemaforo_clicked(){
    //on_guardarSemaforo_clicked();
    engine->setSemaforoDevicesConnect(true);
}

void Configuracion::on_desconectarSemaforo_clicked(){
    engine->setSemaforoDevicesConnect(false);
}

void Configuracion::on_semaforoVerde_clicked(){
    engine->setSemaforoStatus(SEMAFORO_VERDE);
}

void Configuracion::on_semaforoAmarillo_clicked(){
    engine->setSemaforoStatus(SEMAFORO_AMARILLO);
}

void Configuracion::on_semaforoRojo_clicked(){
    engine->setSemaforoStatus(SEMAFORO_ROJO);
}

void Configuracion::on_SemaforoConnect(bool status){
    ui->semaforoConectado->setChecked(status);/** MEMORY LEAK **/
}

void Configuracion::on_SemaforoEstadoChanged(int _color){
    switch (_color) {
    case SEMAFORO_VERDE:
        qDebug() << "SEMAFORO VERDE";
        ui->semaforoVerde->setChecked(true);
        break;
    case SEMAFORO_AMARILLO:
        qDebug() << "SEMAFORO AMARILLO";
        ui->semaforoAmarillo->setChecked(true); /** MEMORY LEAK **/
        break;
    case SEMAFORO_ROJO:
        qDebug() << "SEMAFORO ROJO";
        ui->semaforoRojo->setChecked(true); /** MEMORY LEAK **/
        break;
    default:
        qDebug() << "SEMAFORO VERDE INDETERMINADO";
        ui->semaforoVerde->setChecked(true);
        break;
    }
}
/** END NEWSAGES I/O **/

/** BASCULAS **/
void Configuracion::on_actualizarBasculas_clicked(){
    ui->BasculaDevicesComboBox->clear();
    QStringList l_Devices = engine->getSerialDevices();
    ui->BasculaDevicesComboBox->addItems(l_Devices);
}

void Configuracion::on_conectarBascula_clicked(){
    on_guardarBascula_clicked();
    engine->setBasculaConnect(true);
}

void Configuracion::on_desconectarBascula_clicked(){
    engine->setBasculaConnect(false);
}

void Configuracion::on_guardarBascula_clicked(){
    engine->appConfig()->setValue(QString(BASCULA) + "/device",            ui->BasculaDevicesComboBox->currentText());
    engine->appConfig()->setValue(QString(BASCULA) + "/tipo",              QString::number(ui->BasculaTipoComboBox->currentIndex()));
    engine->appConfig()->setValue(QString(BASCULA) + "/tolerancia_minima", QString::number(ui->BasculaErrorMinimo->value()));
    engine->appConfig()->setValue(QString(BASCULA) + "/factor_estable",    QString::number(ui->BasculaFactorEstable->value()));
}

void Configuracion::on_BasculaConectada(bool conectada){
    ui->BasculaStatus->setChecked(conectada);/** MEMORY LEAK **/
    if (!conectada){
        ui->BasculaLcd->display("-------");
        ui->BasculaLcd2->display("-------");
        ui->BasculaLcd3->display("-------");
        ui->BasculaStatus->setChecked(false);
    }
}

void Configuracion::onBascula(const Bascula &_bascula){          
    ui->BasculaLcd->display(_bascula.getIBruto());
    ui->BasculaLcd2->display(_bascula.getITara());
    ui->BasculaLcd3->display(_bascula.getINeto());
    ui->BasculaEstable->setChecked(_bascula.getBEstado()); /** MEMORY LEAK **/
}
/** END BASCULAS **/


/** CALIBRACION ***************************************************************/
    /** GUI **/
void Configuracion::on_calibracionSelect_currentIndexChanged(int index){
    setAlprIndex(index);
    loadPlanks(index);
    updateCalibracionGui();
}

void Configuracion::loadPlanks(const int &index){
    switch (index) {
    case 0:
        ui->vPlankA->setValue(engine->appConfig()->value(QString(ALPR) + "/planka1","0").toInt());
        ui->vPlankB->setValue(engine->appConfig()->value(QString(ALPR) + "/plankb1","0").toInt());
        ui->vPlankC->setValue(engine->appConfig()->value(QString(ALPR) + "/plankc1","0").toInt());
        break;
    case 1:
        ui->vPlankA->setValue(engine->appConfig()->value(QString(ALPR) + "/planka2","0").toInt());
        ui->vPlankB->setValue(engine->appConfig()->value(QString(ALPR) + "/plankb2","0").toInt());
        ui->vPlankC->setValue(engine->appConfig()->value(QString(ALPR) + "/plankc2","0").toInt());
        break;
    }
}
void Configuracion::on_TestMatricula_clicked(){
    int index = getAlprIndex();
    on_guardarPlanK_clicked();
    engine->getFotoMatricula(index,m_matricularesults[index]->getMatriculaResults());
}
void Configuracion::on_ActualizarCamara_clicked(){
    int index = getAlprIndex();
    engine->getCamaraFoto(index);
}
void Configuracion::updateCalibracionGui(){
    ui->FotoOriginalA->clear();
    ui->FotoMatriculaA->clear();
    ui->FotoMatriculaB->clear();
    ui->MatriculaA->clear();
    ui->MatriculaB->clear();
    ui->FotoBlancosA->clear();
    ui->FotoRojosA->clear();
    ui->LongMatriculaA->clear();
    ui->LongMatriculaB->clear();

    int index = getAlprIndex();

    Tools *m_tools = new Tools(this);
    ui->FotoOriginalA->setPixmap( QPixmap::fromImage( m_tools->convertMat2QImage(m_matricularesults[index]->getOrigenFoto())  ));
    ui->FotoMatriculaA->setPixmap(QPixmap::fromImage( m_tools->convertMat2QImage(m_matricularesults[index]->getMatriculaFotoA())      ));
    ui->FotoMatriculaB->setPixmap(QPixmap::fromImage( m_tools->convertMat2QImage(m_matricularesults[index]->getMatriculaFotoB())      ));
    ui->FotoBlancosA->setPixmap(  QPixmap::fromImage( m_tools->convertMat2QImage(m_matricularesults[index]->getOrigenFotoBlanca())    ));
    ui->FotoRojosA->setPixmap(    QPixmap::fromImage( m_tools->convertMat2QImage(m_matricularesults[index]->getOrigenFotoRoja())      ));
    ui->MatriculaA->setText(m_matricularesults[index]->getMatriculaA());
    ui->MatriculaB->setText(m_matricularesults[index]->getMatriculaB());
    ui->LongMatriculaA->setText(m_matricularesults[index]->getMatriculaPrecisionAs());
    ui->LongMatriculaB->setText(m_matricularesults[index]->getMatriculaPrecisionBs());
    m_tools->deleteLater();
}
    /** END GUI **/

/** ALPR **/
    /** ALPR 1 **/
void Configuracion::onReplyMatriculaResults1(const MatriculaResults &_result){
    m_matricularesults[0]->setMatriculaResults(_result);
    loadPlanks(0);
    updateCalibracionGui();
}

void Configuracion::onGetOriginalMatricula1(const MatriculaResults &_camara){
    m_matricularesults[0]->setMatriculaResults(_camara);
    updateCalibracionGui();
}

void Configuracion::onGetCalibrationResult1(const MatriculaResults &_calibration_result){
    m_matricularesults[0]->setMatriculaResults(_calibration_result);
    updateCalibracionGui();
}
    /** END ALPR1 **/
    /** ALPR2 **/
void Configuracion::onReplyMatriculaResults2(const MatriculaResults &_result){
    m_matricularesults[1]->setMatriculaResults(_result);
    loadPlanks(1);
    updateCalibracionGui();
}

void Configuracion::onGetOriginalMatricula2(const MatriculaResults &_camara){
    m_matricularesults[1]->setMatriculaResults(_camara);
    updateCalibracionGui();
}

void Configuracion::onGetCalibrationResult2(const MatriculaResults &_calibration_result){
    m_matricularesults[1]->setMatriculaResults(_calibration_result);
    updateCalibracionGui();
}

    /** END ALPR2 **/
    /** PLANKs **/
void Configuracion::on_vPlankA_valueChanged(int value){
    ui->lvPlankA->setText(QString::number( value));
}
void Configuracion::on_vPlankB_valueChanged(int value){
    ui->lvPlankB->setText(QString::number( value));
}
void Configuracion::on_vPlankC_valueChanged(int value){
    ui->lvPlankC->setText(QString::number( value));
}

void Configuracion::on_guardarPlanK_clicked(){
    int index = getAlprIndex();
    switch (index) {
    case 0:
        engine->appConfig()->setValue(QString(ALPR) + "/planka1",QString::number(ui->vPlankA->value()));
        engine->appConfig()->setValue(QString(ALPR) + "/plankb1",QString::number(ui->vPlankB->value()));
        engine->appConfig()->setValue(QString(ALPR) + "/plankc1",QString::number(ui->vPlankC->value()));
        break;
    case 1:
        engine->appConfig()->setValue(QString(ALPR) + "/planka2",QString::number(ui->vPlankA->value()));
        engine->appConfig()->setValue(QString(ALPR) + "/plankb2",QString::number(ui->vPlankB->value()));
        engine->appConfig()->setValue(QString(ALPR) + "/plankc2",QString::number(ui->vPlankC->value()));
        break;
    }
    engine->calibrarFoto(index,m_matricularesults[index]->getMatriculaResults());
}
    /** END PLANKs **/
/** END CALIBRACION *******************************************************************/

void Configuracion::on_reportsTreeWidget_doubleClicked(const QModelIndex &index){
    qDebug() << index.data(Qt::UserRole).toString();
    engine->report_loadFromFile(index.data(Qt::UserRole).toString());
    engine->report_desingReport();
}

void Configuracion::on_guardarEmpresa_clicked(){
    QVector<QString> empresa_info = QVector<QString>(7);
    empresa_info[0] = ui->empresa_razon->text();
    empresa_info[1] = ui->empresa_nif->text();
    empresa_info[2] = ui->empresa_direccion1->text();
    empresa_info[3] = ui->empresa_direccion2->text();
    empresa_info[4] = ui->empresa_direccion3->text();
    empresa_info[5] = ui->empresa_certificado->text();
    empresa_info[6] = ui->empresa_enac->text();
    engine->updateEmpresa(empresa_info);
    reloadEmpresa();
}

void Configuracion::reloadEmpresa(){


    m_mapper->addMapping(ui->empresa_razon,         1);
    m_mapper->addMapping(ui->empresa_nif,           2);
    m_mapper->addMapping(ui->empresa_direccion1,    3);
    m_mapper->addMapping(ui->empresa_direccion2,    4);
    m_mapper->addMapping(ui->empresa_direccion3,    5);
    m_mapper->addMapping(ui->empresa_certificado,   6);
    m_mapper->addMapping(ui->empresa_enac,          7);
    m_mapper->toFirst();

    QPixmap pixmap;
    if (pixmap.loadFromData(engine->Empresa->index(m_mapper->currentIndex(),8).data().toByteArray()) )
    {
        ui->empresa_logo->setPixmap(pixmap);
    }

}


} //end Namespace


