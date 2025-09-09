#ifndef OBJECT_H
#define OBJECT_H

#include <QPainter>

class Obj
{
public:
    Obj(int initId, const QString &initName, const QString &initType);
    virtual void draw(QPainter *painter) = 0;
    virtual Obj* transform(Matrix m) = 0;

    // Getters

    int getId() const {return rows; }
    String getName() const {return columns; }
    String getType() const {return type; }

private:
    int id;
    QString name;
    QString type;
};

#endif // OBJECT_H
