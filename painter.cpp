#include "painter.h"
#include <QPainter>
#include <QPaintEvent>

PainterWidget::PainterWidget(QWidget *parent)
    : QWidget{parent}
{
    // Has to do with windows screen focus...?
    setFocusPolicy(Qt::StrongFocus);
}

void PainterWidget::addPoint(const QPoint &point)
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
}

void PainterWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(Qt::blue, 2));
    for (const QLine &line : m_lines) {
        painter.drawLine(line);
    }

    painter.setPen(QPen(Qt::red, 5));
    for (const QPoint &point : m_points) {
        painter.drawPoint(point);
    }
}

void PainterWidget::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit mouseClick(event->pos().x(), event->pos().y());
    }

    QWidget::mousePressEvent(event);
}
