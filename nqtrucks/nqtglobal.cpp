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
#include "nqtglobal.h"

#include <QString>
#include <QCoreApplication>
#include <QDir>
#include <QBuffer>

//QSettings * nQTrucks::nQTrucksConfig::m_settings = 0;

QString nQTrucks::extractClassName(QString className)
{
    int startPos=className.lastIndexOf("::");
    if(startPos==-1) startPos=0;
    else startPos+=2;
    return className.right(className.length()-startPos);
}


namespace nQTrucks {
    namespace Registros{
        MatriculaResults::MatriculaResults(){ /** MEMORY LEAK **/
            tipo                 =0;                                                     //0 para calibracion, 1 para procesado
            id                   =0;                                                     //id fuente de captura de foto

            MatriculaDetectedA   =false;                                                 // Coincide con un patron de busqueda?
            MatriculaA           ="";                                                    // STring de la matricula
            MatriculaPrecisionA  =0;                                                     // Precision del OCR
            MatriculaPrecisionAs ="0%";

            MatriculaDetectedB   =false;                                                 // Coincide con un patron de busqueda?
            MatriculaB           ="";                                                    // STring de la matricula
            MatriculaPrecisionB  =0;                                                     // Precision del OCR
            MatriculaPrecisionBs ="0%";
        }
    } /** end Namespace **/
} /** end namespace **/



