#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>

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

    void on_objectAdded_to_list(const QString &name, int id);

private:
    Ui::MainWindow *ui;
    bool m_isDrawingPolygon = false;
    QPoint m_firstPoint;
    QPoint m_previousPoint;
};
#endif // MAINWINDOW_H
