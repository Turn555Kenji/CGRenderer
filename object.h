#ifndef OBJECT_H
#define OBJECT_H

#include <QPainter>
#include <matrix.h>

class Obj
{
public:
    Obj(int initId, const QString &initName, const QString &initType);
    Obj();
    virtual void draw(QPainter *painter) = 0;
    virtual Obj* transform(Matrix m) = 0;

    // Getters

    int getId() const {return id; }
    QString getName() const {return name; }
    QString getType() const {return type; }

private:
    int id;
    QString name;
    QString type;
};

#endif // OBJECT_H
