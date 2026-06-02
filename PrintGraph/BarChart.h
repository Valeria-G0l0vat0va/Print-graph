#ifndef BARCHART_H
#define BARCHART_H

#include "IChart.h"
#include <QtCharts>

QT_CHARTS_USE_NAMESPACE

    class BarChart : public IChart
{
public:
    QChart* build(const QVector<GraphPoint>& data) override
    {
        QChart* chart = new QChart();
        chart->setTitle("Bar chart");

        QBarSeries* series = new QBarSeries();
        QBarSet* set = new QBarSet("Value");
        QStringList categories;

        int count = qMin(data.size(), 100);

        for (int i = 0; i < count; ++i)
        {
            *set << data[i].value;

            categories << data[i].date.toString("dd.MM");
        }

        series->append(set);
        chart->addSeries(series);

        // Ось X (категории)
        QBarCategoryAxis* axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        // Ось Y
        QValueAxis* axisY = new QValueAxis();
        axisY->setTitleText("Value");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        return chart;
    }
};

#endif
