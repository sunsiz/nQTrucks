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
#ifndef NQTRUCKSENGINE_H
#define NQTRUCKSENGINE_H


#include <QObject>

#include <QSettings>
#include <nqtglobal.h>

namespace nQTrucks {
class nQTrucksEnginePrivate;

class NQTRUCKSLIBSHARED_EXPORT nQTrucksEngine : public QObject
{
    Q_OBJECT
public:
    explicit nQTrucksEngine(QObject *parent=0);
    ~nQTrucksEngine();

    /** CONFIG **/
public:
    static void setSettings(QSettings *value){m_settings=value;}
    void setAppConfig(QSettings *value);
    QSettings *appConfig();
    /** END CONFIG **/

    /** Camaras **/
public:
    QStringList getTiposCamaras();
    void setCamaraIP(int nCamara, QString type,QString host, QString port, QString user, QString passwd);
    void getCamaraFoto(int _ncamara);
signals:

    void CamaraIPFotoCV1(const cv::Mat &fotocv);
    void CamaraIPWeb1(const QString &url);
    void CamaraIPFotoCV2(const cv::Mat &fotocv);
    void CamaraIPWeb2(const QString &url);
    /** END Camaras **/

    /** NEWSAGES I/O **/
public:
    QStringList getIODevices();
public slots:
    void setIODevicesConnect(bool _value);
    void setIODevicesPin10(bool _value);
    void setIODevicesConfig();
signals:
    void IODevicesStatusChanged(const bool &status);
    void IODevicesPIN10Changed(const bool &value);
    /** END NEWSAGES I/O **/

    /** BASCULAS **/
public:
    QStringList getTiposBasculas();
    QStringList getSerialDevices();
public slots:
    void setBasculaConnect(bool _value);
signals:
    void BasculaStatus(const bool &status);
    void BasculaChanged(const t_Bascula &bascula);
    /** END BASCULAS **/

    /** NEWSAGES ALPR **/
public:
    void calibrarFoto(int _device, cv::Mat _foto);
    void getFotoMatricula(int _device, cv::Mat _foto);

    /** ALPR 1 **/
signals:
    void ReplyOriginalFoto1(const cv::Mat &Foto);
    void ReplyOriginalFotoBlanca1(const cv::Mat &Foto);
    void ReplyOriginalFotoRoja1(const cv::Mat &Foto);
    void ReplyMatriculaResults1(const t_MatriculaResults &_results);

    /** ALPR 2 **/
signals:
    void ReplyOriginalFoto2(const cv::Mat &Foto);
    void ReplyOriginalFotoBlanca2(const cv::Mat &Foto);
    void ReplyOriginalFotoRoja2(const cv::Mat &Foto);
    void ReplyMatriculaResults2(const t_MatriculaResults &_results);

    /** END NEWSAGES ALPR **/

    /** CORE ***************************************/
public slots:
    void setInitDaemon(bool _init);

    /** END CORE ************************************/


    /** nQTrucksEnginePrivate **/
protected:
    nQTrucksEnginePrivate * const d_ptr;
    nQTrucksEngine(nQTrucksEnginePrivate &dd, QObject * parent=0);
private:
    Q_DECLARE_PRIVATE(nQTrucksEngine)
    static QSettings* m_settings;
    /** FIN nQTrucksEnginePrivate **/
};

}
#endif // NQTRUCKSENGINE_H
