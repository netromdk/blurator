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

  if (mat->channels() != 3) {
    qCritical() << "mat must have three channels, has:" << mat->channels();
    return FAIL;
  }

  size_t size = image.width() * image.height();
  if (mat->total() != size) {
    qCritical() << "mat size must be:" << size << ", but is:" << mat->total();
    return FAIL;
  }  
  
  return PASS;
}
