// Copyright (c) Burator 2014-2015
#include <QDir>
#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QMenuBar>
#include <QKeyEvent>
#include <QComboBox>
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
  setupMenu();
  resize(800, 500);
}

void MainWindow::showEvent(QShowEvent *event) {
  static bool first = true;
  if (first) {
    first = false;
    askForFiles();
  }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
  if (obj == objList && event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
    switch (keyEvent->key()) {
    case Qt::Key_Backspace:
    case Qt::Key_Delete:
      onRemoveCurrentObject();
      return true;
    }
  }

  return QObject::eventFilter(obj, event);
}

void MainWindow::onImageSelected() {
  QListWidgetItem *itm = imgList->currentItem();
  if (!itm) return;

  curFile = itm->text();
  loadObjects(curFile);

  QString path = QDir(root).absoluteFilePath(curFile);
  loadImage(path);
}

void MainWindow::onObjectSelected() {
  auto items = objList->selectedItems();
  if (items.isEmpty()) return;

  QList<QRect> objs;
  foreach (auto *item, items) {
    objs << item->data(Qt::UserRole).toRect();
  }
  imgView->showObjects(objs);
}

void MainWindow::onNewObject(QRect object) {
  objMgr->addObject(curFile, object);
  loadObjects(curFile);
  objList->setCurrentRow(objList->count() - 1);
}

void MainWindow::onSave() {
  qDebug() << "Saved:" << objMgr->save();
}

void MainWindow::onRemoveCurrentObject() {
  auto items = objList->selectedItems();
  if (items.isEmpty()) return;

  foreach (auto *item, items) {
    objMgr->removeObject(curFile, item->data(Qt::UserRole).toRect());
  }
  loadObjects(curFile);
  imgView->clear();
}

void MainWindow::onZoomChanged(int index) {
  imgView->setZoom(zoomBox->itemData(index).toFloat());
}

void MainWindow::setupLayout() {
  const int listWidth = 200;

  imgList = new QListWidget;
  imgList->setFixedWidth(listWidth);
  connect(imgList, &QListWidget::itemSelectionChanged,
          this, &MainWindow::onImageSelected);

  objList = new QListWidget;
  objList->setFixedWidth(listWidth);
  objList->setSelectionMode(QAbstractItemView::ExtendedSelection);
  objList->installEventFilter(this);
  connect(objList, &QListWidget::itemSelectionChanged,
          this, &MainWindow::onObjectSelected);

  imgView = new ImageWidget;
  connect(imgView, &ImageWidget::newObject, this, &MainWindow::onNewObject);

  auto *scrollArea = new QScrollArea;
  scrollArea->setBackgroundRole(QPalette::Dark);
  scrollArea->setWidget(imgView);

  zoomBox = new QComboBox;
  for (int i = 25; i <= 100; i += 25) {
    zoomBox->addItem(QString("%1%").arg(i), float(i) / 100.0);
  }
  for (int i = 200; i <= 800; i *= 2) {
    zoomBox->addItem(QString("%1%").arg(i), float(i) / 100.0);
  }
  zoomBox->setCurrentIndex(3); // 100 %
  connect(zoomBox, SIGNAL(currentIndexChanged(int)),
          this, SLOT(onZoomChanged(int)));

  QVBoxLayout *listLayout = new QVBoxLayout;
  listLayout->addWidget(new QLabel(tr("Images")));
  listLayout->addWidget(imgList);
  listLayout->addWidget(new QLabel(tr("Objects")));
  listLayout->addWidget(objList);

  QHBoxLayout *zoomLayout = new QHBoxLayout;
  zoomLayout->addWidget(new QLabel(tr("Zoom")+":"));
  zoomLayout->addWidget(zoomBox);
  zoomLayout->addStretch();

  QVBoxLayout *imageLayout = new QVBoxLayout;
  imageLayout->addLayout(zoomLayout);
  imageLayout->addWidget(scrollArea);
  
  QHBoxLayout *layout = new QHBoxLayout;
  layout->addLayout(listLayout);
  layout->addLayout(imageLayout);

  QWidget *w = new QWidget;
  w->setLayout(layout);

  setCentralWidget(w);
}

void MainWindow::setupMenu() {
  auto *fileMenu = menuBar()->addMenu(tr("File"));
  fileMenu->addAction(tr("Save"), this, SLOT(onSave()), QKeySequence::Save);
}

void MainWindow::askForFiles() {
  QString file =
    QFileDialog::getOpenFileName(this, tr("Choose data file"), QDir::homePath(),
                                 "*.dat");

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
