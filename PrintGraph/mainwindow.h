#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QSplitter>
#include "ThemeWidget.h"
#include "IDataProvider.h"
#include "GraphModel.h"
#include "IChart.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);
    void onFileSelected(const QModelIndex& index);
    void chartTypeChanged(int index);
    void grayScaleChanged(bool checked);
    void exportPdf();

private:
    void loadDataFromFile(const QString& filePath);
    void updateChart();


    Ui::MainWindow *ui;

    ThemeWidget *themeWidget;

    QFileSystemModel *rightPartModel;
    QFileSystemModel *leftPartModel;
    QTreeView *treeView;
    QTableView *tableView;

    QComboBox* chartCombo;
    QCheckBox* grayCheckBox;
    QPushButton* printButton;
    QPushButton* openButton;
    std::shared_ptr<IDataProvider> provider;
    GraphModel graphModel;
    std::shared_ptr<IChart> currentChartType;
};

#endif
