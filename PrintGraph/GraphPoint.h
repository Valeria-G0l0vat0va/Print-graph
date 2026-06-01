#ifndef GRAPHPOINT_H
#define GRAPHPOINT_H

#include <QDateTime>

struct GraphPoint
{
    QDateTime date;
    double value;

    GraphPoint() : date(QDateTime()), value(0.0) {}
    GraphPoint(QDateTime d, double val) : date(d), value(val) {}
};

#endif // GRAPHPOINT_H
