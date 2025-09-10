#include "sceneobject.h"

SceneObject::SceneObject(int id, const QString &name, const QString &type) : m_id(id), m_name(name), m_type(type)
{
}

void SceneObject::addLine(const QLine &line)
{
    m_lines.append(line);
}

void SceneObject::addPoint(const QPoint &point)
{
    m_points.append(point);
}

int SceneObject::id() const
{
    return m_id;
}

const QString &SceneObject::name() const
{
    return m_name;
}

const QString &SceneObject::type() const
{
    return m_type;
}

const QVector<QLine> &SceneObject::lines() const
{
    return m_lines;
}

const QVector<QPoint> &SceneObject::points() const
{
    return m_points;
}
