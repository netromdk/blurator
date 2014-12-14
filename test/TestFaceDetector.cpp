#include "FaceDetector.h"
B_USE_NAMESPACE

int main() {
  FaceDetector detector;
  return (detector.isValid() ? 0 : 1);
}
