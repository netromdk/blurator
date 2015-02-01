// Copyright (c) Burator 2014-2015
#ifndef B_UTIL_H
#define B_UTIL_H

#include <QString>

#include "Global.h"

class QRect;

B_BEGIN_NAMESPACE

class Util {
public:
  static QString rectToStr(const QRect &rect);
};

B_END_NAMESPACE

#endif // B_UTIL_H
