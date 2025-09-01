#ifndef PAINTER_H
#define PAINTER_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QLine>
#include <object.h>

class PainterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PainterWidget(QWidget *parent = nullptr);

    void beginNewObject(const QString &name);
    void addLineToCurrentObject(const QLine &line);
    void addPointToCurrentObject(const QPoint &point);
    void endNewObject();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    //Implement MoveEvent for preview!!

private:
    QList<SceneObject> m_objects;        // All finalized objects
    SceneObject* m_currentObject = nullptr; // The object being drawn right now

    int m_nextObjectId = 0;

public slots:
    void removeObject(int id);

signals:
    void mouseClick(int x, int y);
    void objectAdded(const QString &name, int id);
};

#endif // PAINTER_H
