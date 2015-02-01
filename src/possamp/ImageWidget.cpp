// Copyright (c) Burator 2014-2015
#include <QLabel>
#include <QImage>
#include <QString>
#include <QPixmap>
#include <QPainter>
#include <QVBoxLayout>

#include "ImageWidget.h"

B_BEGIN_NAMESPACE

ImageWidget::ImageWidget() {
  setupLayout();
}

void ImageWidget::loadImage(const QString &path) {
  this->path = path;
  QPixmap pix(path);
  view->setPixmap(pix);
  resize(pix.size());
}

void ImageWidget::showObject(const QRect &object) {
  QPixmap pix(path);
  {
    QPainter painter(&pix);
    QPen pen = painter.pen();
    pen.setWidth(2);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawRect(object);
  }
  view->setPixmap(pix);
}

void ImageWidget::clear() {
  view->clear();
}

void ImageWidget::setupLayout() {
  view = new QLabel;
  
  auto *layout = new QVBoxLayout;
  layout->addWidget(view);
  setLayout(layout);
}

B_END_NAMESPACE
