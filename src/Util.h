// Copyright (c) Burator 2014-2015
#ifndef B_UTIL_H
#define B_UTIL_H

#include <QRect>
#include <QList>

#include "Face.h"
#include "Types.h"
#include "Global.h"

class QImage;
class QString;
class QByteArray;

B_BEGIN_NAMESPACE

class Util {
public:
  static MatPtr imageToMat(const QString &file, bool loadAlpha = false);
  static MatPtr imageToMat(const QImage &image, const char *fmt = "PNG");
  static MatPtr imageToMat(const QByteArray &data, bool loadAlpha = false);
  static MatPtr imageToMat(const char *data, int len, bool loadAlpha = false);

  static QRect toQRect(const cv::Rect &rect);
  static cv::Rect toCVRect(const QRect &rect);

  static void drawFaces(QImage &image, const QList<FacePtr> &faces);

  /**
   * Reads data from path and loads into a cv::FileStorage. Most
   * suitable for in-memory Qt resource files.
   */
  static FSPtr fileToFileStorage(const QString &path);

  /**
   * Show console prompt asking to proceed or not and it accepts "y",
   * "yes", "n", or "no" as reply. It returns false if anything else
   * is typed.
   */
  static bool askProceed(const QString &msg);
};

B_END_NAMESPACE

#endif // B_UTIL_H
