#ifndef ICHART_H
#define ICHART_H

#include <QtCharts/QChart>
#include <QVector>
#include "GraphPoint.h"

QT_CHARTS_USE_NAMESPACE

    class IChart
{
public:
    virtual ~IChart() = default;
    virtual QChart* build(const QVector<GraphPoint>& data) = 0;
};
#endif // ICHART_H
