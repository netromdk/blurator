// Copyright (c) Burator 2014-2015
#include <QDir>
#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QScrollArea>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QListWidget>
#include <QApplication>

#include "Util.h"
#include "MainWindow.h"
#include "ImageWidget.h"

B_BEGIN_NAMESPACE

MainWindow::MainWindow() {
  setWindowTitle(tr("Positive Samples Preparation Tool"));
  setupLayout();
  resize(800, 500);
}

void MainWindow::showEvent(QShowEvent *event) {
  static bool first = true;
  if (first) {
    first = false;
    askForFiles();
  }
}

void MainWindow::onImageSelected() {
  QListWidgetItem *itm = imgList->currentItem();
  if (!itm) return;

  QString file = itm->text();
  loadObjects(file);

  QString path = QDir(root).absoluteFilePath(file);
  loadImage(path);
}

void MainWindow::onObjectSelected() {
  QListWidgetItem *itm = objList->currentItem();
  if (!itm) return;

  QRect object = itm->data(Qt::UserRole).toRect();
  imgView->showObject(object);
}

void MainWindow::setupLayout() {
  const int listWidth = 200;

  imgList = new QListWidget;
  imgList->setFixedWidth(listWidth);
  connect(imgList, &QListWidget::itemSelectionChanged,
          this, &MainWindow::onImageSelected);

  objList = new QListWidget;
  objList->setFixedWidth(listWidth);
  connect(objList, &QListWidget::itemSelectionChanged,
          this, &MainWindow::onObjectSelected);

  imgView = new ImageWidget;

  auto *scrollArea = new QScrollArea;
  scrollArea->setBackgroundRole(QPalette::Dark);
  scrollArea->setWidget(imgView);

  QVBoxLayout *listLayout = new QVBoxLayout;
  listLayout->addWidget(new QLabel(tr("Images")));
  listLayout->addWidget(imgList);
  listLayout->addWidget(new QLabel(tr("Objects")));
  listLayout->addWidget(objList);
  
  QHBoxLayout *layout = new QHBoxLayout;
  layout->addLayout(listLayout);
  layout->addWidget(scrollArea);

  QWidget *w = new QWidget;
  w->setLayout(layout);

  setCentralWidget(w);
}

void MainWindow::askForFiles() {
  QString file =
    "/Users/netrom/Downloads/face_training/face.dat";
    /*
    QFileDialog::getOpenFileName(this, tr("Choose data file"), QDir::homePath(),
                                 "*.dat");
    */

  // If cancelled then ask to quit or create new file.
  bool isNew = false;
  if (file.isEmpty()) {
    QMessageBox::StandardButton res =
      QMessageBox::question(this, tr("What to do?"),
                            tr("Do you want to create a new data file or quit?"));
    if (res == QMessageBox::No) {
      QApplication::quit();
      return;
    }

    file = QFileDialog::getSaveFileName(this, tr("New data file"),
                                        QDir::homePath(), "*.dat");
    isNew = true;
    if (file.isEmpty()) {
      QApplication::quit();
      return;
    }
  }

  objMgr.reset(new ObjectManager(file));

  // Read and parse file.
  if (!isNew) {
    if (!objMgr->load()) {
      QMessageBox::warning(this, tr("Invalid data file"),
                           tr("Data file was either invalid or empty."));
      return;
    }

    qDebug() << "RESULTS:" << objMgr->getObjects();
  }

  root = objMgr->determineFolder(samplesSub);
  samplesRoot = QDir(root).absoluteFilePath(samplesSub);
    
  if (samplesRoot.isEmpty()) {
    samplesRoot =
      QFileDialog::getExistingDirectory(this, tr("Choose samples image root"),
                                        QDir::homePath());
    if (samplesRoot.isEmpty()) {
      QApplication::quit();
      return;
    }
  }
  qDebug() << "SAMPLES ROOT FOLDER:" << samplesRoot;

  loadItems();
}

void MainWindow::loadItems() {
  imgList->clear();
  objList->clear();

  foreach (const QString &file, Util::getImages(samplesRoot)) {
    if (samplesSub.isEmpty()) {
      imgList->addItem(file);
    }
    else {
      imgList->addItem(QString("%1/%2").arg(samplesSub).arg(file));
    }
  }  
}

void MainWindow::loadObjects(const QString &file) {
  objList->clear();

  foreach (const QRect &obj, objMgr->getObjects(file)) {
    QListWidgetItem *itm = new QListWidgetItem(Util::rectToStr(obj));
    itm->setData(Qt::UserRole, obj);
    objList->addItem(itm);
  }
}

void MainWindow::loadImage(const QString &path) {
  imgView->clear();
  if (!QFile::exists(path)) {
    QMessageBox::warning(this, tr("Error"), tr("File does not exist!"));
    return;
  }
  imgView->loadImage(path);
}

B_END_NAMESPACE
