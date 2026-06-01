#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QWidget>
#include <QtCharts>

QT_CHARTS_USE_NAMESPACE

    class ThemeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ThemeWidget(QWidget *parent = nullptr);

    void setChart(QChart* chart);
    void setGrayScale(bool enabled);

    QChartView* getChartView();

private:
    QChartView* chartView;
    bool isGrayScale = false;
};

#endif
