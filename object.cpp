#include "object.h"

Obj::Obj(int initId, const QString &initName, Type initType) : id(initId), name(initName), type(initType)
{}

Obj::Obj() : id(0), name(""), type(Type::Point)
{}

QString Obj::getType() const{
    switch (this->type) {
    case Obj::Type::Point:   return "Point";
    case Obj::Type::Line:    return "Line";
    case Obj::Type::Polygon: return "Polygon";
    default:                 return "Unknown";
    }
}
