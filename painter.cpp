#include "painter.h"
#include <QPainter>
#include <QPaintEvent>

PainterWidget::PainterWidget(QWidget *parent)
    : QWidget{parent}
{
    // Window Screen focus
    setFocusPolicy(Qt::StrongFocus);
}

void PainterWidget::beginNewObject(const QString &name, const QString &type)
{
    if (m_currentObject) {
        endNewObject();
    }
    m_currentObject = new SceneObject(m_nextObjectId++, name, type);
    update();
}

void PainterWidget::addLineToCurrentObject(const QLine &line)
{
    if (m_currentObject) {
        m_currentObject->addLine(line);
        update();
    }
}

void PainterWidget::addPointToCurrentObject(const QPoint &point)
{
    if (m_currentObject) {
        m_currentObject->addPoint(point);
        update();
    }
}

/*void PainterWidget::addSquareToCurrentObject(const QPoint &point)
{
    if (m_currentObject) {
        m_currentObject->addPoint(point);
        update();
    }
}*/

void PainterWidget::endNewObject()
{
    if (m_currentObject) {
        m_objects.append(*m_currentObject);
        emit objectAdded(m_currentObject->name(), m_currentObject->type(), m_currentObject->id(), m_currentObject->points().size(), m_currentObject->lines().size());
        delete m_currentObject;
        m_currentObject = nullptr;
        update();
    }
}

void PainterWidget::removeObject(int id)
{
    auto it = m_objects.begin();
    for (; it != m_objects.end(); ) {
        if (it->id() == id) {
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

    for (const SceneObject &obj : m_objects) {
        painter.setPen(QPen(Qt::blue, 2));
        for (const QLine &line : obj.lines()) {
            painter.drawLine(line);
        }
        painter.setPen(QPen(Qt::red, 5));
        for (const QPoint &point : obj.points()) {
            painter.drawPoint(point);
        }
    }

    if (m_currentObject) {
        painter.setPen(QPen(Qt::green, 2, Qt::DashLine)); // Draw with a different style
        for (const QLine &line : m_currentObject->lines()) {
            painter.drawLine(line);
        }
        for (const QPoint &point : m_currentObject->points()) {
            painter.drawPoint(point);
        }
    }
}

void PainterWidget::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit mouseClick(event->pos().x(), event->pos().y());
    }

    QWidget::mousePressEvent(event);
}
