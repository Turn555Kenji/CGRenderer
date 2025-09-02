#include "object.h"

SceneObject::SceneObject(int id, const QString &name) : m_id(id), m_name(name)
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

const QVector<QLine> &SceneObject::lines() const
{
    return m_lines;
}

const QVector<QPoint> &SceneObject::points() const
{
    return m_points;
}
