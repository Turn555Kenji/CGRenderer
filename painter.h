#ifndef PAINTER_H
#define PAINTER_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QLine>
#include <object.h>
#include "point.h"
class PainterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PainterWidget(QWidget *parent = nullptr);

    //void beginNewObject();
    void setupCoordinates();
    void addLineToCurrentObject(Point* p1, Point* p2,const QString name);
    void addPointToCurrentObject( int x,  int y ,const QString &name);//anteriormente qpoint
    void addVertexToCurrentObject(Point *p1, Point *p2, const QString name);
    void closePolygonObject();
    void endNewObject();
    Obj* getObject(int id);
    void setWindow(int newXwmax, int newXwmin, int newYwmax, int newYwmin);
    void setViewPort(int newXvpmax, int newXvpmin, int newYvpmax, int newYvpmin);
    void  resetWindowViewPort();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    //Implement MoveEvent for preview!!

private:
    QList<Obj*> displayFile;        // Display File
    Obj* m_currentObject = nullptr; // Current object
    //Point* m_currentPoint = nullptr;
    int m_nextObjectId = 0;
    double OGXwmin, OGYwmin, OGXwmax, OGYwmax;
    double OGXvpmin, OGYvpmin, OGXvpmax, OGYvpmax;

    double Xwmin, Ywmin, Xwmax, Ywmax;
    double Xvpmin, Yvpmin, Xvpmax, Yvpmax;

public slots:
    void removeObject(int id);

signals:
    void mouseClick(int x, int y);
    void objectAdded(const QString &name, int id, const QString &type);
};

#endif // PAINTER_H
