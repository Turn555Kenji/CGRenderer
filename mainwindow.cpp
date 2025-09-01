#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>

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

    connect(ui->drawArea, &PainterWidget::mouseClick, this, &MainWindow::on_PainterMouseClicked);
    connect(ui->drawArea, &PainterWidget::objectAdded, this, &MainWindow::on_objectAdded_to_list);
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
unsigned char option = 0;


void MainWindow::on_PainterMouseClicked(int x, int y)
{
    ui->X1Label->setText(QString::number(x));
    ui->Y1Label->setText(QString::number(y));

    switch (option) {
    case 0: { // Drawing polygon
        if (!m_isDrawingPolygon) {
            m_firstPoint = QPoint(x, y);
            m_previousPoint = m_firstPoint;
            m_isDrawingPolygon = true;
        } else {
            QPoint currentPoint(x, y);
            QLine line(m_previousPoint, currentPoint);
            ui->drawArea->addLineToCurrentObject(line);
            m_previousPoint = currentPoint;
        }
        break;
    }
    case 1: {
        break;
    }
    case 2: {
        break;
    }
    }
}
void MainWindow::on_newObjectButton_clicked()
{
    i = false;
    m_isDrawingPolygon = false;
    bool ok;

    ui->lineButton->setDisabled(false);
    ui->pointButton->setDisabled(false);
    ui->polygonButton->setDisabled(false);

    QString name = QInputDialog::getText(this, "Add New Object", "Object Name:", QLineEdit::Normal, "", &ok);

    if (ok && !name.isEmpty()) {
        ui->drawArea->beginNewObject(name);
        statusBar()->showMessage("Drawing new object: '" + name + "'. Click 'Finish Object' when done.");
    }
}

void MainWindow::on_finishButton_clicked()
{
    if (m_isDrawingPolygon) {
        if (m_previousPoint != m_firstPoint) {
            QLine closingLine(m_previousPoint, m_firstPoint);
            ui->drawArea->addLineToCurrentObject(closingLine);
        }
    }

    ui->drawArea->endNewObject();
    m_isDrawingPolygon = false;
    i = false;

    ui->lineButton->setDisabled(true);
    ui->pointButton->setDisabled(true);
    ui->polygonButton->setDisabled(true);
}

void MainWindow::on_lineButton_clicked()
{
    ui->lineButton->setDisabled(true);
    ui->pointButton->setDisabled(false);
    ui->polygonButton->setDisabled(false);
}


void MainWindow::on_polygonButton_clicked()
{
    ui->lineButton->setDisabled(false);
    ui->pointButton->setDisabled(false);
    ui->polygonButton->setDisabled(true);
    m_isDrawingPolygon = true;
    option = 0;
}


void MainWindow::on_pointButton_clicked()
{
    ui->lineButton->setDisabled(false);
    ui->pointButton->setDisabled(true);
    ui->polygonButton->setDisabled(false);
}

void MainWindow::on_objectAdded_to_list(const QString &name, int id)
{
    QString itemText = QString("%1 (ID: %2)").arg(name).arg(id);
    ui->objectListWidget->addItem(itemText);
}

