// Copyright (c) Burator 2014-2015
#include <QTimer>
#include <QApplication>

#include "Global.h"
#include "Version.h"
#include "MainWindow.h"

B_USE_NAMESPACE

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  QApplication::setApplicationName("possamp");
  QApplication::setApplicationVersion(versionString());

  // Start in event loop.
  MainWindow main;
  QTimer::singleShot(0, &main, SLOT(show()));

  return app.exec();
}
