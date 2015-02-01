// Copyright (c) Burator 2014-2015
#include <QDir>
#include <QLabel>
#include <QDebug>
#include <QWidget>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QApplication>

#include "Util.h"
#include "MainWindow.h"

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

void MainWindow::setupLayout() {
  QLabel *l1 = new QLabel("IMAGE LIST"),
    *l2 = new QLabel("RECT LIST"),
    *l3 = new QLabel("IMAGE");

  QVBoxLayout *listLayout = new QVBoxLayout;
  listLayout->addWidget(l1);
  listLayout->addWidget(l2);
  
  QHBoxLayout *layout = new QHBoxLayout;
  layout->addLayout(listLayout);
  layout->addWidget(l3);

  QWidget *w = new QWidget;
  w->setLayout(layout);

  setCentralWidget(w);
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

  // Read and parse file.
  if (!isNew) {
    QMap<QString, QList<QRect> > results;
    if (!Util::parseDataFile(file, results)) {
      QMessageBox::warning(this, tr("Invalid data file"),
                           tr("Data file was either invalid or empty."));
      return;
    }
    qDebug() << "RESULTS:" << results;
  }
}

B_END_NAMESPACE
