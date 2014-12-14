#include "Test.h"

#include "Util.h"
B_USE_NAMESPACE

int main() {
  // Position: (20, 30)
  // Size: 200 x 400
  QRect r1(20, 30, 200, 400);
  cv::Rect r2 = Util::toCVRect(r1);
  QRect r3 = Util::toQRect(r2);
  return (r3 == r1 ? PASS : FAIL);
}
