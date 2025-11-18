#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "csvparser.h"
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void open();
    void save();
    void save_as();
    void exit();
    void addColumn();
    void addRow();

    void cellEdit(QTableWidgetItem*);

private:
    Ui::MainWindow *ui;
    CSVFile *parser = nullptr;
};

#endif // MAINWINDOW_H
