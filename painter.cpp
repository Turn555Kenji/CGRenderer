#include "painter.h"
#include <QPainter>
#include <QPaintEvent>
#include "point.h"
#include "line.h"


//#include <iostream>

PainterWidget::PainterWidget(QWidget *parent)
    : QWidget{parent}
{
    // Window Screen focus
    setFocusPolicy(Qt::StrongFocus);
}

void PainterWidget::addPointToCurrentObject(int x, int y,const QString &name)//anteriormente qpoint
{

    if (m_currentObject) {
        //delete m_currentPoint;
        endNewObject();
    }
    m_currentObject = new Point(x, y, m_nextObjectId++, name);

    update();
}

void PainterWidget::addLineToCurrentObject(Point* p1, Point* p2,const QString name)
{
    if (m_currentObject) {
        //delete m_currentPoint;
        endNewObject();
    }
    m_currentObject= new Line(*p1, *p2, m_nextObjectId++, name);
    update();    //(p1, p2, m_nextObjectId++, name);

}


void PainterWidget::endNewObject()
{
    if (m_currentObject) {
        m_objects.append(m_currentObject);
        emit objectAdded(m_currentObject->getName(), m_currentObject->getId());
        //delete m_currentObject;
        m_currentObject = nullptr;
        update();
    }
}

void PainterWidget::removeObject(int id){

    auto it = m_objects.begin();
    for (; it != m_objects.end(); ) {
        Obj* currentObject = *it;

        if (currentObject->getId() == id) {
            it = m_objects.erase(it);
        } else {
            ++it;
        }
    }

    if (it != m_objects.end()) {
        m_objects.erase(it, m_objects.end());
        update();
    }

    update();
}

void PainterWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //painter.setRenderHint(QPainter::Antialiasing);

    for (Obj *obj : m_objects) {
        painter.setPen(QPen(Qt::red, 5));
        obj->draw(&painter);
    }
    if (m_currentObject) {
        painter.setPen(QPen(Qt::cyan, 5));
        m_currentObject->draw(&painter);
        //endNewObject();
    }

}

void PainterWidget::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit mouseClick(event->pos().x(), event->pos().y());
    }

    QWidget::mousePressEvent(event);
}
void PainterWidget::addPoint(const Point &point)
{
    m_points.append(point);
}

/*SceneObject* PainterWidget::getObject(int id){}
{
    for (auto it = m_objects.begin(); it != m_objects.end(); ++it) {
        if (it->id() == id) {
            return &(*it);
        }
    }
    return nullptr;
}
*/
