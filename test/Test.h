#ifndef B_TEST_H
#define B_TEST_H

#define PASS 0
#define FAIL 1

/**
 * Constructor sets up a testing environment to use in test cases, and
 * desconstructor cleans up everything.
 */
class TestEnv {
public:
  TestEnv();
  ~TestEnv();
};

#endif // B_TEST_H
