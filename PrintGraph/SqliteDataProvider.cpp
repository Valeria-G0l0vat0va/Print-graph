#include "SqliteDataProvider.h"
#include <QtSql>
#include <QDebug>

QVector<GraphPoint>SqliteDataProvider::loadData(const QString& path)
{
    QVector<GraphPoint> data;
    QString connectionName = QUuid::createUuid().toString();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",connectionName);
    db.setDatabaseName(path);

    if(!db.open())
    {
        qDebug()<< "DB OPEN ERROR:"<< db.lastError().text();
        return data;
    }

    // Получаем список таблиц
    QStringList tables =db.tables();

    if(tables.isEmpty())
    {
        qDebug()<< "No tables found";
        db.close();
        return data;
    }

    // Берем первую таблицу
    QString tableName =tables.first();
    qDebug()<< "TABLE:"<< tableName;
    // Получаем информацию о колонках
    QSqlQuery pragmaQuery(db);
    pragmaQuery.exec("PRAGMA table_info(" + tableName + ")");
    QStringList columns;

    while(pragmaQuery.next())
    {
        columns <<pragmaQuery.value(1).toString();
    }

    if(columns.size() < 2)
    {
        qDebug()<< "Not enough columns";
        db.close();
        return data;
    }

    // Первая колонка = дата
    QString dateColumn =columns[0];
    // Вторая колонка = значение
    QString valueColumn =columns[1];
    qDebug()<< "DATE COLUMN:"<< dateColumn;
    qDebug()<< "VALUE COLUMN:"<< valueColumn;
    // SQL запрос
    QString sql =QString("SELECT %1, %2 FROM %3").arg(dateColumn).arg(valueColumn).arg(tableName);
    QSqlQuery query(db);

    if(!query.exec(sql))
    {
        qDebug()<< "SQL ERROR:"<< query.lastError().text();
        db.close();
        return data;
    }

    while(query.next())
    {
        GraphPoint point;

        QString dateString =query.value(0).toString();
        dateString.remove('Z');
        point.date =QDateTime::fromString(dateString,"dd.MM.yyyy hh:mm");

        if(!point.date.isValid())
        {
            point.date =QDateTime::fromString(dateString,"dd.MM.yyyy hhmm");
        }
        if(!point.date.isValid())
        {
            point.date =QDateTime::fromString(dateString,"dd.MM.yyyy hh:mm:ss");
        }
        if(!point.date.isValid())
        {
            point.date =QDateTime::fromString(dateString,Qt::ISODate);
        }
        if(!point.date.isValid())
        {
            qDebug()<< "Bad date:"<< dateString;
            continue;
        }

        point.value = query.value(1).toDouble();
        qDebug()<< dateString<< point.date.toString("dd.MM.yyyy hh:mm");
        data.push_back(point);
    }

    qDebug()<< "POINTS LOADED:"<< data.size();
    db.close();
    QSqlDatabase::removeDatabase(connectionName);
    return data;
}
