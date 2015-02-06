// Copyright (c) Burator 2014-2015
#ifndef B_VERSION_H
#define B_VERSION_H

#include <QString>

#include "Global.h"

B_BEGIN_NAMESPACE

#define MAJOR_VERSION 0
#define MINOR_VERSION 0
#define BUILD_VERSION 1

static QString versionString() {
  return QString("%1.%2.%3")
    .arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(BUILD_VERSION);
}

B_END_NAMESPACE

#endif // B_VERSION_H
