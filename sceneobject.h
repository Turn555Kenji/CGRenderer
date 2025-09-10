#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QString>
#include <QVector>
#include <QLine>
#include <QPoint>

class SceneObject
{
public:
    SceneObject(int id, const QString &name, const QString &type);

    void addLine(const QLine &line);
    void addPoint(const QPoint &point);

    // Getters
    int id() const;
    const QString& name() const;
    const QString& type() const;
    const QVector<QLine>& lines() const;
    const QVector<QPoint>& points() const;

private:    //Display File
    int m_id;
    QString m_name;
    QString m_type;
    QVector<QLine> m_lines;
    QVector<QPoint> m_points;
};

#endif // SCENEOBJECT_H
