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
#include "RegistrosUi.h"
#include "ui_RegistrosUi.h"

#include <QStandardItemModel>
#include <QModelIndex>
#include <QDebug>
#include <QSqlQuery>
namespace nQTrucks {


RegistrosUi::RegistrosUi(nQTrucksEngine *_engine, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegistrosUi)
    , engine(_engine)
{
    ui->setupUi(this);

//id ,fecha, pesobruto, pesoneto , matriculaA1, matriculaB1, matriculaA2, matriculaB2, procesado, emparejado, entrada

    /** FILTRO CALENDARIO **/
    flt_fecha = new QSortFilterProxyModel(this);
    flt_fecha->setSourceModel(engine->RegistrosPesos);
    //flt_fecha->setFilterKeyColumn(1);
    flt_fecha->setDynamicSortFilter(false);

    /** Filtro Procesado **/
    flt_procesado = new QSortFilterProxyModel(this);
    flt_procesado->setSourceModel(flt_fecha);
    //flt_procesado->setFilterKeyColumn(8);
    flt_procesado->setDynamicSortFilter(false);
    flt_procesado->setFilterRegExp(QRegExp(QString::number(ui->selectProcesados->isChecked()),Qt::CaseSensitive, QRegExp::RegExp));

    /** FILTRO SI ES PROCESADO, enseñame solo la entrada **/
    flt_entrada = new QSortFilterProxyModel(this);
    flt_entrada->setSourceModel(flt_procesado);
    //flt_entrada->setFilterKeyColumn(10);
    flt_entrada->setDynamicSortFilter(false);
    flt_entrada->setFilterRegExp(QRegExp(QString::number(ui->selectProcesados->isChecked()),Qt::CaseSensitive, QRegExp::RegExp));

    /** FILTRO FINAL **/
    flt_final = new QSortFilterProxyModel(this);
    flt_final->setSourceModel(flt_entrada);


    ui->tableRegistrosView->verticalHeader()->setVisible(false);
    ui->tableRegistrosView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableRegistrosView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableRegistrosView->setModel(flt_final);

    connect(engine,SIGNAL(rangoFechasChanged(QVector<QDate>)),this,SLOT(rangoFechasChanged(QVector<QDate>)));


//    for (int i = 0; i < engine->RegistrosPesos->columnCount(); i++){
//        ui->tableRegistrosView->hideColumn(i);
//    }

//    ui->tableRegistrosView->showColumn(0);
//    ui->tableRegistrosView->model()->setHeaderData(0,Qt::Horizontal,"Registro");
//    ui->tableRegistrosView->showColumn(4);
//    ui->tableRegistrosView->model()->setHeaderData(4, Qt::Horizontal,"Matrícula1");
//    ui->tableRegistrosView->showColumn(5);
//    ui->tableRegistrosView->model()->setHeaderData(5, Qt::Horizontal,"Matrícula2");
//    ui->tableRegistrosView->showColumn(6);
//    ui->tableRegistrosView->model()->setHeaderData(6,Qt::Horizontal,"Matrícula3");
//    ui->tableRegistrosView->showColumn(7);
//    ui->tableRegistrosView->model()->setHeaderData(7,Qt::Horizontal,"Matrícula4");


    on_calendarioWidget_clicked(QDate::currentDate());    


}

RegistrosUi::~RegistrosUi(){
    delete ui;
}

void RegistrosUi::rangoFechasChanged(const QVector<QDate> &_fechaMinMax){
    ui->calendarioWidget->setMinimumDate(_fechaMinMax[0]);
    ui->calendarioWidget->setMaximumDate(_fechaMinMax[1]);
    ui->calendarioWidget->setDateRange(  _fechaMinMax[0],_fechaMinMax[1]);
    ui->tableRegistrosView->resizeColumnsToContents();
}


void RegistrosUi::on_calendarioWidget_clicked(const QDate &date){
    flt_fecha->setFilterRegExp(QRegExp(date.toString("yyyy-MM-dd").prepend("^").append("(.*)"),
                                    Qt::CaseSensitive,
                                    QRegExp::RegExp));
    qDebug() << "Calendario fecha:"  <<  date.toString("yyyy-MM-dd");//Qt::SystemLocaleShortDate);

}


void RegistrosUi::on_selectProcesados_clicked(bool checked){
   // procesados 19
   // emparejado 20
   // entrada 21
    //engine->RegistrosPesos->query();
    flt_procesado->setFilterRegExp(QRegExp(QString::number(checked),Qt::CaseSensitive, QRegExp::RegExp));
    flt_entrada->setFilterRegExp(QRegExp(QString::number(checked),Qt::CaseSensitive, QRegExp::RegExp));
    if (checked){
        ui->tableRegistrosView->hideColumn(0);

    }else{
        ui->tableRegistrosView->showColumn(0);
        ui->tableRegistrosView->model()->setHeaderData(0,Qt::Horizontal,"Registro");
    }
}

void RegistrosUi::on_tableRegistrosView_clicked(const QModelIndex &index){
    int  row        = index.row();
    long long id    = index.sibling(row, 0).data().toLongLong();
    QTime hora      = index.sibling(row, 1).data().toTime();
    float pesobruto = index.sibling(row, 2).data().toFloat();
    bool procesado  = index.sibling(row, 8).data().toBool();
//    long long id2   = index.sibling(row, 20).data().toLongLong();




//    qDebug() << "row:       " << row;
//    qDebug() << "id:        " << id;
//    qDebug() << "procesado: " << procesado;
//    qDebug() << "id2:       " << id2;

}

void RegistrosUi::on_imprimirInforme_clicked(){
//id ,fecha, pesobruto, pesoneto , matriculaA1, matriculaB1, matriculaA2, matriculaB2, procesado, emparejado, entrada
    if( ui->tableRegistrosView->currentIndex().isValid()){
        int  row       = ui->tableRegistrosView->currentIndex().row();
        bool procesado = ui->tableRegistrosView->currentIndex().sibling(row, 8).data().toBool();
        QVector<long long>  vectorRows = QVector<long long>(2);
        vectorRows[0]  = ui->tableRegistrosView->currentIndex().sibling(row, 9).data().toLongLong();
        vectorRows[1]  = ui->tableRegistrosView->currentIndex().sibling(row, 9).data().toLongLong();
        engine->printReport(procesado,vectorRows);

//        qDebug() << "row:       " << row;
//        qDebug() << "id:        " << vectorRows[0];
//        qDebug() << "procesado: " << procesado;
//        qDebug() << "id2:       " << vectorRows[1];
    }


}
}
