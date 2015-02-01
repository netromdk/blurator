// Copyright (c) Burator 2014-2015
#ifndef B_MAIN_WINDOW_H
#define B_MAIN_WINDOW_H

#include <QMainWindow>

#include <memory>

#include "Global.h"
#include "ObjectManager.h"

B_BEGIN_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();

protected:
  void showEvent(QShowEvent *event);

private:
  void setupLayout();
  void askForFiles();

  QString samplesRoot;
  std::shared_ptr<ObjectManager> objMgr;
};

B_END_NAMESPACE

#endif // B_MAIN_WINDOW_H
