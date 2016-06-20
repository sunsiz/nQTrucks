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
#ifndef REGISTROSUI_H
#define REGISTROSUI_H

#include <QMainWindow>

#include "nQTrucksEngine.h"
#include "nqtglobal.h"



namespace Ui {
class RegistrosUi;
}

namespace nQTrucks {

class RegistrosUi : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrosUi(nQTrucksEngine *_engine=nullptr, QWidget *parent = 0);
    ~RegistrosUi();

private:
    Ui::RegistrosUi *ui;

    /** nQTrucks Lib **/
private:
    nQTrucksEngine *engine;
    /** END nQTrucks Lib **/    
};

}
#endif // REGISTROS_H