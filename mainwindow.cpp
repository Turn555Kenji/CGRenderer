#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette pal = ui->drawArea->palette();

    pal.setColor(QPalette::Window, Qt::black);
    ui->drawArea->setAutoFillBackground(true);
    ui->drawArea->setPalette(pal);
    ui->drawArea->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::on_myDrawButton_clicked()
{
    int w = ui->drawArea->width();
    int h = ui->drawArea->height();
    QPoint randPoint(qrand() % w, qrand() % h);
    QLine randLine(QPoint(qrand() % w, qrand() % h), QPoint(qrand() % w, qrand() % h));

    ui->drawArea->addPoint(randPoint);
    ui->drawArea->addLine(randLine);
}

void MainWindow::on_myClearButton_clicked()
{
    ui->drawArea->clearShapes();
}*/

void MainWindow::on_pushButton_clicked()
{
    QPoint p1(50, 50);
    QPoint p2(200, 150);
    QLine line1(p1, p2);

    ui->drawArea->addPoint(QPoint(30, 30));
    ui->drawArea->addLine(line1);
    ui->drawArea->addLine(QLine(50, 200, 250, 20));
}

