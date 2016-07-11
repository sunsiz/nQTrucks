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
#pragma once
#ifndef NQTRUCKS_H
#define NQTRUCKS_H
#include <QtCore/qglobal.h>
#include <stdexcept>
#include <QString>

#include <QSettings>

#include "Registros/MatriculaResults.h"
#include "Registros/RegistroMatriculas.h"

#if defined(NQTRUCKS_LIBRARY)
#  define NQTRUCKSLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NQTRUCKSLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

/** SETTINGS **/
namespace nQTrucks
{

    #define SEMAFORO_VERDE 0
    #define SEMAFORO_AMARILLO 1
    #define SEMAFORO_ROJO 2

    #define RELE_ON  false
    #define RELE_OFF true

    #define NEWSAGESIO "NEWSAGESIO"

    QString extractClassName(QString className);
    class nQTrucksError : public std::runtime_error{
    public:
        nQTrucksError(const QString& message):std::runtime_error(message.toStdString()){}
    };

    /** CAMPOS REGISTRO_MATRICULAS **/
    #define REGISTROS_MATRICULAS_ID                 0
    #define REGISTROS_MATRICULAS_FECHA              1
    #define REGISTROS_MATRICULAS_PESOBRUTO          2
    #define REGISTROS_MATRICULAS_PESONETO           3
    #define REGISTROS_MATRICULAS_PESOTARA           4
    #define REGISTROS_MATRICULAS_FOTOCAMARA1        5
    #define REGISTROS_MATRICULAS_FOTOMATRICULAA1    6
    #define REGISTROS_MATRICULAS_FOTOMATRICULAB1    7
    #define REGISTROS_MATRICULAS_MATRICULAA1        8
    #define REGISTROS_MATRICULAS_MATRICULAB1        9
    #define REGISTROS_MATRICULAS_PRECISIONA1        10
    #define REGISTROS_MATRICULAS_PRECISIONB1        11
    #define REGISTROS_MATRICULAS_FOTOCAMARA2        12
    #define REGISTROS_MATRICULAS_FOTOMATRICULAA2    13
    #define REGISTROS_MATRICULAS_FOTOMATRICULAB2    14
    #define REGISTROS_MATRICULAS_MATRICULAA2        15
    #define REGISTROS_MATRICULAS_MATRICULAB2        16
    #define REGISTROS_MATRICULAS_PRECISIONA2        17
    #define REGISTROS_MATRICULAS_PRECISIONB2        18
    #define REGISTROS_MATRICULAS_PROCESADO          19
    #define REGISTROS_MATRICULAS_EMPAREJADO         20
    #define REGISTROS_MATRICULAS_ENTRADA            21


    #define EMPRESA_ID            0
    #define EMPRESA_RAZON         1
    #define EMPRESA_NIF           2
    #define EMPRESA_DIRECCION1    3
    #define EMPRESA_DIRECCION2    4
    #define EMPRESA_DIRECCION3    5
    #define EMPRESA_CERTIFICADO   6
    #define EMPRESA_ENAC          7
    #define EMPRESA_LOGO          8


    namespace Const{
    } //end namespace Const
} // namespace nQTrucks
#endif // NQTRUCKS_H
