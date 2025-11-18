#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actOpen,    &QAction::triggered, this, &MainWindow::open);
    connect(ui->actSave,    &QAction::triggered, this, &MainWindow::save);
    connect(ui->actSaveAs,    &QAction::triggered, this, &MainWindow::save_as);
    connect(ui->actExit,    &QAction::triggered, this, &MainWindow::exit);
    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &MainWindow::cellEdit);
    connect(ui->add_row,    &QAction::triggered, this, &MainWindow::addRow);
    connect(ui->add_column,    &QAction::triggered, this, &MainWindow::addColumn);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addRow()
{
    if (parser){
        parser->NewRow();
        ui->tableWidget->setRowCount(parser->rowCount());
        ui->statusBar->showMessage(QString("Добавленна строка"));
    }else{
        QMessageBox::warning(this, "INVALID OPERATION", "Open a file");
    }
}

void MainWindow::addColumn()
{
     if (parser){
        parser->NewColumn();
        ui->tableWidget->setColumnCount(parser->columnCount());
        ui->statusBar->showMessage(QString("Добавлен столбец"));
     }else{
         QMessageBox::warning(this, "INVALID OPERATION", "Open a file");
     }
}

void MainWindow::save()
{
    if (parser){
        auto path = parser->getFileName();
        parser->save(path);
        ui->statusBar->showMessage(QString("Файл сохранен"));
    }else{
        QMessageBox::warning(this, "INVALID OPERATION", "Open a file");
    }

}

void MainWindow::save_as()
{
    if (parser){
        auto path = QFileDialog::getOpenFileName(this);
        ui->statusBar->showMessage(QString("Файл сохранен в ") + path);
        parser->save(path.toStdString());
    }else{
        QMessageBox::warning(this, "INVALID OPERATION", "Open a file");
    }
}

void MainWindow::exit()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit", "Whant to save?",
                                    QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        save();
        close();
    }else{
        close();
    }
}

void MainWindow::open()
{
    if (parser) {
        QMessageBox::StandardButton reply;
          reply = QMessageBox::question(this, "Open", "Whant to save?",
                                        QMessageBox::Yes|QMessageBox::No);
          if (reply == QMessageBox::Yes) {
              save();
              auto path = QFileDialog::getOpenFileName(this);
              if (path.isEmpty()) {
                  return;
              }

              ui->statusBar->showMessage(QString("Открыт файл ") + path);

              parser = new CSVFile(path.toStdString());

              ui->tableWidget->setRowCount(parser->rowCount());
              ui->tableWidget->setColumnCount(parser->columnCount());

              for (size_t row = 0; row < parser->rowCount(); row++) {
                  Row rowVal = parser->getRow(row);
                  for (size_t col = 0; col < rowVal.size(); col++) {
                      QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(rowVal[col]));
                      ui->tableWidget->setItem(row, col, newItem);
                  }
              }
          } else if(reply == QMessageBox::No) {
              auto path = QFileDialog::getOpenFileName(this);
              if (path.isEmpty()) {
                  return;
              }

              ui->statusBar->showMessage(QString("Открыт файл ") + path);

              parser = new CSVFile(path.toStdString());

              ui->tableWidget->setRowCount(parser->rowCount());
              ui->tableWidget->setColumnCount(parser->columnCount());

              for (size_t row = 0; row < parser->rowCount(); row++) {
                  Row rowVal = parser->getRow(row);
                  for (size_t col = 0; col < rowVal.size(); col++) {
                      QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(rowVal[col]));
                      ui->tableWidget->setItem(row, col, newItem);
                  }
              }
          }

    }else {
        auto path = QFileDialog::getOpenFileName(this);
        if (path.isEmpty()) {
            return;
        }

        ui->statusBar->showMessage(QString("Открыт файл ") + path);

        parser = new CSVFile(path.toStdString());

        ui->tableWidget->setRowCount(parser->rowCount());
        ui->tableWidget->setColumnCount(parser->columnCount());

        for (size_t row = 0; row < parser->rowCount(); row++) {
            Row rowVal = parser->getRow(row);
            for (size_t col = 0; col < rowVal.size(); col++) {
                QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(rowVal[col]));
                ui->tableWidget->setItem(row, col, newItem);
            }
        }
    }
}


void MainWindow::cellEdit(QTableWidgetItem *item)
{
    parser->setValue(item->row(), item->column(), item->text().toStdString());
}
