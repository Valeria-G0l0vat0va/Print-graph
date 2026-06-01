#ifndef IDATAPROVIDER_H
#define IDATAPROVIDER_H
#include <QVector>
#include <QString>
#include "GraphPoint.h"

class IDataProvider
{
public:
    virtual ~IDataProvider() = default;
    virtual QVector<GraphPoint> loadData(const QString& path) = 0;
};
#endif // IDATAPROVIDER_H
