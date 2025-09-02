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
    QList<SceneObject> m_objects;        // Display File
    SceneObject* m_currentObject = nullptr; // Current object

    int m_nextObjectId = 0;

public slots:
    void removeObject(int id);

signals:
    void mouseClick(int x, int y);
    void objectAdded(const QString &name, int id, int pointNum, int lineNum);
};

#endif // PAINTER_H
