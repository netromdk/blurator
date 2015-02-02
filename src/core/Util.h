// Copyright (c) Burator 2014-2015
#ifndef B_UTIL_H
#define B_UTIL_H

#include <QRect>
#include <QString>
#include <QStringList>

#include "Types.h"
#include "Global.h"

class QImage;
class QString;
class QByteArray;

B_BEGIN_NAMESPACE

class Util {
public:
  static MatPtr imageToMat(const QString &file, bool loadAlpha = false);
  static MatPtr imageToMat(const QImage &image, const char *fmt = "PNG");
  static MatPtr imageToMat(const QByteArray &data, bool loadAlpha = false);
  static MatPtr imageToMat(const char *data, int len, bool loadAlpha = false);

  static bool matToImage(MatPtr image, QImage &out);

  static QRect toQRect(const cv::Rect &rect);
  static cv::Rect toCVRect(const QRect &rect);

  /**
   * Reads data from path and loads into a cv::FileStorage. Most
   * suitable for in-memory Qt resource files.
   */
  static FSPtr fileToFileStorage(const QString &path);

  /**
   * Show console prompt asking to proceed or not and it accepts "y",
   * "yes", "n", or "no" as reply. It returns false if anything else
   * is typed.
   *
   * If override is set then that will be the answer after outputting
   * the message.
   */
  static bool askProceed(const QString &msg, bool *override = nullptr);

  /**
   * Return path with .bak appended and if any backup file already
   * exists then it will be .bak2, .bak3 etc. instead.
   */
  static QString getBackupPath(const QString &path);

  /**
   * Checks whether the extension of the file is a supported image
   * file.
   */
  static bool isSupportedImage(const QString &path);

  /**
   * Formats a number of milliseconds in string format.
   */
  static QString formatTime(qint64 msecs);

  /**
   * Blur a region of an image with optional kernel size.
   */
  static void blurRegion(MatPtr image, cv::Rect region,
                         cv::Size ksize = cv::Size(50, 50));

  /**
   * Blur each face in an image.
   */
  static void blurFaces(MatPtr image, const std::vector<cv::Rect> &faces);

  /**
   * Represent rect as string.
   */
  static QString rectToStr(const QRect &rect);

  /**
   * Scales the rect by the factor.
   */
  static QRect scaleRect(const QRect &rect, float factor);

  /**
   * Search recursively for supported image files.
   */
  static QStringList getImages(const QString &folder);
};

B_END_NAMESPACE

#endif // B_UTIL_H
