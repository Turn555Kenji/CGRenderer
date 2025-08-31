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

    connect(ui->drawArea, &PainterWidget::mouseClick, this, &MainWindow::on_PainterMouseClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_drawLineButton_clicked()
{
    float X1 = ui->X1Value->value();
    float Y1 = ui->Y1Value->value();
    float X2 = ui->X2Value->value();
    float Y2 = ui->Y2Value->value();

    QPoint Lp1(X1, Y1);
    QPoint Lp2(X2, Y2);
    QLine line1(Lp1, Lp2);

    ui->drawArea->addLine(line1);
}

void MainWindow::on_drawPointButton_clicked()
{
    float X1 = ui->X1Value->value();
    float Y1 = ui->Y1Value->value();

    QPoint p1(X1, Y1);

    ui->drawArea->addPoint(p1);
}

void MainWindow::on_clearButton_clicked()
{
    ui->drawArea->clearShapes();
}


//Criado apenas para testar funcionalidades do mouse! Nada concreto!
/*
bool i = false;
QPoint previous;


void MainWindow::on_PainterMouseClicked(int x, int y)
{
    ui->X1Label->setText(QString::number(x));
    ui->Y1Label->setText(QString::number(y));

    if(i == true){
        QPoint next(x, y);
        QLine line(previous, next);
        ui->drawArea->addLine(line);
        previous = next;
    }
    else{
        previous = QPoint(x, y);
        i = true;
    }
}
*/
