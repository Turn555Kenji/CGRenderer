#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <cmath>

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
QPoint previous;
QPoint first;
unsigned char option = 0;

int pointDistance(QPoint next, QPoint first)
{
    return abs(next.x() - first.x()) + abs(next.y() - first.y());
}

void MainWindow::configureButtons(bool linB, bool poiB, bool polB){ //Could just set to reset later
    ui->lineButton->setDisabled(linB);
    ui->pointButton->setDisabled(poiB);
    ui->polygonButton->setDisabled(polB);
}

void MainWindow::finishObject(){
    ui->drawArea->endNewObject();
    i = false;
    option = 0;
    configureButtons(false, false, false);

    ui->statusbar->showMessage("");
}

void MainWindow::on_PainterMouseClicked(int x, int y)//Called when mouse is left clicked, use x and y for implementation
{

    ui->X1Label->setText(QString::number(x));
    ui->Y1Label->setText(QString::number(y));

    switch (option){

        case 0:{    //Drawing point
            QPoint p(x, y);
            ui->drawArea->addPointToCurrentObject(p);
            finishObject();
            break;
        }

        case 1:{    //Drawing line
            if(i == true){
                QPoint next(x, y);
                QLine line(previous, next);
                ui->drawArea->addLineToCurrentObject(line);
                previous = next;
                i = false;
                finishObject();
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
                    finishObject();
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

void MainWindow::on_pointButton_clicked()
{
    option = 0;
    bool ok;
    configureButtons(true, true, true);

    QString name = QInputDialog::getText(this, "Add New Point", "Object Name:", QLineEdit::Normal, "", &ok);

    if (ok && !name.isEmpty()) {
        ui->drawArea->beginNewObject(name, "Point");
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

    if (ok && !name.isEmpty()) {
        ui->drawArea->beginNewObject(name, "Line");
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

    if (ok && !name.isEmpty()) {
        ui->drawArea->beginNewObject(name, "Polygon");
        statusBar()->showMessage("Drawing new polygon: '" + name + "'. Click 'Finish Object' when done.");
    }
}

void MainWindow::on_objectAdded(const QString &name, const QString &type, int id)
{
    int row = ui->objectTableWidget->rowCount();
    ui->objectTableWidget->insertRow(row);
    ui->objectTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
    ui->objectTableWidget->setItem(row, 1, new QTableWidgetItem(name));
    ui->objectTableWidget->setItem(row, 2, new QTableWidgetItem(type));
}

/*void MainWindow::on_deleteObjectButton_clicked()
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
        SceneObject *obj = ui->drawArea->getObject(objectId);
        QString objType = obj->type();
        if(objType == "Point"){
            const QPoint& pt = obj->points()[0];
            QString pointStr = QString("(%1, %2)").arg(pt.x()).arg(pt.y());
            ui->pointListWidget->addItem(pointStr);
        } else if (objType == "Line"){
            const QLine& pt = obj->lines()[0];
            QString lineStr = QString("(%1, %2)\n(%3, %4)").arg(pt.p1().x()).arg(pt.p1().y()).arg(pt.p2().x()).arg(pt.p2().y());
            ui->pointListWidget->addItem(lineStr);
        } else if (objType == "Polygon"){
            for (const QLine& pt : obj->lines()) {
                QString polygonStr = QString("(%1, %2)").arg(pt.p1().x()).arg(pt.p1().y());
                ui->pointListWidget->addItem(polygonStr);
            }
        }
    }
}*/

