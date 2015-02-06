// Copyright (c) Burator 2014-2015
#include <QFile>
#include <QImage>
#include <QString>
#include <QBuffer>
#include <QFileInfo>
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

bool Util::matToImage(MatPtr image, QImage &out) {
  std::vector<unsigned char> buf;
  if (!cv::imencode(".png", *image, buf)) {
    return false;
  }
  return out.loadFromData(buf.data(), buf.size());
}

QRect Util::toQRect(const cv::Rect &rect) {
  return QRect(rect.x, rect.y, rect.width, rect.height);
}

cv::Rect Util::toCVRect(const QRect &rect) {
  return cv::Rect(rect.x(), rect.y(), rect.width(), rect.height());
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

bool Util::isSupportedImage(const QString &path) {
  static const QStringList exts =
    QStringList{"jpg", "jpeg", "png", "tif", "tiff"};
  return exts.contains(QFileInfo(path).suffix().toLower());
}

QString Util::formatTime(qint64 msecs) {
  constexpr qint64 Second = 1000, Minute = Second * 60, Hour = Minute * 60,
    Day = Hour * 24, Week = Day * 7;
  QString res;
  if (msecs >= Week) {
    qint64 weeks = msecs / Week;
    res += QString("%1w").arg(weeks);
    msecs -= weeks * Week;
  }
  if (msecs >= Day) {
    qint64 days = msecs / Day;
    res += QString("%1%2d").arg(!res.isEmpty() ? " " : "").arg(days);
    msecs -= days * Day;
  }
  if (msecs >= Hour) {
    qint64 hours = msecs / Hour;
    res += QString("%1%2h").arg(!res.isEmpty() ? " " : "").arg(hours);
    msecs -= hours * Hour;
  }
  if (msecs >= Minute) {
    qint64 minutes = msecs / Minute;
    res += QString("%1%2m").arg(!res.isEmpty() ? " " : "").arg(minutes);
    msecs -= minutes * Minute;
  }
  if (msecs >= Second) {
    qint64 seconds = msecs / Second;
    res += QString("%1%2s").arg(!res.isEmpty() ? " " : "").arg(seconds);
    msecs -= seconds * Second;
  }
  if (msecs > 0 || res.isEmpty()) {
    res += QString("%1%2ms").arg(!res.isEmpty() ? " " : "").arg(msecs);
  }
  return res;
}

void Util::blurRegion(MatPtr image, cv::Rect region, cv::Size ksize) {
  cv::Mat sub = cv::Mat(*image, region);
  cv::blur(sub, sub, ksize);
}

void Util::blurFaces(MatPtr image, const std::vector<cv::Rect> &faces) {
  foreach (const cv::Rect &face, faces) {
    blurRegion(image, face);
  }
}

B_END_NAMESPACE
