#ifndef TYPEOBJ_H
#define TYPEOBJ_H

#include "object.h"      // Inclua a classe base
#include "polygon.h"  // Inclua a classe das "faces"
#include <QList>
#include <QPainter>

class TypeObj : public Obj {
public:

    TypeObj(const QList<Polygon>& faces, int id, QString name);
    virtual void draw(QPainter *painter, double dist, bool perspectflag,
                      double Xwmin, double Ywmin, double Xwmax, double Ywmax,
                      double Xvpmin, double Yvpmin, double Xvpmax, double Yvpmax) override;


    virtual Obj* transform(Matrix m) override;
    void addFace(Polygon p); // Passando por valor para espelhar addPoint(Point p)
    void setFaces(const QList<Polygon>& newFaces);

    QList<Polygon> getFaces() const { return faces; }

    //QList<Polygon> getFaces() const;


private:

    QList<Polygon> faces;
};

#endif
