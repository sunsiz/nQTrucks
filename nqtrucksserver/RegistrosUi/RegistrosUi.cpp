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
#include <QDebug>
namespace nQTrucks {


RegistrosUi::RegistrosUi(nQTrucksEngine *_engine, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegistrosUi)
    , engine(_engine)
{
    ui->setupUi(this);


    ui->tableRegistrosView->verticalHeader()->setVisible(false);

    flt_fecha = new QSortFilterProxyModel(this);
    flt_fecha->setSourceModel(engine->RegistrosPesos);
    flt_fecha->setFilterKeyColumn(1);
    flt_fecha->setDynamicSortFilter(false);



    ui->tableRegistrosView->setModel(flt_fecha);

    connect(engine,SIGNAL(rangoFechasChanged(QVector<QDate>)),this,SLOT(rangoFechasChanged(QVector<QDate>)));


    for (int i = 0; i < engine->RegistrosPesos->columnCount(); i++){
        ui->tableRegistrosView->hideColumn(i);
    }

    ui->tableRegistrosView->showColumn(8);
    ui->tableRegistrosView->model()->setHeaderData(8, Qt::Horizontal,"Matricula1");
    ui->tableRegistrosView->showColumn(9);
    ui->tableRegistrosView->model()->setHeaderData(9, Qt::Horizontal,"Matricula2");
    ui->tableRegistrosView->showColumn(15);
    ui->tableRegistrosView->model()->setHeaderData(15,Qt::Horizontal,"Matricula3");
    ui->tableRegistrosView->showColumn(16);
    ui->tableRegistrosView->model()->setHeaderData(16,Qt::Horizontal,"Matricula4");


    on_calendarioWidget_clicked(QDate::currentDate());


}

RegistrosUi::~RegistrosUi(){
    delete ui;
}

void RegistrosUi::rangoFechasChanged(const QVector<QDate> &_fechaMinMax){
    ui->calendarioWidget->setMinimumDate(_fechaMinMax[0]);
    ui->calendarioWidget->setMaximumDate(_fechaMinMax[1]);
    ui->calendarioWidget->setDateRange(_fechaMinMax[0],_fechaMinMax[1]);
}


void nQTrucks::RegistrosUi::on_calendarioWidget_clicked(const QDate &date){
    flt_fecha->setFilterRegExp(QRegExp(date.toString("yyyy-MM-dd").prepend("^").append("(.*)"),
                                    Qt::CaseSensitive,
                                    QRegExp::RegExp));
    qDebug() << "Calendario fecha:"  <<  date.toString("yyyy-MM-dd");//Qt::SystemLocaleShortDate);

}

}
