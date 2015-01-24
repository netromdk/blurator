// Copyright (c) Burator 2014-2015
#include <QFile>
#include <QImage>
#include <QString>
#include <QBuffer>
#include <QPainter>
#include <QByteArray>

#include <vector>
#include <string>
#include <iostream>

#include "Util.h"

B_BEGIN_NAMESPACE

MatPtr Util::imageToMat(const QString &file, bool loadAlpha) {
  QFile f(file);
  if (!f.open(QIODevice::ReadOnly)) {
    return nullptr;
  }
  return imageToMat(f.readAll(), loadAlpha);
}

MatPtr Util::imageToMat(const QImage &image, const char *fmt) {
  QByteArray arr;
  QBuffer buf(&arr);
  buf.open(QIODevice::WriteOnly);
  image.save(&buf, fmt);
  return imageToMat(arr, image.hasAlphaChannel());
}

MatPtr Util::imageToMat(const QByteArray &data, bool loadAlpha) {
  return imageToMat(data.constData(), data.size(), loadAlpha);
}

MatPtr Util::imageToMat(const char *data, int len, bool loadAlpha) {
  if (!data || len <= 0) {
    return nullptr;
  }

  std::vector<char> vec(data, data + len);
  if (vec.size() == 0) {
    return nullptr;
  }

  // If the flag value is <0 then it will load the image with alpha
  // channel. If it is >0 then it will load as 3-channel image.
  auto mat = MatPtr(new cv::Mat);
  imdecode(cv::Mat(vec), CV_LOAD_IMAGE_COLOR * (loadAlpha ? -1 : 1), mat.get());
  return mat;
}

QRect Util::toQRect(const cv::Rect &rect) {
  return QRect(rect.x, rect.y, rect.width, rect.height);
}

cv::Rect Util::toCVRect(const QRect &rect) {
  return cv::Rect(rect.x(), rect.y(), rect.width(), rect.height());
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

FSPtr Util::fileToFileStorage(const QString &path) {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly)) {
    return NULL;
  }
  QString data = file.readAll();
  auto fs =
    new cv::FileStorage(data.toStdString(),
                        cv::FileStorage::READ | cv::FileStorage::MEMORY);
  return FSPtr(fs);
}

bool Util::askProceed(const QString &msg, bool *override) {
  using namespace std;
  cout << msg.toStdString() << " [y/N] ";
  cout.flush();

  if (override) {
    cout << (*override ? "yes" : "no") << endl;
    return *override;
  }

  string _r;
  cin >> _r;
  QString reply(_r.c_str());
  reply = reply.toLower().trimmed();
  if (reply == "y" || reply == "yes") {
    return true;
  }
  return false;
}

QString Util::getBackupPath(const QString &path) {
  for (int num = 1;; num++) {
    QString bpath =
      QString("%1.bak%2").arg(path).arg(num > 1 ? QString::number(num) : "");
    if (!QFile::exists(bpath)) {
      return bpath;
    }
  }
  return QString();
}

B_END_NAMESPACE
