// Copyright (c) Burator 2014-2015
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "MainWindow.h"

B_BEGIN_NAMESPACE

MainWindow::MainWindow() {
  setWindowTitle(tr("Positive Samples Preparation Tool"));
  setupLayout();
  resize(800, 500);
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

B_END_NAMESPACE
