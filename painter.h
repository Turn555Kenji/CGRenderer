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
    void addLineToCurrentObject(Point* p1, Point* p2,const QString name);
    void addPointToCurrentObject( int x,  int y ,const QString &name);//anteriormente qpoint
    void addLineToPolygon(Point* p1, Point* p2);
    void endNewObject();
    void addPoint(const Point &point);
    //Obj* getObject(int id);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    //Implement MoveEvent for preview!!

private:
    QList<Obj*> m_objects;        // Display File
    Obj* m_currentObject = nullptr; // Current object
    QVector<Point>m_points;
    //Point* m_currentPoint = nullptr;
    int m_nextObjectId = 0;

public slots:
    void removeObject(int id);

signals:
    void mouseClick(int x, int y);
    void objectAdded(const QString &name, int id);
};

#endif // PAINTER_H
