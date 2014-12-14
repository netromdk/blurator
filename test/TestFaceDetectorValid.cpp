#include "Test.h"

#include "FaceDetector.h"
B_USE_NAMESPACE

int main() {
  TestEnv env;

  FaceDetector detector;
  return (detector.isValid() ? PASS : FAIL);
}
