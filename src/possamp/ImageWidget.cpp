// Copyright (c) Burator 2014-2015
#include <QImage>
#include <QDebug>
#include <QString>
#include <QPixmap>
#include <QPainter>
#include <QVBoxLayout>
#include <QMouseEvent>

#include "Util.h"
#include "ImageWidget.h"

B_BEGIN_NAMESPACE

ImageWidget::ImageWidget() : zoomFactor(1), zoomSet(false) { }

void ImageWidget::loadImage(const QString &path) {
  image = origImage = QImage(path);
  if (zoomSet) {
    setZoom(zoomFactor);
  }
  else {
    resize(image.size());
  }
}

void ImageWidget::setZoom(float factor) {
  zoomFactor = factor;
  zoomSet = true;
  if (origImage.isNull()) {
    return;
  }
  image = origImage.scaledToWidth(float(origImage.width()) * factor);
  resize(image.size());
  update();
}

void ImageWidget::resetZoom() {
  zoomFactor = 1.0;
  zoomSet = false;
  if (origImage.isNull()) {
    return;
  }
  image = origImage;
  resize(image.size());
  update();
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
  update();
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
    QRect obj = getCurObject();
    emit newObject(zoomSet ? Util::scaleRect(obj, 1.0 / zoomFactor) : obj);
  }
  startPos = curPos = QPoint();
  update();
}

void ImageWidget::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.drawImage(QPoint(0, 0), image);

  foreach (const QRect &object, curObjects) {
    drawObject(painter,
               (zoomSet ? Util::scaleRect(object, zoomFactor) : object));
  }

  if (!startPos.isNull() && !curPos.isNull() && startPos != curPos) {
    drawObject(painter, getCurObject());
  }
}

QRect ImageWidget::getCurObject() const {
  int x = startPos.x(), y = startPos.y(),
    x2 = curPos.x(), y2 = curPos.y();
  if (x2 < x) qSwap(x, x2);
  if (y2 < y) qSwap(y, y2);
  if (x < 0) x = 0;
  if (y < 0) y = 0;
  int w = x2 - x;
  int h = y2 - y;
  if (x + w > image.width()) {
    w = image.width() - x;
  }
  if (y + h > image.height()) {
    h = image.height() - y;
  }
  return QRect(x, y, w, h);
}

void ImageWidget::drawObject(QPainter &painter, const QRect &object) {
  painter.save();

  QPen pen = painter.pen();
  pen.setWidth(2);
  pen.setColor(Qt::red);
  painter.setPen(pen);

  painter.drawRect(object);
  painter.restore();
}

B_END_NAMESPACE
