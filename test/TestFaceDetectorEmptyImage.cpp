#include "Test.h"

#include "Detector.h"
B_USE_NAMESPACE

int main() {
  TestEnv env;

  Detector detector;
  if (!detector.isValid()) {
    qCritical() << "Detector is not valid!";
    return FAIL;
  }

  auto faces = detector.detectFaces(nullptr);
  return (faces.empty() ? PASS : FAIL);
}
