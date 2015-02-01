// Copyright (c) Burator 2014-2015
#ifndef B_UTIL_H
#define B_UTIL_H

#include <QMap>
#include <QList>
#include <QRect>
#include <QString>

#include "Global.h"

B_BEGIN_NAMESPACE

class Util {
public:
  static bool parseDataFile(const QString &file,
                            QMap<QString, QList<QRect> > &results);
  static bool saveDataFile(const QString &file,
                           const QMap<QString, QList<QRect> > &results);
};

B_END_NAMESPACE

#endif // B_UTIL_H
