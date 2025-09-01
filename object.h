#ifndef OBJECT_H
#define OBJECT_H

#include <QString>
#include <QVector>
#include <QLine>
#include <QPoint>

class SceneObject
{
public:
    SceneObject(int id, const QString &name);

    void addLine(const QLine &line);
    void addPoint(const QPoint &point);

    // Getters
    int id() const;
    const QString& name() const;
    const QVector<QLine>& lines() const;
    const QVector<QPoint>& points() const;

private:    //Display File
    int m_id;
    QString m_name;
    QList<QLine> m_lines;
    QList<QPoint> m_points;
};

#endif // OBJECT_H
