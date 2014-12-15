#include "Test.h"

#include "FaceDetector.h"
B_USE_NAMESPACE

int main() {
  TestEnv env;

  FaceDetector detector;
  if (!detector.isValid()) {
    qCritical() << "Detector is not valid!";
    return FAIL;
  }

  auto faces = detector.detect(nullptr);
  return (faces.isEmpty() ? PASS : FAIL);
}
