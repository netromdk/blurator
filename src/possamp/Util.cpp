#include <QRect>

#include "Util.h"

B_BEGIN_NAMESPACE

QString Util::rectToStr(const QRect &rect) {
  return QString("(%1, %2) %3x%4")
    .arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
}

B_END_NAMESPACE
