#include "object.h"

Obj::Obj(int initId, const QString &initName, Type initType) : id(initId), name(initName), type(initType)
{}

Obj::Obj() : id(0), name(""), type(Type::Point)
{}
