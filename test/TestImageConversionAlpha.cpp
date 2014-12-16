#include <QImage>
#include <QDebug>

#include "Test.h"

#include "Util.h"
B_USE_NAMESPACE

int main() {
  TestEnv env;

  QImage image(256, 256, QImage::Format_ARGB32);
  image.fill(qRgba(255, 0, 0, 126));

  MatPtr mat = Util::imageToMat(image);
  if (!mat) {
    qCritical() << "mat is null!";
    return FAIL;
  }

  int channels = mat->channels();
  if (channels != 4) {
    qCritical() << "mat must have four channels, has:" << channels;
    return FAIL;
  }

  size_t size = image.width() * image.height();
  if (mat->total() != size) {
    qCritical() << "mat size must be:" << size << ", but is:" << mat->total();
    return FAIL;
  }

  // Check all pixels are red with transparency.
  for (int y = 0; y < image.height(); y++) {
    for (int x = 0; x < image.width(); x++) {
      auto pixels = mat->at<cv::Vec4b>(y, x); // In BGRA format.
      if (pixels[2] != 255 && pixels[1] != 0 && pixels[0] != 0) {
        qDebug() << pixels[0] << pixels[1] << pixels[2] << pixels[3];
        qCritical() << "mat pixel (" << x << "," << y
                    << ") is not (255, 0, 0) RGB!";
        return FAIL;
      }
    }
  }

  return PASS;
}
