#include <QDebug>

#include "Test.h"

#include "Face.h"
B_USE_NAMESPACE

int main() {
  Face f1;
  if (f1.hasFace() || f1.hasEyes()) {
    qCritical() << "f1 cannot have face or eyes!";
    return FAIL;
  }

  Face f2(cv::Rect(0, 0, 80, 80));
  if (!f2.hasFace() || f2.hasEyes()) {
    qCritical() << "f2 must face and not eyes!";
    return FAIL;
  }

  Face f3(cv::Rect(0, 0, 80, 80), cv::Rect(10, 10, 30, 30),
          cv::Rect(20, 20, 30, 30));
  if (!f3.hasFace() || !f3.hasEyes() || !f3.hasEye1() || !f3.hasEye2()) {
    qCritical() << "f3 must face and both eyes!";
    return FAIL;
  }

  return PASS;
}
