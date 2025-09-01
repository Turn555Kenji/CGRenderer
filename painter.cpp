#include "painter.h"
#include <QPainter>
#include <QPaintEvent>

PainterWidget::PainterWidget(QWidget *parent)
    : QWidget{parent}
{
    // Has to do with windows screen focus...?
    setFocusPolicy(Qt::StrongFocus);
}

/*void PainterWidget::addPoint(const QPoint &point)
{
    m_points.append(point);
    update();
}

void PainterWidget::addLine(const QLine &line)
{
    m_lines.append(line);
    update();
}

void PainterWidget::clearShapes()
{
    m_points.clear();
    m_lines.clear();
    update();
}*/

void PainterWidget::beginNewObject(const QString &name)
{
    // Finalize any previous object that wasn't completed
    if (m_currentObject) {
        endNewObject();
    }
    m_currentObject = new SceneObject(m_nextObjectId++, name);
    update();
}

// Step 2 of creation: Called by your mouse events
void PainterWidget::addLineToCurrentObject(const QLine &line)
{
    if (m_currentObject) {
        m_currentObject->addLine(line);
        update(); // Redraw to show the new line in real-time
    }
}

void PainterWidget::addPointToCurrentObject(const QPoint &point)
{
    if (m_currentObject) {
        m_currentObject->addPoint(point);
        update();
    }
}

// Step 3 of creation: Called by MainWindow when the user is done
void PainterWidget::endNewObject()
{
    if (m_currentObject) {
        m_objects.append(*m_currentObject);
        emit objectAdded(m_currentObject->name(), m_currentObject->id());
        delete m_currentObject;
        m_currentObject = nullptr;
        update();
    }
}

void PainterWidget::removeObject(int id)
{
    auto it = std::remove_if(m_objects.begin(), m_objects.end(),
                             [id](const SceneObject &obj) { return obj.id() == id; });

    if (it != m_objects.end()) {
        m_objects.erase(it, m_objects.end());
        update();
    }
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
