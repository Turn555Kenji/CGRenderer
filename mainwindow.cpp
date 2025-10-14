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
    // Esta chamada agora também cria o objeto "Window"
    ui->drawArea->setupCoordinates();
    ui->drawArea->setAutoFillBackground(true);
    ui->drawArea->setPalette(pal);
    ui->drawArea->show();

    ui->vp_xmax->setValue(ui->drawArea->width() - 1);
    ui->vp_ymax->setValue(ui->drawArea->height() - 1);

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

    // Adiciona manualmente o objeto Window à tabela da UI
    Obj* windowObj = ui->drawArea->getObject(-1);
    if (windowObj) {
        on_objectAdded(windowObj->getName(), windowObj->getId(), windowObj->getType());
    }

    connect(ui->drawArea, &PainterWidget::mouseClick, this, &MainWindow::on_PainterMouseClicked);
    connect(ui->drawArea, &PainterWidget::objectAdded, this, &MainWindow::on_objectAdded);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::configureButtons(bool linB, bool poiB, bool polB){ //Could just set to reset later
    ui->lineButton->setDisabled(linB);
    ui->pointButton->setDisabled(poiB);
    ui->polygonButton->setDisabled(polB);
}

void MainWindow::finishObject(){
    ui->drawArea->endNewObject();
    configureButtons(false, false, false);

    ui->statusbar->showMessage("");
}

void MainWindow::on_PainterMouseClicked(int x, int y)//Called when mouse is left clicked, use x and y for implementation
{

    ui->X1Label->setText(QString::number(x));
    ui->Y1Label->setText(QString::number(y));

    // Calcula as coordenadas NDC [-1, 1] para exibição na UI
    double ndcX = -1.0 + 2.0 * (x - ui->vp_xmin->value()) / (ui->vp_xmax->value() - ui->vp_xmin->value());
    double ndcY = 1.0 - 2.0 * (y - ui->vp_ymin->value()) / (ui->vp_ymax->value() - ui->vp_ymin->value()); // Eixo Y invertido

    ui->X1Label_Normalized->setText(QString::number(ndcX, 'f', 2)); // Exibe como float com 2 casas decimais
    ui->Y1Label_Normalized->setText(QString::number(ndcY, 'f', 2));
}

void MainWindow::on_pointButton_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Add New Point", "Object Name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty())
        return;

    int xp = ( ui->drawArea->getXwmax() + ui->drawArea->getXwmin()) / 2 ;
    int yp = ( ui->drawArea->getYwmax() + ui->drawArea->getYwmin()) / 2 ;

    ui->drawArea->addPointToCurrentObject(xp, yp, name);
    finishObject();
}


void MainWindow::on_lineButton_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Add New Line", "Object Name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty())
        return;

    int xp = ( ui->drawArea->getXwmax() + ui->drawArea->getXwmin()) / 2 ;
    int yp = ( ui->drawArea->getYwmax() + ui->drawArea->getYwmin()) / 2 ;

    Point *p1 = new Point(xp - 50, yp);
    Point *p2 = new Point(xp + 50, yp);
    ui->drawArea->addLineToCurrentObject(p1, p2, name);
    finishObject();
}


void MainWindow::on_polygonButton_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Add New Polygon", "Object Name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty())
        return;

    int sides = ui->polygonSides->value();
    double step = 360.0 / sides;
    double radius = 50.0;

    int xp = ( ui->drawArea->getXwmax() + ui->drawArea->getXwmin()) / 2 ;
    int yp = ( ui->drawArea->getYwmax() + ui->drawArea->getYwmin()) / 2 ;

    Point* first = nullptr;
    Point* previous = nullptr;

    for (int i = 0; i < sides; ++i) {
        double angle = i * step * M_PI / 180.0;
        int x = static_cast<int>(xp + radius * std::cos(angle));
        int y = static_cast<int>(yp + radius * std::sin(angle));

        Point* current = new Point(x, y);

        if (previous) {
            ui->drawArea->addVertexToCurrentObject(previous, current, name);
        } else {
            first = current;
        }

        previous = current;
    }

    if (first && previous)
        ui->drawArea->addVertexToCurrentObject(previous, first, name);

    finishObject();
}

void MainWindow::drawCustomShape(QList<Obj*> pointList, QString name)
{
    for (int i = 0; i < pointList.size() - 1; ++i) {
        ui->drawArea->addVertexToCurrentObject(
            static_cast<Point*>(pointList[i]),
            static_cast<Point*>(pointList[i + 1]),
            name
            );
    }

    ui->drawArea->addVertexToCurrentObject(
        static_cast<Point*>(pointList.last()),
        static_cast<Point*>(pointList.first()),
        name
        );

    finishObject();
}

void MainWindow::on_treeButton_clicked()
{
    int xp = ( ui->drawArea->getXwmax() + ui->drawArea->getXwmin()) / 2 ;
    int yp = ( ui->drawArea->getYwmax() + ui->drawArea->getYwmin()) / 2 ;

    QList<Obj*> pointList;

    pointList   << new Point(xp - 25, yp - 40)
                << new Point(xp - 25, yp)
                << new Point(xp - 100, yp)
                << new Point(xp - 40, yp + 80)
                << new Point(xp - 90, yp + 80)
                << new Point(xp, yp + 200)
                << new Point(xp + 90, yp + 80)
                << new Point(xp + 40, yp + 80)
                << new Point(xp + 100, yp)
                << new Point(xp + 25, yp)
                << new Point(xp + 25, yp - 40)
        ;

    drawCustomShape(pointList, "Tree");
}


