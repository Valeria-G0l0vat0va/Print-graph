#ifndef SQLITEDATAPROVIDER_H
#define SQLITEDATAPROVIDER_H

#include "IDataProvider.h"

class SqliteDataProvider : public IDataProvider
{
public:
    QVector<GraphPoint> loadData(const QString& path) override;
};

#endif // SQLITEDATAPROVIDER_H
