// Copyright (c) Burator 2014-2015
#include <QFile>
#include <QImage>
#include <QDebug>
#include <QString>
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

static void msgHandler(QtMsgType type, const QMessageLogContext &ctx,
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
  parser.addPositionalArgument("path", "Path to images or image.");
  // Face detection option
  QCommandLineOption detectFaces(QStringList() << "f" << "faces",
                                 "Detect faces.");
  parser.addOption(detectFaces);

  // License plate detection option
  QCommandLineOption detectPlates(QStringList() << "p" << "plates",
                                  "Detect license plates.");
  parser.addOption(detectPlates);
  
  // No-backup file option
  QCommandLineOption noBackupFile(QStringList() << "no-backup",
                                 "Don't store a backup of the original image.");
  parser.addOption(noBackupFile);

  parser.process(app);
  const QStringList args = parser.positionalArguments();

  // Check optional command line options
  bool dFaces = parser.isSet(detectFaces);
  bool dPlates = parser.isSet(detectPlates);
  bool noBackup = parser.isSet(noBackupFile);

  // ensure the arguments are passed
  if (args.isEmpty()) {
  	qCritical() << "Must provide path to images!";
  	return -1;
  } 
  QString path = args.at(0);

  if (!dPlates && !dFaces) {
    qCritical() << "Must choose to detect plates and/or faces!";
    return -1;
  }

  if (noBackup) {
    qWarning() << "Warning no backup file will be created!";
  }

  if (dFaces) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
      qCritical() << "Could not read from image file!";
      return -1;
    }
    QByteArray imageData = f.readAll();
    f.close();

    qDebug() << "Read" << imageData.size() << "bytes";

    MatPtr image = Util::imageToMat(imageData);
    if (image == nullptr) {
      qCritical() << "Invalid image!";
      return -1;
    }
    qDebug() << "Loaded image..";

    FaceDetector detector;
    if (!detector.isValid()) {
      qCritical() << "Could not setup facial detector.";
      return -1;
    }

    qDebug() << "Detecting faces..";
    QList<FacePtr> faces = detector.detect(image);
    if (faces.isEmpty()) {
      qDebug() << "Did not find any faces!";
      return 0;
    }

    qDebug() << "Found" << faces.size() << "face(s).";
    qDebug() << faces;

    // Render face overlays to an image file to visualize the result.
    if (!faces.isEmpty()) {
      QImage overlay = QImage::fromData(imageData);
      Util::drawFaces(overlay, faces);

      // Save original to backup file.
      if (!noBackup) {
        QString bpath = path + ".bak";
        if (QFile::copy(path, bpath)) {
          qDebug() << "Saved backup to" << bpath;
        }
        else {
          qWarning() << "Could not save backup to" << bpath;
          if (!Util::askProceed("Do you want to proceed?")) {
            qWarning() << "Aborting..";
            return -1;
          }
        }
      }

      if (overlay.save(path)) {
        qDebug() << "Saved face overlays to" << path;
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

  return 0;
}

static void msgHandler(QtMsgType type, const QMessageLogContext &ctx,
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
      stream << "(I) " << msg << endl;
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
