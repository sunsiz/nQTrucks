#ifndef QUERYMODEL_H
#define QUERYMODEL_H

#include <QSqlQueryModel>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

#include <QVariant>
#include <QHash>
#include <QBitArray>

#include <QDateTime>

namespace nQTrucks {
    namespace Db {

        class QueryModel : public QSqlQueryModel{
            Q_OBJECT
        public:
            explicit QueryModel(QObject *parent = nullptr);
            ~QueryModel();

            // Reimplementacion de QSqlQueryModel
            bool setCustomView(const QString &query);
        private:
            QHash<int, QByteArray> roleNames() const override;
            QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const override;

        public slots:
            Q_INVOKABLE bool setQuery(const QString &query);//, const QSqlDatabase &_db);

        //DATABASE MANAGER
        protected:
            QSqlDatabase m_db;
            bool initDB();
            bool execQuery(const QByteArray &_query);
        private:
            // verify
            QSqlError lastError();//const QSqlDatabase &db);
        };

    } //name space Db
} //name space nQtrucks

#endif // QUERYMODEL_H
