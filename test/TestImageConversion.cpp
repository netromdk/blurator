#include <QImage>

#include "Test.h"

#include "Util.h"
B_USE_NAMESPACE

int main() {
  TestEnv env;

  QImage image(256, 256, QImage::Format_RGB888);
  image.fill(Qt::red);

  MatPtr mat = Util::imageToMat(image);
  if (!mat) {
    return FAIL;
  }

  if (mat->channels() != 3) {
    return FAIL;
  }

  if (mat->total() != image.width() * image.height()) {
    return FAIL;
  }  
  
  return PASS;
}
