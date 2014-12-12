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

  static void drawFaces(QImage &image, const QList<FacePtr> &faces);
};

B_END_NAMESPACE

#endif // B_UTIL_H
