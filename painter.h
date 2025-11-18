#ifndef PAINTER_H
#define PAINTER_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QLine>
#include <object.h>
#include "point.h"
#include "polygon.h"

class PainterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PainterWidget(QWidget *parent = nullptr);

    void setupCoordinates();
    void addLineToCurrentObject(Point* p1, Point* p2,const QString name);
    void addPointToCurrentObject( int x,  int y ,const QString &name);
    void addVertexToCurrentObject(Point *p1, Point *p2, const QString name);
    void addPolygon(const QList<Point>& vertices, const QString& name, bool closed);
    void closePolygonObject();
    void endNewObject();
    Obj* getObject(int id);
    void setWindow(int newXwmax, int newXwmin, int newYwmax, int newYwmin);
    void setViewPort(int newXvpmax, int newXvpmin, int newYvpmax, int newYvpmin);
    void resetWindowViewPort();
    void rotateScene(int angle, int xpivot, int ypivot, int zpivot);

    void addTypeObject(const QList<Polygon>& faces, const QString& name);

    double getXwmin() const { return Xwmin; }
    double getYwmin() const { return Ywmin; }
    double getXwmax() const { return Xwmax; }
    double getYwmax() const { return Ywmax; }

    double getXvpmin() const { return Xvpmin; }
    double getYvpmin() const { return Yvpmin; }
    double getXvpmax() const { return Xvpmax; }
    double getYvpmax() const { return Yvpmax; }

    void setDistance(double value);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QList<Obj*> displayFile;
    Obj* m_currentObject = nullptr;
    int m_nextObjectId = 0;

    int Regiao(double x, double y);
    bool clipplingCohen(double& x1, double& y1, double& x2, double& y2);
    // LINHA ABAIXO CORRIGIDA (OGYmax -> OGYwmax)
    double OGXwmin, OGYwmin, OGXwmax, OGYwmax;
    double OGXvpmin, OGYvpmin, OGXvpmax, OGYvpmax;

    double Xwmin, Ywmin, Xwmax, Ywmax;
    double Xvpmin, Yvpmin, Xvpmax, Yvpmax;

    double dist;

    Polygon* m_windowObject = nullptr;

public slots:
    void removeObject(int id);

signals:
    void mouseClick(int x, int y);
    void objectAdded(const QString &name, int id, const QString &type);
};

#endif // PAINTER_H
