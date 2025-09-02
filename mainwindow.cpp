#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <cmath>

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

    ui->lineButton->setDisabled(true);
    ui->pointButton->setDisabled(true);
    ui->polygonButton->setDisabled(true);
    ui->openPolygonButton->setDisabled(true);

    connect(ui->drawArea, &PainterWidget::mouseClick, this, &MainWindow::on_PainterMouseClicked);
    connect(ui->drawArea, &PainterWidget::objectAdded, this, &MainWindow::on_objectAdded);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_drawLineButton_clicked()
{
    /*float X1 = ui->X1Value->value();
    float Y1 = ui->Y1Value->value();
    float X2 = ui->X2Value->value();
    float Y2 = ui->Y2Value->value();

    QPoint Lp1(X1, Y1);
    QPoint Lp2(X2, Y2);
    QLine line1(Lp1, Lp2);

    ui->drawArea->addLine(line1);*/
}

void MainWindow::on_drawPointButton_clicked()
{
    /*float X1 = ui->X1Value->value();
    float Y1 = ui->Y1Value->value();

    QPoint p1(X1, Y1);

    ui->drawArea->addPoint(p1);*/
}

void MainWindow::on_clearButton_clicked()
{
    //ui->drawArea->clearShapes();
}


//Criado apenas para testar funcionalidades do mouse! Nada concreto!

bool i = false;
QPoint previous;
QPoint first;
unsigned char option = 0;

int pointDistance(QPoint next, QPoint first){
    return abs(next.x() - first.x()) + abs(next.y() - first.y());
}
void MainWindow::on_PainterMouseClicked(int x, int y)//Called when mouse is left clicked, use x and y for implementation
{

    ui->X1Label->setText(QString::number(x));
    ui->Y1Label->setText(QString::number(y));

    switch (option){

        case 0:{    //Drawing point
            QPoint p(x, y);
            ui->drawArea->addPointToCurrentObject(p);
            break;
        }

        case 1:{    //Drawing line
            if(i == true){
                QPoint next(x, y);
                QLine line(previous, next);
                ui->drawArea->addLineToCurrentObject(line);
                previous = next;
                i = false;
            }
            else{
                previous = QPoint(x, y);
                i = true;
            }
            break;
        }

        case 2:{    //Drawing polygon
            if(i == true){
                QPoint next(x, y);
                if(pointDistance(next, first) < 10){
                    QLine line(previous, first);
                    ui->drawArea->addLineToCurrentObject(line);
                    i = false;
                    break;
                }
                QLine line(previous, next);
                ui->drawArea->addLineToCurrentObject(line);
                previous = next;
            }
            else{
                previous = QPoint(x, y);
                first = previous;
                i = true;
            }
            break;
        }
    }
}

/*int pointDistance(QPoint next, QPoint first){
    return abs(next.x() - first.x()) + abs(next.y() - first.y());
}*/

void MainWindow::on_newObjectButton_clicked()
{
    i = false;
    option = 0;
    bool ok;

    ui->lineButton->setDisabled(false);
    ui->pointButton->setDisabled(true);
    ui->polygonButton->setDisabled(true);

    QString name = QInputDialog::getText(this, "Add New Object", "Object Name:", QLineEdit::Normal, "", &ok);

    if (ok && !name.isEmpty()) {
        ui->drawArea->beginNewObject(name);
        // Optional: Update status bar to guide the user
        statusBar()->showMessage("Drawing new object: '" + name + "'. Click 'Finish Object' when done.");
    }
}

void MainWindow::on_finishButton_clicked()
{
    ui->drawArea->endNewObject();
    i = false;
    option = 0;
    ui->lineButton->setDisabled(true);
    ui->pointButton->setDisabled(true);
    ui->polygonButton->setDisabled(true);

    statusBar()->showMessage("");
}

void MainWindow::on_pointButton_clicked()
{
    option = 0;
    ui->lineButton->setDisabled(false);
    ui->pointButton->setDisabled(true);
    ui->polygonButton->setDisabled(false);
}


void MainWindow::on_lineButton_clicked()
{   option = 1;
    ui->lineButton->setDisabled(true);
    ui->pointButton->setDisabled(false);
    ui->polygonButton->setDisabled(false);
}


void MainWindow::on_polygonButton_clicked()
{
    option = 2;
    ui->lineButton->setDisabled(false);
    ui->pointButton->setDisabled(false);
    ui->polygonButton->setDisabled(true);
    ui->openPolygonButton->setDisabled(false);
}

void MainWindow::on_objectAdded(const QString &name, int id)
{
    QString itemText = QString("%1 (ID: %2)").arg(name).arg(id);
    ui->objectListWidget->addItem(itemText);
}


void MainWindow::on_openPolygonButton_clicked()
{
    i = false;
}

