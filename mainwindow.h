#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>

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
    //void finishPoint();
private slots:
    void on_PainterMouseClicked(int x, int y);

    void on_lineButton_clicked();

    void on_polygonButton_clicked();

    void on_pointButton_clicked();

    void on_objectAdded(const QString &name, int id);

    void on_deleteObjectButton_clicked();

    //void on_objectTableWidget_itemClicked();

    //void on_objectTableWidget_itemClicked(QTableWidgetItem *item);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
