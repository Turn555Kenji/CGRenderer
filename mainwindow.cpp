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

    ui->lineButton->setDisabled(false);
    ui->pointButton->setDisabled(false);
    ui->polygonButton->setDisabled(false);
    ui->clearLastPositionButton->setDisabled(true);
    ui->deleteObjectButton->setDisabled(true);

    ui->objectTableWidget->setColumnCount(4);
    ui->objectTableWidget->setHorizontalHeaderLabels({"ID", "Name", "Points", "Lines"});
    ui->objectTableWidget->setColumnWidth(0, 30);
    ui->objectTableWidget->setColumnWidth(1, 165);
    ui->objectTableWidget->setColumnWidth(2, 45);
    ui->objectTableWidget->setColumnWidth(3, 45);
    ui->objectTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->drawArea, &PainterWidget::mouseClick, this, &MainWindow::on_PainterMouseClicked);
    connect(ui->drawArea, &PainterWidget::objectAdded, this, &MainWindow::on_objectAdded);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool i = false;
QPoint previous;
QPoint first;
unsigned char option = 0;

int pointDistance(QPoint next, QPoint first)
{
    return abs(next.x() - first.x()) + abs(next.y() - first.y());
}

void finishObject(Ui::MainWindow *ui){
    ui->drawArea->endNewObject();
    i = false;
    option = 0;
    ui->lineButton->setDisabled(false);
    ui->pointButton->setDisabled(false);
    ui->polygonButton->setDisabled(false);
    ui->clearLastPositionButton->setDisabled(true);

    //statusBar()->showMessage("");
}

void MainWindow::on_PainterMouseClicked(int x, int y)//Called when mouse is left clicked, use x and y for implementation
{

    ui->X1Label->setText(QString::number(x));
    ui->Y1Label->setText(QString::number(y));

    switch (option){

        case 0:{    //Drawing point
            QPoint p(x, y);
            ui->drawArea->addPointToCurrentObject(p);
            finishObject(ui);
            break;
        }

        case 1:{    //Drawing line
            if(i == true){
                QPoint next(x, y);
                QLine line(previous, next);
                ui->drawArea->addLineToCurrentObject(line);
                previous = next;
                i = false;
                finishObject(ui);
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
                if(pointDistance(next, first) < 30){
                    QLine line(previous, first);
                    ui->drawArea->addLineToCurrentObject(line);
                    i = false;
                    finishObject(ui);
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

void MainWindow::on_newObjectButton_clicked()
{
    i = false;
    option = 0;
    bool ok;

    ui->lineButton->setDisabled(false);
    ui->pointButton->setDisabled(true);
    ui->polygonButton->setDisabled(false);
    ui->clearLastPositionButton->setDisabled(true);

    QString name = QInputDialog::getText(this, "Add New Object", "Object Name:", QLineEdit::Normal, "", &ok);

    if (ok && !name.isEmpty()) {
        ui->drawArea->beginNewObject(name);
        statusBar()->showMessage("Drawing new object: '" + name + "'. Click 'Finish Object' when done.");
    }
}

void MainWindow::on_finishButton_clicked()
{
    ui->drawArea->endNewObject();
    i = false;
    option = 0;
    ui->lineButton->setDisabled(false);
    ui->pointButton->setDisabled(false);
    ui->polygonButton->setDisabled(false);
    ui->clearLastPositionButton->setDisabled(true);

    statusBar()->showMessage("");
}

void MainWindow::on_pointButton_clicked()
{
    option = 0;
    bool ok;
    ui->lineButton->setDisabled(true);
    ui->pointButton->setDisabled(true);
    ui->polygonButton->setDisabled(true);
    ui->clearLastPositionButton->setDisabled(true);

    QString name = QInputDialog::getText(this, "Add New Point", "Object Name:", QLineEdit::Normal, "", &ok);

    if (ok && !name.isEmpty()) {
        ui->drawArea->beginNewObject(name);
        statusBar()->showMessage("Drawing new point: '" + name + "'. Click 'Finish Object' when done.");
    }
}


void MainWindow::on_lineButton_clicked()
{
    option = 1;
    bool ok;
    i = false;
    ui->lineButton->setDisabled(true);
    ui->pointButton->setDisabled(true);
    ui->polygonButton->setDisabled(true);
    ui->clearLastPositionButton->setDisabled(true);

    QString name = QInputDialog::getText(this, "Add New Point", "Object Name:", QLineEdit::Normal, "", &ok);

    if (ok && !name.isEmpty()) {
        ui->drawArea->beginNewObject(name);
        statusBar()->showMessage("Drawing new point: '" + name + "'. Click 'Finish Object' when done.");
    }
}


void MainWindow::on_polygonButton_clicked()
{
    option = 2;
    bool ok;
    i = false;
    ui->lineButton->setDisabled(true);
    ui->pointButton->setDisabled(true);
    ui->polygonButton->setDisabled(true);
    ui->clearLastPositionButton->setDisabled(true);

    QString name = QInputDialog::getText(this, "Add New Point", "Object Name:", QLineEdit::Normal, "", &ok);

    if (ok && !name.isEmpty()) {
        ui->drawArea->beginNewObject(name);
        statusBar()->showMessage("Drawing new point: '" + name + "'. Click 'Finish Object' when done.");
    }
}

void MainWindow::on_objectAdded(const QString &name, int id, int pointNum, int lineNum)
{
    int row = ui->objectTableWidget->rowCount();
    ui->objectTableWidget->insertRow(row);
    ui->objectTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
    ui->objectTableWidget->setItem(row, 1, new QTableWidgetItem(name));
    ui->objectTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(pointNum)));
    ui->objectTableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(lineNum)));
}

void MainWindow::on_clearLastPositionButton_clicked()
{
    i = false;
}

void MainWindow::on_deleteObjectButton_clicked()
{
    QTableWidgetItem *selectedItem = ui->objectTableWidget->currentItem();
    if (!selectedItem) {
        return;
    }
    int row = selectedItem->row();
    QTableWidgetItem *idItem = ui->objectTableWidget->item(row, 0);

    if (idItem) {
        int objectId = idItem->text().toInt();
        ui->drawArea->removeObject(objectId);
        ui->objectTableWidget->removeRow(row);
    }
}


void MainWindow::on_objectTableWidget_itemClicked()
{
    ui->deleteObjectButton->setDisabled(false);
}

