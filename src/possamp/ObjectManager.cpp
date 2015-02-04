#include <QDir>
#include <QFile>
#include <QDebug>
#include <QFileInfo>

#include "ObjectManager.h"

B_BEGIN_NAMESPACE

ObjectManager::ObjectManager(const QString &file) : file(file) {
  
}

bool ObjectManager::load() {
  objects.clear();
  
  if (!QFile::exists(file)) {
    qWarning() << "File does not exist:" << file;
    return false;
  }

  QFile f(file);
  if (!f.open(QIODevice::ReadOnly)) {
    qWarning() << "Could not open file for reading:" << file;
    return false;
  }

  while (true) {
    QString line = f.readLine();
    if (line.isEmpty()) break;

    QStringList items = line.trimmed().split(' ', QString::SkipEmptyParts);
    const int size = items.size();
    if (size <= 2 || (size - 2) % 4 != 0) {
      qWarning() << "Invalid line:" << line << (size - 2) % 4 << size;
      return false;
    }

    bool ok;
    int amount = items[1].toInt(&ok);
    if (!ok || amount <= 0) {
      qWarning() << "Invalid rect amount:" << amount << line;
      return false;
    }
    if ((size - 2) / 4 != amount) {
      qWarning() << "Invalid amount of rects found:" << amount << line;
      return false;
    }

    QList<QRect> rects;
    for (int i = 2; i < size; i += 4) {
      int data[4];
      bool ok;
      for (int j = 0; j < 4; j++) {
        const QString &val = items[i+j];
        data[j] = val.toInt(&ok);
        if (!ok || data[j] < 0) {
          qWarning() << "Invalid rect value:" << val << line;
          return false;
        }
      }

      rects << QRect(data[0], data[1], data[2], data[3]);
    }

    objects[items[0]] = rects;
  }
  
  return true;
}

bool ObjectManager::save() {
  QFile f(file);
  if (!f.open(QIODevice::WriteOnly)) {
    qWarning() << "Could not open file for writing:" << file;
    return false;
  }

  foreach (const QString &itm, objects.keys()) {
    const auto &rects = objects[itm];
    if (rects.size() == 0) {
      continue;
    }
    QString line = QString("%1 %2 ").arg(itm).arg(rects.size());
    foreach (const QRect &rect, rects) {
      line += QString("%1 %2 %3 %4 ")
        .arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
    }
    line += '\n';
    f.write(line.toUtf8());
  }
  
  return true;
}

void ObjectManager::addObject(const QString &file, const QRect &object) {
  auto &objs = objects[file];
  objs.append(object);
}

void ObjectManager::removeObject(const QString &file, const QRect &object) {
  auto &objs = objects[file];
  objs.removeAll(object);
}

QList<QRect> ObjectManager::getObjects(const QString &file) const {
  return objects[file];
}

QString ObjectManager::determineFolder(QString &subDir) const {
  if (!hasObjects()) return QString();
  QList<QString> flds;
  foreach (const QString &itm, objects.keys()) {
    QString fld = QFileInfo(itm).dir().path();
    if (!flds.contains(fld)) {
      flds << fld;
    }
  }
  QString root(".");
  if (flds.size() == 1) {
    root = flds[0];
  }
  QDir fileDir = QFileInfo(file).dir();
  subDir = root;
  if (fileDir.exists(root)) {
    return fileDir.absolutePath();
  }
  return fileDir.absoluteFilePath(root);
}

B_END_NAMESPACE
