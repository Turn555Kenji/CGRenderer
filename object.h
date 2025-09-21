#ifndef OBJECT_H
#define OBJECT_H

#include <QPainter>
#include <matrix.h>

class Obj
{
public:
    enum class Type {   //Point = 0, Line = 1, Polygon = 2
        Point = 0,
        Line,
        Polygon
    };

    Obj(int initId, const QString &initName, Type initType);
    Obj();

    virtual void draw(QPainter *painter) = 0;
    virtual Obj* transform(Matrix m) = 0;

    // Getters

    int getId() const {return id; }
    QString getName() const {return name; }
    QString getType() const;

private:
    int id;
    QString name;
    Type type;
};

#endif // OBJECT_H
