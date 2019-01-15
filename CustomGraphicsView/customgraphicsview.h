#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QPainter>
#include <QPaintEvent>
#include <QOpenGLWidget>

class CustomGraphicsView : public QOpenGLWidget {

    Q_OBJECT

public:
    CustomGraphicsView(QWidget *parent = nullptr);
    ~CustomGraphicsView();
    void drawImage(const QImage &image);

private:
    QGraphicsScene *_scene = nullptr;
    QPainter _painter;
    QImage _image;

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // CUSTOMGRAPHICSVIEW_H
