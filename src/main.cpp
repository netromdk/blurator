// Copyright (c) Burator 2014-2015
#include <QDir>
#include <QFile>
#include <QImage>
#include <QDebug>
#include <QString>
#include <QDateTime>
#include <QFileInfo>
#include <QByteArray>
#include <QStringList>
#include <QCoreApplication>
#include <QCommandLineParser>

#include "Util.h"
#include "Types.h"
#include "Global.h"
#include "Version.h"
#include "FaceDetector.h"

B_USE_NAMESPACE

// Show console output.
static const bool showOutput = true;

// Show debug/info output.
static const bool showDebug = true;

void msgHandler(QtMsgType type, const QMessageLogContext &ctx,
                const QString &msg);

int main(int argc, char **argv) {
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("Blurator");
	QCoreApplication::setApplicationVersion(versionString());

  qInstallMessageHandler(msgHandler);

  QCommandLineParser parser;
  parser.setApplicationDescription("Blur license plates and faces.");
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addPositionalArgument("paths", "Paths to images.", "paths..");

  // Face detection option
  QCommandLineOption detectFaces(QStringList() << "f" << "faces",
                                 "Detect faces.");
  parser.addOption(detectFaces);

  // License plate detection option
  QCommandLineOption detectPlates(QStringList() << "p" << "plates",
                                  "Detect license plates.");
  parser.addOption(detectPlates);

  // Reply "yes" to everything option
  QCommandLineOption replyYes(QStringList() << "y" << "yes",
                              "Automatically reply \"yes\" to all questions.");
  parser.addOption(replyYes);

  // Reply "no" to everything option
  QCommandLineOption replyNo(QStringList() << "n" << "no",
                             "Automatically reply \"no\" to all questions.");
  parser.addOption(replyNo);
  
  // No-backup file option
  QCommandLineOption noBackupFile(QStringList() << "no-backup",
                                 "Don't store a backup of the original image.");
  parser.addOption(noBackupFile);

  parser.process(app);
  const QStringList args = parser.positionalArguments();

  // Check optional command line options
  bool dFaces = parser.isSet(detectFaces);
  bool dPlates = parser.isSet(detectPlates);
  bool rYes = parser.isSet(replyYes);
  bool rNo = parser.isSet(replyNo);
  bool noBackup = parser.isSet(noBackupFile);

  // Ensure the arguments are passed.
  if (args.isEmpty()) {
  	qCritical() << "Must provide paths to images!";
  	return -1;
  }

  if (!dPlates && !dFaces) {
    qCritical() << "Must choose to detect plates and/or faces!";
    return -1;
  }

  if (rYes && rNo) {
    qCritical() << "Can't both say yes and no to everything!";
    return -1;
  }

  // If non-null then the boolean value means reply "yes" for true and
  // "no" for false.
  BoolPtr autoReplyYes(nullptr);
  if (rYes || rNo) {
    autoReplyYes.reset(new bool);
    *autoReplyYes = rYes;
  }

  // Find all images from the arguments.
  QStringList images;
  foreach (const QString &path, args) {
    QFileInfo fi(path);
    if (!fi.exists()) {
      qWarning() << "Ignoring nonexistent file:" << path;
      continue;
    }

    if (fi.isFile() && Util::isSupportedImage(path)) {
      images << path;
    }
    else if (fi.isDir()) {
      qWarning() << "Ignoring folder:" << path;
    }
  }

  if (images.isEmpty()) {
    qCritical() << "Found no supported images to process!";
    return -1;
  }

  if (noBackup) {
    qWarning() << "Warning no backup files will be created!";
  }

  QDateTime startDate = QDateTime::currentDateTime();

  const int size = images.size();
  for (int i = 0; i < size; i++) {
    const QString &path = images[i];
    QString msg = QString("%1%2")
      .arg(size > 1 ? QString("[%1/%2] ").arg(i+1).arg(size) : "")
      .arg("Processing");
    qDebug() << qPrintable(msg) << path;

    if (dFaces) {
      QFile f(path);
      if (!f.open(QIODevice::ReadOnly)) {
        qCritical() << "Could not read from image file!";
        return -1;
      }
      QByteArray imageData = f.readAll();
      f.close();

      MatPtr image = Util::imageToMat(imageData);
      if (image == nullptr) {
        qCritical() << "Invalid image!";
        return -1;
      }

      FaceDetector detector;
      if (!detector.isValid()) {
        qCritical() << "Could not setup facial detector.";
        return -1;
      }

      QList<FacePtr> faces = detector.detect(image);
      if (faces.isEmpty()) {
        continue;
      }

      qDebug() << "Found" << faces.size() << "face(s).";
      qDebug() << faces;

      // Render face overlays to an image file to visualize the result.
      if (!faces.isEmpty()) {
        QImage overlay = QImage::fromData(imageData);
        Util::drawFaces(overlay, faces);

        // Save original to backup file.
        if (!noBackup) {
          QString bpath = Util::getBackupPath(path);
          if (QFile::copy(path, bpath)) {
            qDebug() << "Saved backup:" << bpath;
          }
          else {
            qWarning() << "Could not save backup:" << bpath;
            if (!Util::askProceed("Do you want to proceed?", autoReplyYes.get())) {
              qWarning() << "Aborting..";
              return -1;
            }
          }
        }

        if (overlay.save(path)) {
          qDebug() << "Saved face overlays:" << path;
        }
        else {
          qCritical() << "Could not save overlays";
        }
      }
    }

    // TODO: Plate detection
    if (dPlates) {
      qCritical() << "Plate detection not implemented yet!";
      return -1;
    }
  }

  qint64 msecs = startDate.msecsTo(QDateTime::currentDateTime());
  qDebug() << endl << "Time elapsed:" << qPrintable(Util::formatTime(msecs));
  if (size > 1) {
    qDebug() << "Time per image:" << qPrintable(Util::formatTime(msecs / size));
  }

  return 0;
}

void msgHandler(QtMsgType type, const QMessageLogContext &ctx,
                const QString &msg) {
  if (!showOutput) return;

  QTextStream stream(stdout);

  switch (type) {
  case QtDebugMsg:
    if (showDebug) {
      if (QString(msg) == "") {
        stream << endl;
        return;
      }
      stream << msg << endl;
    }
    break;

  case QtWarningMsg:
    stream << "(W) " << msg << endl;
    break;

  case QtCriticalMsg:
    stream << "(E) " << msg << endl;
    break;

  case QtFatalMsg:
    stream << "(F) " << msg << endl;
    abort();
  }
}
