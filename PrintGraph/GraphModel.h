#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H
#include <QVector>
#include "GraphPoint.h"

class GraphModel
{
public:
    void setData(const QVector<GraphPoint>& data)
    {
        m_data = data;
    }

    const QVector<GraphPoint>& data() const
    {
        return m_data;
    }

    bool isEmpty() const
    {
        return m_data.isEmpty();
    }

private:
    QVector<GraphPoint> m_data;
};
#endif // GRAPHMODEL_H
