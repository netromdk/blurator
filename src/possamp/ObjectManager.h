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

  void addObject(const QString &file, const QRect &object);
  void removeObject(const QString &file, const QRect &object);

  bool hasObjects() const { return !objects.isEmpty(); }
  const QMap<QString, QList<QRect> > &getObjects() const { return objects; }
  QList<QRect> getObjects(const QString &file) const;

  QString determineFolder(QString &subDir) const;

private:
  QString file;
  QMap<QString, QList<QRect> > objects;
};

B_END_NAMESPACE

#endif // B_OBJECT_MANAGER_H
