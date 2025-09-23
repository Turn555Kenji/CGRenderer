#include "painter.h"
#include <QPainter>
#include <QPaintEvent>
#include "point.h"
#include "line.h"
#include "polygon.h"


//#include <iostream>

PainterWidget::PainterWidget(QWidget *parent)
    : QWidget{parent}
{
    // Window Screen focus
    setFocusPolicy(Qt::StrongFocus);
}

void PainterWidget::setupCoordinates(){
    OGXwmin = 0;
    OGYwmin = 0;
    OGXwmax = width();
    OGYwmax = height();
    OGXvpmin = 0;
    OGYvpmin = 0;
    OGXvpmax = width();
    OGYvpmax = height();

    Xwmin = OGXwmin;
    Ywmin = OGYwmin;
    Xwmax = OGXwmax;
    Ywmax = OGYwmax;
    Xvpmin = OGXvpmin;
    Yvpmin = OGYvpmin;
    Xvpmax = OGXvpmax;
    Yvpmax = OGYvpmax;
}

void PainterWidget::resetWindowViewPort(){
    Xwmin = OGXwmin;
    Ywmin = OGYwmin;
    Xwmax = OGXwmax;
    Ywmax = OGYwmax;
    Xvpmin = OGXvpmin;
    Yvpmin = OGYvpmin;
    Xvpmax = OGXvpmax;
    Yvpmax = OGYvpmax;
    update();
}

void PainterWidget::addPointToCurrentObject(int x, int y, const QString &name)//anteriormente qpoint
{

    if (m_currentObject) {
        //delete m_currentPoint;
        endNewObject();
    }
    m_currentObject = new Point(x, y, m_nextObjectId++, name);

    update();
}

void PainterWidget::addLineToCurrentObject(Point* p1, Point* p2, const QString name)
{
    if (m_currentObject) {
        //delete m_currentPoint;
        //endNewObject();
    }
    m_currentObject = new Line(*p1, *p2, m_nextObjectId++, name);
    update();    //(p1, p2, m_nextObjectId++, name);

}

void PainterWidget::addVertexToCurrentObject(Point *p1, Point *p2, const QString name)
{
    if (m_currentObject) {
        Polygon *iter = dynamic_cast<Polygon*>(m_currentObject);
        iter->addPoint(*p2);
        update();
        return;
    }
    QList<Point> p;
    p.append(*p1);
    p.append(*p2);
    m_currentObject= new Polygon(p, m_nextObjectId++, name);
    update();

}

void PainterWidget::closePolygonObject(){
    Polygon *iter = dynamic_cast<Polygon*>(m_currentObject);
    iter->setClosed();
}


void PainterWidget::endNewObject()
{
    if (m_currentObject) {
        displayFile.append(m_currentObject);
        emit objectAdded(m_currentObject->getName(), m_currentObject->getId(), m_currentObject->getType());
        //delete m_currentObject;
        m_currentObject = nullptr;
        update();
    }
}

void PainterWidget::removeObject(int id){

    auto it = displayFile.begin();
    for (; it != displayFile.end(); ) {
        Obj* currentObject = *it;

        if (currentObject->getId() == id) {
            it = displayFile.erase(it);
        } else {
            ++it;
        }
    }

    if (it != displayFile.end()) {
        displayFile.erase(it, displayFile.end());
        update();
    }

    update();
}

void PainterWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //painter.setRenderHint(QPainter::Antialiasing);

    for (Obj *obj : displayFile) {
        painter.setPen(QPen(Qt::red, 5));
        obj->draw(&painter, Xwmin, Ywmin, Xwmax, Ywmax, Xvpmin, Yvpmin, Xvpmax, Yvpmax);
    }
    if (m_currentObject) {
        painter.setPen(QPen(Qt::cyan, 5));
        m_currentObject->draw(&painter, Xwmin, Ywmin, Xwmax, Ywmax, Xvpmin, Yvpmin, Xvpmax, Yvpmax);
        //endNewObject();
    }

}

void PainterWidget::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit mouseClick(event->pos().x(), event->pos().y());
    }

    QWidget::mousePressEvent(event);
}

Obj* PainterWidget::getObject(int id)
{
    for (auto* obj : displayFile) {
        if (obj->getId() == id) {
            return obj;
        }
    }
    return nullptr; // not found
}

void PainterWidget::setWindow(int newXwmax, int newXwmin, int newYwmax, int newYwmin){
    Xwmax = newXwmax;
    Xwmin = newXwmin;
    Ywmax = newYwmax;
    Ywmin = newYwmin;
    update();
}

void PainterWidget::setViewPort(int newXvpmax, int newXvpmin, int newYvpmax, int newYvpmin){
    Xvpmax = newXvpmax;
    Xvpmin = newXvpmin;
    Yvpmax = newYvpmax;
    Yvpmin = newYvpmin;
    update();
}
