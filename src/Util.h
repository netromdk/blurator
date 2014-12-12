#ifndef B_UTIL_H
#define B_UTIL_H

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
};

B_END_NAMESPACE

#endif // B_UTIL_H
