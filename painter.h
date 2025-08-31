#ifndef PAINTER_H
#define PAINTER_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QLine>

class PainterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PainterWidget(QWidget *parent = nullptr);

    void addPoint(const QPoint &point);
    void addLine(const QLine &line);
    void clearShapes();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QVector<QPoint> m_points;
    QVector<QLine> m_lines;

signals:
    void mouseClick(int x, int y);
};

#endif // PAINTER_H
