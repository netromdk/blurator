// Copyright (c) Burator 2014-2015
#ifndef B_OBJECT_MANAGER_H
#define B_OBJECT_MANAGER_H

#include <QMap>
#include <QList>
#include <QRect>
#include <QString>

#include "Global.h"

B_BEGIN_NAMESPACE

class ObjectManager {
public:
  ObjectManager(const QString &file);

  bool load();
  bool save();

  const QMap<QString, QList<QRect> > &getObjects() const { return objects; }

private:
  QString file;
  QMap<QString, QList<QRect> > objects;
};

B_END_NAMESPACE

#endif // B_OBJECT_MANAGER_H
