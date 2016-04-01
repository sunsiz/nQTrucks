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

#ifndef NQCAMARAS_H
#define NQCAMARAS_H

#include <QObject>
#include <nqtglobal.h>

namespace nQTrucks {

class nqcamaras : public QObject
{
    Q_OBJECT
    Q_ENUMS(CameraType)
    Q_PROPERTY(CameraType TipoCamara  READ TipoCamara WRITE setTipoCamara NOTIFY TipoCamaraChanged)

public:
    explicit nqcamaras(QObject *parent = 0);
    enum CameraType
    {
        HIKVISION,
        CAMTRONIC
    };

    CameraType TipoCamara() const;
    void setTipoCamara(const CameraType &_TipoCamara);

signals:
    void TipoCamaraChanged();

private:
    CameraType m_TipoCamara=CameraType::HIKVISION;

public slots:

};

}
#endif // NQCAMARAS_H
