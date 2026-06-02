#ifndef LINECHART_H
#define LINECHART_H

#include "IChart.h"
#include <QtCharts>

QT_CHARTS_USE_NAMESPACE

    class LineChart : public IChart
{
public:
    QChart* build(
        const QVector<GraphPoint>& data) override
    {
        QChart* chart = new QChart();
        chart->setTitle("Line chart");

        QLineSeries* series = new QLineSeries();

        for (const auto& p : data)
        {
            series->append(p.date.toMSecsSinceEpoch(),p.value);
        }

        chart->addSeries(series);

        QDateTimeAxis* axisX = new QDateTimeAxis();
        axisX->setFormat("dd.MM");
        axisX->setTitleText("Дата");

        QValueAxis* axisY = new QValueAxis();
        axisY->setTitleText("Значение");

        chart->addAxis(axisX, Qt::AlignBottom);
        chart->addAxis(axisY, Qt::AlignLeft);

        series->attachAxis(axisX);
        series->attachAxis(axisY);

        return chart;
    }
};

#endif
