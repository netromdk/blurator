// Copyright (c) Burator 2014-2015
#ifndef B_IMAGE_WIDGET_H
#define B_IMAGE_WIDGET_H

#include <QString>
#include <QWidget>

#include "Global.h"

class QRect;
class QLabel;

B_BEGIN_NAMESPACE

class ImageWidget : public QWidget {
public:
  ImageWidget();
  
  void loadImage(const QString &path);
  void showObject(const QRect &object);
  void clear();

private:
  void setupLayout();

  QString path;
  QLabel *view;
};

B_END_NAMESPACE

#endif // B_IMAGE_WIDGET_H
