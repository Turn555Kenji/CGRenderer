#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <cmath>
#include <point.h>
#include <line.h>
#include <polygon.h>

/*
Kenji Henrique Ueyama Yashinishi
Luiz Felipe Fuzeto
João Vitor Pereira Cantadori
Marcos Vinicius Santos Passos
*/

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
    ui->deleteObjectButton->setDisabled(true);

    ui->objectTableWidget->setColumnCount(3);
    ui->objectTableWidget->setHorizontalHeaderLabels({"ID", "Name", "Type"});
    ui->objectTableWidget->setColumnWidth(0, 30);
    ui->objectTableWidget->setColumnWidth(1, 180);
    ui->objectTableWidget->setColumnWidth(2, 65);
    ui->objectTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->drawArea, &PainterWidget::mouseClick, this, &MainWindow::on_PainterMouseClicked);
    connect(ui->drawArea, &PainterWidget::objectAdded, this, &MainWindow::on_objectAdded);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool i = false;
Point *previous = nullptr;
Point *first = nullptr;
unsigned char option = 9;
QString lastObj;

int pointDistance(Point next, Point first)
{
    return abs(next[0][0] - first[0][0]) + abs(next[1][0] - first[1][0]);
}

void MainWindow::configureButtons(bool linB, bool poiB, bool polB){ //Could just set to reset later
    ui->lineButton->setDisabled(linB);
    ui->pointButton->setDisabled(poiB);
    ui->polygonButton->setDisabled(polB);
}

void MainWindow::finishObject(){
    ui->drawArea->endNewObject();
    i = false;
    option = 9;
    configureButtons(false, false, false);
    ui->statusbar->showMessage("finished deu certo");

    ui->statusbar->showMessage("");
}

void MainWindow::on_PainterMouseClicked(int x, int y)//Called when mouse is left clicked, use x and y for implementation
{

    ui->X1Label->setText(QString::number(x));
    ui->Y1Label->setText(QString::number(y));

    switch (option){

        case 0:{    //Drawing point
            //remover getters de point se este método funcionar
            statusBar()->showMessage("has enter in case 0");
            ui->drawArea->addPointToCurrentObject(x, y, lastObj);
            finishObject();
            break;
        }

        case 1:{
            if(i == true){
                Point *next = new Point(x, y);
                ui->drawArea->addLineToCurrentObject(previous, next, lastObj);
                previous = next;
                i = false;
                finishObject();
            }
            else{
                previous = new Point(x, y);
                i = true;
            }
            break;
        }

        case 2:{    //Drawing polygon
            if(i == true){
                Point *next = new Point(x, y);
                if(pointDistance(*next, *first) < 30){
                    ui->drawArea->closePolygonObject();
                    //ui->drawArea->addVertexToCurrentObject(previous, first, lastObj);
                    i = false;
                    finishObject();
                    break;
                }
                ui->drawArea->addVertexToCurrentObject(previous, next, lastObj);
                previous = next;
            }
            else{
                previous = new Point(x, y);
                first = previous;
                i = true;
            }
            break;
        }
        //default :
        //break;
    }
}

void MainWindow::on_pointButton_clicked()
{
    option = 0;
    bool ok;
    configureButtons(true, true, true);

    QString name = QInputDialog::getText(this, "Add New Point", "Object Name:", QLineEdit::Normal, "", &ok);
    lastObj=name;
    if (ok && !name.isEmpty()) {
        //ui->drawArea->beginNewObject();
        statusBar()->showMessage("Drawing new point: '" + name + "'. Click 'Finish Object' when done.");
    }
}


void MainWindow::on_lineButton_clicked()
{
    option = 1;
    bool ok;
    i = false;
    configureButtons(true, true, true);

    QString name = QInputDialog::getText(this, "Add New Line", "Object Name:", QLineEdit::Normal, "", &ok);
    lastObj=name;

    if (ok && !name.isEmpty()) {
        statusBar()->showMessage("Drawing new line: '" + name + "'. Click 'Finish Object' when done.");
    }
}


void MainWindow::on_polygonButton_clicked()
{
    option = 2;
    bool ok;
    i = false;
    configureButtons(true, true, true);

    QString name = QInputDialog::getText(this, "Add New Polygon", "Object Name:", QLineEdit::Normal, "", &ok);
    lastObj=name;

    if (ok && !name.isEmpty()) {
        statusBar()->showMessage("Drawing new polygon: '" + name + "'. Click 'Finish Object' when done.");
    }
}


void MainWindow::on_objectAdded(const QString &name, int id, const QString &type)
{
    int row = ui->objectTableWidget->rowCount();
    ui->objectTableWidget->insertRow(row);
    ui->objectTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
    ui->objectTableWidget->setItem(row, 1, new QTableWidgetItem(name));
    ui->objectTableWidget->setItem(row, 2, new QTableWidgetItem(type));
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

void MainWindow::on_objectTableWidget_itemClicked(QTableWidgetItem *item)
{
    ui->pointListWidget->clear();
    if (!item) {
        return;
    }
    int row = item->row();
    QTableWidgetItem *idItem = ui->objectTableWidget->item(row, 0);

    if (idItem) {
        int objectId = idItem->text().toInt();
        Obj *obj = ui->drawArea->getObject(objectId);
        QString objType = obj->getType();
        if(objType == "Point"){
            Point *pt = dynamic_cast<Point*>(obj);
            QString pointStr = QString("(%1, %2)").arg((*pt)[0][0]).arg((*pt)[1][0]);
            ui->pointListWidget->addItem(pointStr);
        } else if (objType == "Line"){
            Line *pt = dynamic_cast<Line*>(obj);
            QString lineStr = QString("(%1, %2)\n(%3, %4)").arg(pt->getP1()[0][0]).arg(pt->getP1()[1][0]).arg(pt->getP2()[0][0]).arg(pt->getP2()[1][0]);
            ui->pointListWidget->addItem(lineStr);
        } else if (objType == "Polygon"){
            Polygon *pt = dynamic_cast<Polygon*>(obj);
            for (const Point& it : pt->getVertices()) {
                QString polygonStr = QString("(%1, %2)").arg(it[0][0]).arg(it[1][0]);
                ui->pointListWidget->addItem(polygonStr);
            }
        }
    }
}

