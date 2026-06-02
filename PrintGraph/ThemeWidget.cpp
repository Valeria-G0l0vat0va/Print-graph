#include "ThemeWidget.h"
#include <QVBoxLayout>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include "IChart.h"
#include <memory>
#include <QWidget>
#include "GraphPoint.h"

ThemeWidget::ThemeWidget(QWidget *parent)
    : QWidget(parent), isGrayScale(false)
{
    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    QChart* emptyChart = new QChart();
    emptyChart->setTitle("Выберите SQLite файл");
    chartView->setChart(emptyChart);
    QVBoxLayout* layout =new QVBoxLayout(this);
    layout->addWidget(chartView);
    setLayout(layout);
}

void ThemeWidget::setGrayScale(bool enabled)
{
    isGrayScale = enabled;
}
void ThemeWidget::setChart(QChart* chart)
{
    if(isGrayScale)
    {
        chart->setTheme(QChart::ChartThemeHighContrast);
    }
    else
    {
        chart->setTheme(QChart::ChartThemeLight);
    }

    QChart* oldChart =chartView->chart();
    chartView->setChart(chart);
    delete oldChart;
}


QChartView* ThemeWidget::getChartView()
{
    return chartView;
}
