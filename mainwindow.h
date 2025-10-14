#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "object.h"
#include "point.h"
#include "line.h"
#include "polygon.h"
#include "matrixmath.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void configureButtons(bool linB, bool poiB, bool polB);
    void finishObject();

    Obj *getTableObject();

private slots:
    void on_PainterMouseClicked(int x, int y);

    void on_lineButton_clicked();
    void on_polygonButton_clicked();
    void on_pointButton_clicked();

    void on_objectAdded(const QString &name, int id, const QString &type);

    void on_deleteObjectButton_clicked();
    void on_objectTableWidget_itemClicked();
    void on_objectTableWidget_itemClicked(QTableWidgetItem *item);

    void on_translateButton_clicked();
    void on_rotateButton_clicked();
    void on_scaleButton_clicked();

    void on_applyVpButton_clicked();

    void on_resetButton_clicked();

    void drawCustomShape(QList<Obj *> pointList, QString name);
    void on_treeButton_clicked();
    void on_houseButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
