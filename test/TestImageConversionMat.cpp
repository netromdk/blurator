#include <QImage>
#include <QDebug>

#include "Test.h"

#include "Util.h"
B_USE_NAMESPACE

int main() {
  TestEnv env;

  QImage image(256, 256, QImage::Format_RGB888);
  image.fill(Qt::red);

  MatPtr mat = Util::imageToMat(image);
  if (!mat) {
    qCritical() << "mat is null!";
    return FAIL;
  }

  QImage image2;
  if (!Util::matToImage(mat, image2)) {
    qCritical() << "Couldn't convert mat to image!";
    return FAIL;
  }

  if (image.size() != image2.size()) {
    qCritical() << "Size differs:" << image.size() << "vs." << image2.size();
    return FAIL;
  }

  for (int y = 0; y < image.height(); y++) {
    for (int x = 0; x < image.width(); x++) {
      if (image.pixel(x, y) != image2.pixel(x, y)) {
        qCritical() << "pixel different at (" << x << "," << y << ")";
        return FAIL;
      }
    }
  }

  return PASS;
}
