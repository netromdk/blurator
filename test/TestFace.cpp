#include "Test.h"

#include "Face.h"
B_USE_NAMESPACE

int main() {
  Face f1;
  if (f1.hasFace() || f1.hasEyes()) {
    return FAIL;
  }

  Face f2(cv::Rect(0, 0, 80, 80));
  if (!f2.hasFace() || f2.hasEyes()) {
    return FAIL;
  }

  Face f3(cv::Rect(0, 0, 80, 80), cv::Rect(10, 10, 30, 30),
          cv::Rect(20, 20, 30, 30));
  if (!f3.hasFace() || !f3.hasEyes() || !f3.hasEye1() || !f3.hasEye2()) {
    return FAIL;
  }

  return PASS;
}
