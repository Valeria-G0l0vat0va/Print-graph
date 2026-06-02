#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "IOC.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QStatusBar>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QDebug>
#include "BarChart.h"
#include "LineChart.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    provider = gContainer.GetObject<IDataProvider>();

    themeWidget = new ThemeWidget();

    chartCombo = new QComboBox();
    chartCombo->addItem("BarChart");
    chartCombo->addItem("LineChart");

    grayCheckBox = new QCheckBox("Черно-белый график");
    printButton = new QPushButton("Печать графика");

    connect(chartCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::chartTypeChanged);
    connect(grayCheckBox, &QCheckBox::toggled,
            this, &MainWindow::grayScaleChanged);
    connect(printButton, &QPushButton::clicked,
            this, &MainWindow::exportPdf);

    this->setGeometry(100, 100, 1500, 500);
    this->setStatusBar(new QStatusBar(this));
    this->statusBar()->showMessage("Choosen Path: ");

    QString homePath = QDir::homePath();

    leftPartModel = new QFileSystemModel(this);
    leftPartModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    leftPartModel->setRootPath(homePath);

    rightPartModel = new QFileSystemModel(this);
    rightPartModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    rightPartModel->setRootPath(homePath);

    treeView = new QTreeView();
    treeView->setModel(leftPartModel);
    tableView = new QTableView();
    tableView->setModel(rightPartModel);
    QSplitter *splitter = new QSplitter(parent);
    splitter->addWidget(treeView);
    splitter->addWidget(tableView);
    QVBoxLayout* rightLayout = new QVBoxLayout();
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(new QLabel("Выберите тип диаграммы"));
    topLayout->addWidget(chartCombo);
    topLayout->addWidget(grayCheckBox);
    topLayout->addWidget(printButton);
    rightLayout->addLayout(topLayout);
    rightLayout->addWidget(themeWidget);
    QWidget* rightWidget = new QWidget();
    rightWidget->setLayout(rightLayout);
    splitter->addWidget(rightWidget);
    setCentralWidget(splitter);
    statusBar()->showMessage("Выберите SQLite файл");
    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::on_selectionChangedSlot);
    connect(tableView, &QTableView::doubleClicked,
            this, &MainWindow::onFileSelected);
    currentChartType =std::make_shared<BarChart>();
}

void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    QModelIndexList indexes = selected.indexes();
    if(indexes.isEmpty()) return;

    QModelIndex ix = indexes.constFirst();
    QString folderPath = leftPartModel->filePath(ix);

    tableView->setRootIndex(rightPartModel->setRootPath(folderPath));
    statusBar()->showMessage("Папка: " + folderPath);
}

void MainWindow::onFileSelected(const QModelIndex& index)
{
    QString filePath = rightPartModel->filePath(index);

    if(!filePath.endsWith(".sqlite"))
    {
        statusBar()->showMessage("Выбран не SQLite файл");
        return;
    }

    loadDataFromFile(filePath);
}

void MainWindow::loadDataFromFile(const QString& filePath)
{
    if(!provider)
        return;

    QVector<GraphPoint> data =
        provider->loadData(filePath);

    graphModel.setData(data);

    if(!graphModel.isEmpty())
    {
        updateChart();
        statusBar()->showMessage("Загружено " + QString::number(graphModel.data().size()) + " точек");
    }
    else
    {
        statusBar()->showMessage("Нет данных в файле");
    }
}

void MainWindow::updateChart()
{
    if(graphModel.isEmpty())
        return;

    if(!currentChartType)
        return;

    themeWidget->setGrayScale(grayCheckBox->isChecked());
    QChart* chart =currentChartType->build(graphModel.data());
    themeWidget->setChart(chart);
}

void MainWindow::chartTypeChanged(int index)
{
    switch(index)
    {
    case 0:
        currentChartType =std::make_shared<BarChart>();
        break;
    case 1:
        currentChartType =std::make_shared<LineChart>();
        break;
    }
    updateChart();
}

void MainWindow::grayScaleChanged(bool checked)
{
    Q_UNUSED(checked);
    updateChart();
}

void MainWindow::exportPdf()
{
    if(graphModel.isEmpty())
    {
        statusBar()->showMessage("Нет данных для экспорта");
        return;
    }

    QString fileName =
        QFileDialog::getSaveFileName(this,"Save PDF","","*.pdf");

    if(fileName.isEmpty())
        return;

    QPrinter printer;

    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    if(grayCheckBox->isChecked())
    {
        printer.setColorMode(QPrinter::GrayScale);
    }

    QPainter painter(&printer);
    themeWidget->getChartView()->render(&painter);
    painter.end();
    statusBar()->showMessage("PDF сохранен: " + fileName);
}

MainWindow::~MainWindow()
{
    delete ui;
}
