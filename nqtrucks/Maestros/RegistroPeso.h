#ifndef REGISTROPESO_H
#define REGISTROPESO_H

#include <Db/QueryModel.h>
#include "nqtglobal.h"

namespace nQTrucks{
    namespace Maestros{

        class RegistroPeso : public Db::QueryModel {
            Q_OBJECT
        public:
           explicit RegistroPeso(QObject *parent=0);
        public slots:
            void syncTable() {setTable();}

        protected:
            void setTable();
            void refreshModel() {setTable();}
            void configQueries();
        private:
            QVector<QDate> m_fecha_min_max= QVector<QDate>(2);
            void setFecha_min_max();

        protected:
            QString m_DefaultQuery;// = "select * from " + QString(appServer_tablename);

        public:
            QDateTime getFechaRegistro(const long long &_id);
            bool guardarRegistroRegistroMatriculas(Registros::RegistroMatriculas &RegistroMatriculas);
            bool eliminaFotosCamara(const long long &_id);
            bool buscarPareja(QVector<Registros::RegistroMatriculas> &RegistrosMatriculas, const QString &_matricula);
            bool actualizarPareja(QVector<Registros::RegistroMatriculas> &RegistrosMatriculas);


        signals:
            void rangoFechasChanged(const QVector<QDate> &_fechaMinMax);
        public slots:
            QVector<QDate> getRangoFechas() const{return m_fecha_min_max;}


        };
    } /** End namespace Maestros **/
} /** End namespace nQTrucks **/

#endif // REGISTROPESO_H
