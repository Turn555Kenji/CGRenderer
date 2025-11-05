#include "TypeObj.h"


TypeObj::TypeObj(const QList<Polygon>& faces, int id, QString name)
    : Obj(id, name, Type::TypeObj), faces(faces) // Usa a lista de inicialização
{}



void TypeObj::draw(QPainter *painter,
                           double Xwmin, double Ywmin, double Xwmax, double Ywmax,
                           double Xvpmin, double Yvpmin, double Xvpmax, double Yvpmax)
{

    for (Polygon& face : this->faces) {
        face.draw(painter, Xwmin, Ywmin, Xwmax, Ywmax, Xvpmin, Yvpmin, Xvpmax, Yvpmax);
    }
}


Obj* TypeObj::transform(Matrix m)
{
    for (Polygon& face : this->faces) {
        face.transform(m);
    }
    return this;
}


void TypeObj::addFace(Polygon p)
{
    this->faces.append(p);
}


void TypeObj::setFaces(const QList<Polygon>& newFaces)
{
    this->faces = newFaces;
}


