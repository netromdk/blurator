// Copyright (c) Burator 2014-2015
#ifndef B_MAIN_WINDOW_H
#define B_MAIN_WINDOW_H

#include <QMainWindow>

#include <memory>

#include "Global.h"
#include "ObjectManager.h"

class QListWidget;

B_BEGIN_NAMESPACE

class ImageWidget;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();

protected:
  void showEvent(QShowEvent *event);

private slots:
  void onImageSelected();
  void onObjectSelected();
  void onNewObject(QRect object);
  void onSave();

private:
  void setupLayout();
  void setupMenu();
  void askForFiles();
  void loadItems();
  void loadObjects(const QString &file);
  void loadImage(const QString &path);

  QString root, samplesSub, samplesRoot, curFile;
  std::shared_ptr<ObjectManager> objMgr;

  QListWidget *imgList, *objList;
  ImageWidget *imgView;
};

B_END_NAMESPACE

#endif // B_MAIN_WINDOW_H
