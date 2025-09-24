#include "painter.h"
#include <QPainter>
#include <QPaintEvent>
#include "point.h"
#include "line.h"
#include "polygon.h"


PainterWidget::PainterWidget(QWidget *parent)
    : QWidget{parent}
{
    setFocusPolicy(Qt::StrongFocus);
}

void PainterWidget::setupCoordinates(){
    OGXwmin = 0;
    OGYwmin = 0;
    OGXwmax = width();
    OGYwmax = height();
    OGXvpmin = 0;
    OGYvpmin = 0;
    OGXvpmax = width() - 1;
    OGYvpmax = height() - 1;

    Xwmin = OGXwmin;
    Ywmin = OGYwmin;
    Xwmax = OGXwmax;
    Ywmax = OGYwmax;
    Xvpmin = OGXvpmin;
    Yvpmin = OGYvpmin;
    Xvpmax = OGXvpmax;
    Yvpmax = OGYvpmax;

    // Cria e adiciona o objeto window se ele não existir
    if (!m_windowObject) {
        QList<Point> vertices;
        vertices.append(Point(Xwmin, Ywmin));
        vertices.append(Point(Xwmax, Ywmin));
        vertices.append(Point(Xwmax, Ywmax));
        vertices.append(Point(Xwmin, Ywmax));
        m_windowObject = new Polygon(vertices, -1, "Window"); // ID especial -1
        m_windowObject->setClosed();
        displayFile.prepend(m_windowObject); // Insere no início da lista
    }
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

    if (m_windowObject) {
        QList<Point> originalVertices;
        originalVertices.append(Point(OGXwmin, OGYwmin));
        originalVertices.append(Point(OGXwmax, OGYwmin));
        originalVertices.append(Point(OGXwmax, OGYwmax));
        originalVertices.append(Point(OGXwmin, OGYwmax));
        m_windowObject->setVertices(originalVertices);
    }

    update();
}

void PainterWidget::addPointToCurrentObject(int x, int y, const QString &name)
{
    if (m_currentObject) {
        endNewObject();
    }
    m_currentObject = new Point(x, y, m_nextObjectId++, name);
    update();
}

void PainterWidget::addLineToCurrentObject(Point* p1, Point* p2, const QString name)
{
    if (m_currentObject) {
        //endNewObject();
    }
    m_currentObject = new Line(*p1, *p2, m_nextObjectId++, name);
    update();
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
    update();
}

void PainterWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    for (Obj *obj : displayFile) {
        // Lógica para desenhar a window com estilo diferente
        if (obj->getId() == -1) {
            QPen windowPen(Qt::white, 1);
            windowPen.setStyle(Qt::DashLine);
            painter.setPen(windowPen);
        } else {
            painter.setPen(QPen(Qt::red, 5));
        }
        obj->draw(&painter, Xwmin, Ywmin, Xwmax, Ywmax, Xvpmin, Yvpmin, Xvpmax, Yvpmax);
    }
    if (m_currentObject) {
        painter.setPen(QPen(Qt::cyan, 5));
        m_currentObject->draw(&painter, Xwmin, Ywmin, Xwmax, Ywmax, Xvpmin, Yvpmin, Xvpmax, Yvpmax);
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

// ESTA É A ÚNICA DEFINIÇÃO CORRETA DE setWindow
void PainterWidget::setWindow(int newXwmax, int newXwmin, int newYwmax, int newYwmin){
    Xwmax = newXwmax;
    Xwmin = newXwmin;
    Ywmax = newYwmax;
    Ywmin = newYwmin;

    // Atualiza os vértices do objeto window para refletir as novas coordenadas
    if (m_windowObject) {
        QList<Point> newVertices;
        newVertices.append(Point(Xwmin, Ywmin));
        newVertices.append(Point(Xwmax, Ywmin));
        newVertices.append(Point(Xwmax, Ywmax));
        newVertices.append(Point(Xwmin, Ywmax));
        m_windowObject->setVertices(newVertices);
    }

    update();
}

void PainterWidget::setViewPort(int newXvpmax, int newXvpmin, int newYvpmax, int newYvpmin){
    Xvpmax = newXvpmax;
    Xvpmin = newXvpmin;
    Yvpmax = newYvpmax;
    Yvpmin = newYvpmin;
    update();
}
