#include <QResource>

#include "Test.h"

TestEnv::TestEnv() {
  // Initialize statically linked Qt resources.
  Q_INIT_RESOURCE(res);
}

TestEnv::~TestEnv() {
  Q_CLEANUP_RESOURCE(res);
}
