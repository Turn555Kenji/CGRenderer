#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void on_drawLineButton_clicked();

    void on_drawPointButton_clicked();

    void on_clearButton_clicked();

    void on_PainterMouseClicked(int x, int y);

    void on_finishButton_clicked();

    void on_newObjectButton_clicked();

    void on_lineButton_clicked();

    void on_polygonButton_clicked();

    void on_pointButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
