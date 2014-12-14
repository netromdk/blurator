#include "FaceDetector.h"
B_USE_NAMESPACE

int main() {
  Q_INIT_RESOURCE(res);

  FaceDetector detector;
  return (detector.isValid() ? 0 : 1);
}
