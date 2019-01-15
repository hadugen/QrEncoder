#include "customgraphicsview.h"
#include <QDebug>
CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    : QOpenGLWidget (parent),
      _image(QImage(1000, 1000, QImage::Format_Mono)){
    _image.fill(Qt::black);
}

CustomGraphicsView::~CustomGraphicsView() {
    delete _scene;
}

void CustomGraphicsView::drawImage(const QImage &image) {
    _image = image;
    update();
}

void CustomGraphicsView::paintEvent(QPaintEvent *event) {
    _painter.begin(this);
    _painter.fillRect(event->rect(), Qt::white);
    QImage _frame = _image.scaled(
                event->rect().width(),
                event->rect().height(),
                Qt::KeepAspectRatio
    );
    _painter.drawImage(_frame.rect(), _frame);
    _painter.end();
}
