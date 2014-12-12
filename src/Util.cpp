#include <QFile>
#include <QImage>
#include <QString>
#include <QBuffer>
#include <QPainter>
#include <QByteArray>

#include <vector>

#include "Util.h"

B_BEGIN_NAMESPACE

MatPtr Util::imageToMat(const QString &file) {
  QFile f(file);
  if (!f.open(QIODevice::ReadOnly)) {
    return nullptr;
  }
  return imageToMat(f.readAll());
}

MatPtr Util::imageToMat(const QImage &image, const char *fmt) {
  QByteArray arr;
  QBuffer buf(&arr);
  buf.open(QIODevice::WriteOnly);
  image.save(&buf, fmt);
  return imageToMat(arr);
}

MatPtr Util::imageToMat(const QByteArray &data) {
  return imageToMat(data.constData(), data.size());
}

MatPtr Util::imageToMat(const char *data, int len) {
  if (!data || len <= 0) {
    return nullptr;
  }

  std::vector<char> vec(data, data + len);
  if (vec.size() == 0) {
    return nullptr;
  }

  auto mat = MatPtr(new cv::Mat);
  imdecode(cv::Mat(vec), CV_LOAD_IMAGE_COLOR, mat.get());
  return mat;
}

QRect Util::toQRect(const cv::Rect &rect) {
  return QRect(rect.x, rect.y, rect.width, rect.height);
}

void Util::drawFaces(QImage &image, const QList<FacePtr> &faces) {
  QPainter painter;
  painter.begin(&image);

  QPen pen = painter.pen();
  pen.setWidth(3);
  painter.setPen(pen);

  foreach (const auto &face, faces) {
    painter.drawRect(toQRect(face->getFace()));
    if (face->hasEyes()) {
      painter.drawRect(toQRect(face->getEye1()));
      painter.drawRect(toQRect(face->getEye2()));
    }
  }

  painter.end();
}

B_END_NAMESPACE