void MainWindow::on_houseButton_clicked()
{
    int xp = ( ui->drawArea->getXwmax() + ui->drawArea->getXwmin()) / 2 ;
    int yp = ( ui->drawArea->getYwmax() + ui->drawArea->getYwmin()) / 2 ;

    QList<Obj*> houseList;

    //House Shape
    houseList   << new Point(xp - 100, yp - 40)
                << new Point(xp - 100, yp + 50)
                << new Point(xp - 120, yp + 50)
                << new Point(xp, yp + 120)
                << new Point(xp + 120, yp + 50)
                << new Point(xp + 100, yp + 50)
                << new Point(xp + 100, yp - 40)
    ;
    drawCustomShape(houseList, "House structure");

    QList<Obj*> doorList;
    //Door Shape
    doorList    << new Point(xp - 20, yp - 40)
                << new Point(xp - 20, yp + 40)
                << new Point(xp + 20, yp + 40)
                << new Point(xp + 20, yp - 40)
    ;
    drawCustomShape(doorList, "House door");

    QList<Obj*> windowList;
    //Window Shape
    windowList  << new Point(xp - 40, yp)
                << new Point(xp - 40, yp + 40)
                << new Point(xp - 85, yp + 40)
                << new Point(xp - 85, yp)
        ;
    drawCustomShape(windowList, "House window");
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

    // Impede a exclusão do objeto Window
    if(idItem->text().toInt() == -1){
        statusBar()->showMessage("The Window object cannot be deleted.");
        return;
    }

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

Obj *MainWindow::getTableObject(){
    QTableWidgetItem *selectedItem = ui->objectTableWidget->currentItem();
    if (!selectedItem) {
        return nullptr;
    }
    int row = selectedItem->row();
    QTableWidgetItem *idItem = ui->objectTableWidget->item(row, 0);

    if (idItem) {
        Obj *target = ui->drawArea->getObject(idItem->text().toInt());
        if (target)
            return target;
        return nullptr;
    }
    return nullptr;
}

void MainWindow::on_translateButton_clicked()
{
    Obj *target = getTableObject();
    if(!target)
        return;

    if (target->getId() == -1) { // Lógica de PAN para o objeto Window
        int dx = ui->translateXvalue->value();
        int dy = ui->translateYvalue->value();

        double current_xwmin = ui->drawArea->getXwmin();
        double current_ywmin = ui->drawArea->getYwmin();
        double current_xwmax = ui->drawArea->getXwmax();
        double current_ywmax = ui->drawArea->getYwmax();

        ui->drawArea->setWindow(current_xwmax + dx, current_xwmin + dx, current_ywmax + dy, current_ywmin + dy);

    } else { // Lógica normal para outros objetos
        MatrixMath::translateObject(target, ui->translateXvalue->value(), ui->translateYvalue->value());
        ui->drawArea->update();
    }
}


void MainWindow::on_rotateButton_clicked()
{
    Obj *target = getTableObject();
    if(!target)
        return;

    int angle = ui->rotateValue->value();
    int xpivot = ui->rotateXValue->value();
    int ypivot = ui->rotateYValue->value();

    if (target->getId() == -1) {
        ui->drawArea->rotateScene(-angle, xpivot, ypivot);
        statusBar()->showMessage("Scene rotated around pivot.");
    } else {
        MatrixMath::rotateObject(target, angle, xpivot, ypivot);
        ui->drawArea->update();
    }
}


void MainWindow::on_scaleButton_clicked()
{
    Obj *target = getTableObject();
    if(!target)
        return;

    if (target->getId() == -1) { // Lógica de ZOOM para o objeto Window
        double sx = ui->xFactorValue->value();
        double sy = ui->yFactorValue->value();

        if (sx == 0) sx = 1; // Evita escala por zero
        if (sy == 0) sy = 1;

        double xwmin = ui->drawArea->getXwmin();
        double ywmin = ui->drawArea->getYwmin();
        double xwmax = ui->drawArea->getXwmax();
        double ywmax = ui->drawArea->getYwmax();

        double cx = (xwmin + xwmax) / 2.0;
        double cy = (ywmin + ywmax) / 2.0;
        double width = xwmax - xwmin;
        double height = ywmax - ywmin;

        double new_width = width * sx; // Divide para efeito de zoom
        double new_height = height * sy;

        int new_xwmax = static_cast<int>(cx + new_width / 2.0);
        int new_xwmin = static_cast<int>(cx - new_width / 2.0);
        int new_ywmax = static_cast<int>(cy + new_height / 2.0);
        int new_ywmin = static_cast<int>(cy - new_height / 2.0);

        ui->drawArea->setWindow(new_xwmax, new_xwmin, new_ywmax, new_ywmin);

    } else { // Lógica normal para outros objetos
        MatrixMath::scaleObject(target, ui->xFactorValue->value(), ui->yFactorValue->value());
        ui->drawArea->update();
    }
}


void MainWindow::on_applyVpButton_clicked()
{
    ui->drawArea->setViewPort(ui->vp_xmax->value(), ui->vp_xmin->value(), ui->vp_ymax->value(), ui->vp_ymin->value());
}


void MainWindow::on_resetButton_clicked()
{
    ui->drawArea->resetWindowViewPort();

    ui->vp_xmax->setValue(ui->drawArea->width() - 1);
    ui->vp_ymax->setValue(ui->drawArea->height() - 1);

    ui->vp_xmin->setValue(0);
    ui->vp_ymin->setValue(0);
}
