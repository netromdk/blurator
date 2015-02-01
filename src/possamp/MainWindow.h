// Copyright (c) Burator 2014-2015
#ifndef B_MAIN_WINDOW_H
#define B_MAIN_WINDOW_H

#include <QMainWindow>

#include <memory>

#include "Global.h"
#include "ObjectManager.h"

class QLabel;
class QListWidget;

B_BEGIN_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();

protected:
  void showEvent(QShowEvent *event);

private slots:
  void onImageSelected();

private:
  void setupLayout();
  void askForFiles();
  void loadItems();
  void loadObjects(const QString &file);
  void loadImage(const QString &path);

  QString root, samplesSub, samplesRoot;
  std::shared_ptr<ObjectManager> objMgr;

  QListWidget *imgList, *objList;
  QLabel *imgView;
};

B_END_NAMESPACE

#endif // B_MAIN_WINDOW_H
