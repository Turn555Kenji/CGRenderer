#include "polygon.h"
#include <QPainter>
#include <QVector>

// Construtor: inicializa o objeto e copia a lista de vértices recebida
Polygon::Polygon(const QList<Point>& vertices, int id, QString name)
    : Obj(id, name, Type::Polygon), vertices(vertices) {
}

// Método para desenhar o polígono na tela
void Polygon::draw(QPainter *painter) {
    // O painter do Qt não entende nossa classe "Point", então precisamos converter
    QVector<QPoint> Points;

    for (const Point &p : vertices) {
        int x = static_cast<int>(p[0][0]);
        int y = static_cast<int>(p[1][0]);
        Points.append(QPoint(x, y));
    }

    if (Points.size() > 1) {
        for (int i = 0; i < Points.size() - 1; ++i) {
            painter->drawLine(Points[i], Points[i + 1]);
        }
    }
}

// Método para aplicar uma transformação matricial a todos os vértices
Obj* Polygon::transform(const Matrix m) {/*

    QList<Point> transformedVertices;
    for (const Point& p : vertices) {
        // Multiplica a matriz de transformação (m) por cada ponto (p)
        Matrix result = m * p;
        // Cria um novo ponto com o resultado da transformação
        transformedVertices.append(Point(result[0][0], result[1][0]));
    }
    // Atualiza os vértices do polígono com os novos pontos transformados
    this->vertices = transformedVertices;

    return this;*/
}

void Polygon::addPoint(Point p){
    this->vertices.append(p);
}
