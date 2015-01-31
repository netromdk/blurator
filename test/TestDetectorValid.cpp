#include "Test.h"

#include "Detector.h"
B_USE_NAMESPACE

int main() {
  TestEnv env;

  Detector detector;
  return (detector.isValid() ? PASS : FAIL);
}
