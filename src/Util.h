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
  static MatPtr imageToMat(const QString &file);
  static MatPtr imageToMat(const QImage &image, const char *fmt = "PNG");
  static MatPtr imageToMat(const QByteArray &data);
  static MatPtr imageToMat(const char *data, int len);

  static QRect toQRect(const cv::Rect &rect);
  static cv::Rect toCVRect(const QRect &rect);

  static void drawFaces(QImage &image, const QList<FacePtr> &faces);

  /**
   * Reads data from path and loads into a cv::FileStorage. Most
   * suitable for in-memory Qt resource files.
   */
  static FSPtr fileToFileStorage(const QString &path);
};

B_END_NAMESPACE

#endif // B_UTIL_H
