// Copyright (c) Burator 2014-2015
#include <QImage>
#include <QDebug>
#include <QString>
#include <QPixmap>
#include <QPainter>
#include <QVBoxLayout>
#include <QMouseEvent>

#include "ImageWidget.h"

B_BEGIN_NAMESPACE

void ImageWidget::loadImage(const QString &path) {
  this->path = path;
  image = QImage(path);
  resize(image.size());
}

void ImageWidget::showObject(const QRect &object) {
  curObjects.clear();
  curObjects << object;
  update();
}

void ImageWidget::showObjects(const QList<QRect> &objects) {
  curObjects = objects;
  update();
}

void ImageWidget::clear() {
  curObjects.clear();
  startPos = curPos = QPoint();
}

void ImageWidget::mousePressEvent(QMouseEvent *event) {
  QWidget::mousePressEvent(event);
  startPos = event->pos();
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event) {
  QWidget::mouseMoveEvent(event);
  curPos = event->pos();
  update();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *event) {
  QWidget::mouseReleaseEvent(event);
  if (!startPos.isNull() && !curPos.isNull() && startPos != curPos) {
    emit newObject(getCurObject());
  }
  startPos = curPos = QPoint();
  update();
}

void ImageWidget::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.drawImage(QPoint(0, 0), image);

  foreach (const QRect &object, curObjects) {
    drawObject(painter, object);
  }

  if (!startPos.isNull() && !curPos.isNull()) {
    drawObject(painter, getCurObject());
  }
}

QRect ImageWidget::getCurObject() const {
  int x = startPos.x(), y = startPos.y(),
    x2 = curPos.x(), y2 = curPos.y();
  if (x2 < x) qSwap(x, x2);
  if (y2 < y) qSwap(y, y2);
  return QRect(x, y, x2 - x, y2 - y);
}

void ImageWidget::drawObject(QPainter &painter, const QRect &object) {
  QPen pen = painter.pen();
  pen.setWidth(2);
  pen.setColor(Qt::red);
  painter.setPen(pen);
  painter.drawRect(object);
}

B_END_NAMESPACE
