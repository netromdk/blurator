#include <QImage>

#include "Test.h"

#include "Util.h"
#include "Detector.h"
B_USE_NAMESPACE

int main() {
  TestEnv env;

  Detector detector;
  if (!detector.isValid()) {
    qCritical() << "Detector is not valid!";
    return FAIL;
  }

  QImage image(256, 256, QImage::Format_RGB888);
  MatPtr mat = Util::imageToMat(image);
  if (!mat) {
    qCritical() << "mat is null!";
    return FAIL;
  }

  auto faces = detector.detectFaces(mat);
  return (faces.empty() ? PASS : FAIL);
}
