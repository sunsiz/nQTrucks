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


    /** BASCULAS ********************************************************************************************************/
    Bascula::Bascula(){
        this->clear();
    }
    void Bascula::setBascula(const Bascula &value){
        this->setBEstado(        value.m_bEstado);
        this->setBEstadoAnterior(value.m_bEstadoAnterior);
        this->setIBruto(         value.m_iBruto);
        this->setITara(          value.m_iTara);
        this->setINeto(          value.m_iNeto);
    }
    void Bascula::clear(){
        this->setBEstado(        false);
        this->setBEstadoAnterior(false);
        this->setIBruto(         0);
        this->setITara(          0);
        this->setINeto(          0);
    }

    /** END BASCULAS *****************************************************************************************/

    /** CAMARA **********************************************************************************************/
    Camara::Camara(){
        clear();
    }

    Camara::~Camara(){
        clear();
    }

    void Camara::setCamara(const Camara &value){
        setOrigenFoto(    value.getOrigenFoto());
        setOrigenFotoByte(value.getOrigenFotoByte());
        setOrigenFotoQ(   value.getOrigenFotoQ());
    }
    void Camara::clear(){
        m_OrigenFoto.release();
        m_OrigenFoto =cv::Mat::zeros(fotoSize, CV_8UC3 );
        m_OrigenFotoByte.clear();
        m_OrigenFotoQ.detach();
    }

    cv::Mat Camara::getOrigenFoto() const{
        return m_OrigenFoto;
    }

    void Camara::setOrigenFoto(const cv::Mat value){
        m_OrigenFoto.release();
        m_OrigenFoto = value;

    }

    QByteArray Camara::getOrigenFotoByte() const{        
        return m_OrigenFotoByte;
    }
    void Camara::setOrigenFotoByte(const QByteArray &OrigenFotoByte){
        m_OrigenFotoByte.clear();
        m_OrigenFotoByte = OrigenFotoByte;
    }

    QImage Camara::getOrigenFotoQ() const{
        return m_OrigenFotoQ;
    }

    void Camara::setOrigenFotoQ(const QImage &OrigenFotoQ){
        m_OrigenFotoQ.detach();
        m_OrigenFotoQ = OrigenFotoQ;
    }


    /** END CAMARA **********************************************************************************************/

    MatriculaResults::MatriculaResults()
    { /** MEMORY LEAK **/
        camara = new Camara;
    }




    void MatriculaResults::setMatriculaResults(const MatriculaResults &value){
        clear();
        setTipo(value.getTipo());                                                     //0 para calibracion, 1 para procesado
        setId(value.getId());                                                     //id fuente de captura de foto
        camara = new Camara(value.camara->getCamara());

        setOrigenFotoPrewarp(value.getOrigenFotoPrewarp());

        setOrigenFotoBlanca(value.getOrigenFotoBlanca());                    //  Imagen con calibracion de Blancos
        setOrigenFotoRoja(value.getOrigenFotoRoja());                    // Imagen con calibracion de Rojos

        setMatriculaDetectedA(value.getMatriculaDetectedA());                                                 // Coincide con un patron de busqueda?
        setMatriculaA(value.getMatriculaA());                                                    // STring de la matricula
        setMatriculaFotoA(value.getMatriculaFotoA());              // Imagen recortada de la Matricula
        setMatriculaPrecisionA(value.getMatriculaPrecisionA());                                                     // Precision del OCR
        setMatriculaPrecisionAs(value.getMatriculaPrecisionAs());

        setMatriculaDetectedB(value.getMatriculaDetectedB());                                                 // Coincide con un patron de busqueda?
        setMatriculaB(value.getMatriculaB());                                                    // STring de la matricula
        setMatriculaFotoB(value.getMatriculaFotoB());              // Imagen recortada de la Matricula
        setMatriculaPrecisionB(value.getMatriculaPrecisionB());                                                     // Precision del OCR
        setMatriculaPrecisionBs(value.getMatriculaPrecisionBs());

        convertirFotos();

    }

    void MatriculaResults::clear()
    {
        tipo =0;                                                     //0 para calibracion, 1 para procesado
        id   =0;                                                     //id fuente de captura de foto
        delete camara;
        //camara.clear();

        OrigenFotoPrewarp=cv::Mat::zeros(fotoSize, CV_8UC3 );                    // Imagen con calibracion prewarp

        OrigenFotoBlanca=cv::Mat::zeros(fotoSize, CV_8UC3 );                    //  Imagen con calibracion de Blancos
        OrigenFotoRoja=cv::Mat::zeros(fotoSize, CV_8UC3 );                    // Imagen con calibracion de Rojos

        MatriculaDetectedA=false;                                                 // Coincide con un patron de busqueda?
        MatriculaA="";                                                    // STring de la matricula
        MatriculaFotoA=cv::Mat::zeros( matriculaSize, CV_8UC3 );              // Imagen recortada de la Matricula
        MatriculaPrecisionA=0;                                                     // Precision del OCR
        MatriculaPrecisionAs="0%";

        MatriculaDetectedB=false;                                                 // Coincide con un patron de busqueda?
        MatriculaB="";                                                    // STring de la matricula
        MatriculaFotoB=cv::Mat::zeros( matriculaSize, CV_8UC3 );              // Imagen recortada de la Matricula
        MatriculaPrecisionB=0;                                                     // Precision del OCR
        MatriculaPrecisionBs="0%";
        convertirFotos();
    }

    void MatriculaResults::setOrigenFotoPrewarp(const cv::Mat &value){
        OrigenFotoPrewarp.release();
        OrigenFotoPrewarp = value.clone();
    }
    void MatriculaResults::setOrigenFotoBlanca(const cv::Mat &value){
        OrigenFotoBlanca.release();
        OrigenFotoBlanca = value.clone();
    }
    void MatriculaResults::setOrigenFotoBlancaQ(const QImage &value){
        OrigenFotoBlancaQ.detach();
        OrigenFotoBlancaQ = value;
    }
    void MatriculaResults::setOrigenFotoRoja(const cv::Mat &value){
        OrigenFotoRoja.release();
        OrigenFotoRoja = value.clone();
    }
    void MatriculaResults::setOrigenFotoRojaQ(const QImage &value){
        OrigenFotoRojaQ.detach();
        OrigenFotoRojaQ = value;
    }
    void MatriculaResults::setMatriculaFotoA(const cv::Mat &value){
        MatriculaFotoA.release();
        MatriculaFotoA = value.clone();
    }

    void MatriculaResults::setMatriculaFotoB(const cv::Mat &value){
        MatriculaFotoB.release();
        MatriculaFotoB = value.clone();
    }

    void MatriculaResults::setOrigenFotoPrewarpQ(const QImage &value){
        OrigenFotoPrewarpQ.detach();
        OrigenFotoPrewarpQ = value;
    }
    void MatriculaResults::setMatriculaFotoAByte(const QByteArray &value){
        MatriculaFotoAByte.clear();
        MatriculaFotoAByte = value;
    }
    void MatriculaResults::setMatriculaFotoAQ(const QImage &value){
        MatriculaFotoAQ.detach();
        MatriculaFotoAQ = value;
    }

    void MatriculaResults::setMatriculaFotoBByte(const QByteArray &value){
        MatriculaFotoBByte.clear();
        MatriculaFotoBByte = value;
    }

    void MatriculaResults::setMatriculaFotoBQ(const QImage &value){
        MatriculaFotoBQ.detach();
        MatriculaFotoBQ = value;
    }



    /** REPORTS **/


    RegistroMatriculas::RegistroMatriculas()
        : m_bascula (new Bascula)
        , results0  (new MatriculaResults)
        , results1  (new MatriculaResults)
    {
    }

    void RegistroMatriculas::setRegistroMatriculas(const RegistroMatriculas &value){
        clear();
        m_bascula = new Bascula(value.m_bascula->getBascula());
        results0  = new MatriculaResults(value.results0->getMatriculaResults());
        results1  = new MatriculaResults(value.results1->getMatriculaResults());
        setId(value.getId());
        setFechaRegistro(value.getFechaRegistro());
    }

    void RegistroMatriculas::clear()    {
        delete m_bascula;
        delete results0;
        delete results1;

    }

    long long RegistroMatriculas::getId() const
    {
        return id;
    }

    void RegistroMatriculas::setId(long long value)
    {
        id = value;
    }

    QDateTime RegistroMatriculas::getFechaRegistro() const
    {
        return FechaRegistro;
    }

    void RegistroMatriculas::setFechaRegistro(const QDateTime &value)
    {
        FechaRegistro = value;
    }







































    } /** end Namespace **/
} /** end namespace **/



