#include "painter.h"
#include <QPainter>
#include <QPaintEvent>
#include "point.h"
#include "line.h"
#include "polygon.h"
#include "matrixmath.h"
#include "typeobj.h"
#include <QKeyEvent>
#include <QDebug>

const int INSIDE = 0;
const int LEFT   = 1;
const int RIGHT  = 2;
const int BOTTOM = 4;
const int TOP    = 8;

PainterWidget::PainterWidget(QWidget *parent)
    : QWidget{parent}
{
    setFocusPolicy(Qt::StrongFocus);

    camX = width() / 2.0;  // Centro da tela horizontal
    camY = height() / 2.0; // Centro da tela vertical
    camZ = 500.0;          // Afastado para trás
}

void PainterWidget::setupCoordinates(){
    dist = 0;

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


void PainterWidget::addPolygon(const QList<Point>& vertices, const QString& name, bool closed)
{
   m_currentObject = new Polygon(vertices, m_nextObjectId++, name);
    displayFile.append(m_currentObject);
    update();
}


void PainterWidget::addTypeObject(const QList<Polygon>& faces, const QString& name)
{
   m_currentObject = new TypeObj(faces, m_nextObjectId++, name);

    displayFile.append(   m_currentObject);
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

    Point eye(camX, camY, camZ);
    Point target(camX, camY, 0); // Olha para o fundo da tela na mesma altura
    Point up(0, 1, 0);

    Matrix viewMatrix = MatrixMath::lookAt(eye, target, up);

    if (m_windowObject) {
        QPen windowPen(Qt::white, 1);
        windowPen.setStyle(Qt::DashLine);
        painter.setPen(windowPen);
        painter.drawRect(Xvpmin, Yvpmin, Xvpmax - Xvpmin, Yvpmax - Yvpmin);
    }


    int vpWidth = Xvpmax - Xvpmin;
    int vpHeight = Yvpmax - Yvpmin;

    //onde cortar a window
    painter.setClipRect(Xvpmin, Yvpmin, vpWidth, vpHeight);

    for (Obj *obj : displayFile) {
        if (obj->getId() != -1) {
            painter.setPen(QPen(Qt::white, 1));
            obj->draw(&painter, dist, perspectFlag, viewMatrix, Xwmin, Ywmin, Xwmax, Ywmax, Xvpmin, Yvpmin, Xvpmax, Yvpmax);
        }
    }

    if (m_currentObject) {
        painter.setPen(QPen(Qt::cyan, 5));
        m_currentObject->draw(&painter, dist, perspectFlag, viewMatrix, Xwmin, Ywmin, Xwmax, Ywmax, Xvpmin, Yvpmin, Xvpmax, Yvpmax);
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

int PainterWidget::Regiao(double x, double y)
{
    int code = INSIDE;

    if (x < Xwmin)
        code |= LEFT;
    else if (x > Xwmax)
        code |= RIGHT;

    if (y < Ywmin)
        code |= BOTTOM;
    else if (y > Ywmax)
        code |= TOP;

    return code;
}


bool PainterWidget::clipplingCohen(double& x1, double& y1, double& x2, double& y2)
{
    int code1 = Regiao(x1, y1);
    int code2 = Regiao(x2, y2);
    bool accept = false;

    while (true) {
        if ((code1 == 0) && (code2 == 0)) { //dentro da window
            accept = true;
            break;
        } else if (code1 & code2) {
            // Caso2 os dois estão fora
            accept = false;
            break;
        } else {
            // Caso 3 segmento de reta
            double x, y;
            int codeOut;
            // seleciona a parte da reta que está pra fora
            if (code1 != 0) {
                codeOut = code1;
            } else {
                codeOut = code2;
            }

            // interseção.
            if (codeOut & TOP) { //acima
                x = x1 + (x2 - x1) * (Ywmax - y1) / (y2 - y1);
                y = Ywmax;
            } else if (codeOut & BOTTOM) {//abaixo
                x = x1 + (x2 - x1) * (Ywmin - y1) / (y2 - y1);
                y = Ywmin;
            } else if (codeOut & RIGHT) {//direita
                y = y1 + (y2 - y1) * (Xwmax - x1) / (x2 - x1);
                x = Xwmax;
            } else if (codeOut & LEFT) {   //esquerda
                y = y1 + (y2 - y1) * (Xwmin - x1) / (x2 - x1);
                x = Xwmin;
            }

            if (codeOut == code1) {
                x1 = x;
                y1 = y;
                code1 = Regiao(x1, y1);
            } else {
                x2 = x;
                y2 = y;
                code2 = Regiao(x2, y2);
            }
        }
    }
    return accept;
}

void PainterWidget::rotateScene(int angle, int xpivot, int ypivot, int zpivot) {
    // Itera sobre todos os objetos no displayFile
    for (Obj* obj : displayFile) {
        // Aplica a rotação a todos, exceto à própria window
        if (obj->getId() != -1) {
            MatrixMath::rotateObject(obj, angle, 0, 3, xpivot, ypivot, zpivot);
        }
    }
    update(); // Força o widget a se redesenhar com as novas posições
}

void PainterWidget::setDistance(double value){
    dist = value;
}


void PainterWidget::setProjection(bool value){
    perspectFlag = value;
}

void PainterWidget::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Tecla:" << event->key();

    double step = 20.0; // Velocidade da câmera

    switch (event->key()) {
    case Qt::Key_A:
        camX -= step; // Move para esquerda
        break;
    case Qt::Key_D:
        camX += step; // Move para direita
        break;
    case Qt::Key_W:
        camY += step; // Sobe (lembre que no seu mundo Y cresce pra cima ou baixo dependendo da config)
        break;
    case Qt::Key_S:
        camY -= step; // Desce
        break;
    case Qt::Key_Q:
        camZ += step; // Afasta (Zoom Out)
        break;
    case Qt::Key_E:
        camZ -= step; // Aproxima (Zoom In)
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }

    qDebug() << "Nova Camera:" << camX << camY << camZ;
    // Força o redesenho da tela com a nova posição
    update();
}
