// Copyright (c) Burator 2014-2015
#ifndef B_IMAGE_WIDGET_H
#define B_IMAGE_WIDGET_H

#include <QImage>
#include <QString>
#include <QWidget>

#include "Global.h"

class QRect;
class QPainter;

B_BEGIN_NAMESPACE

class ImageWidget : public QWidget {
  Q_OBJECT

public:
  ImageWidget();

  void loadImage(const QString &path);
  void setZoom(float factor);
  void resetZoom();

  void showObject(const QRect &object);
  void showObjects(const QList<QRect> &objects);
  void clear();

signals:
  void newObject(QRect object);

protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void paintEvent(QPaintEvent *event);

private:
  QRect getCurObject() const;
  void drawObject(QPainter &painter, const QRect &object);

  QPoint startPos, curPos;
  QList<QRect> curObjects;
  QImage image, origImage;
  float zoomFactor;
  bool zoomSet;
};

B_END_NAMESPACE

#endif // B_IMAGE_WIDGET_H
