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

#include <cmath>
#include <iostream>

#include "Util.h"
#include "Types.h"
#include "Global.h"
#include "Version.h"
#include "Detector.h"

B_USE_NAMESPACE

// Show console output.
static const bool showOutput = true;

// Show debug/info output.
static const bool showDebug = true;

void msgHandler(QtMsgType type, const QMessageLogContext &ctx,
                const QString &msg);

int main(int argc, char **argv) {
  // If "imageformats" is located locally then reset paths to be the
  // application folder. Otherwise, leave it with default values so it
  // finds the system plugins.
  QDir appDir =
    QDir::current().absoluteFilePath(QFileInfo(argv[0]).dir().absolutePath());
  if (appDir.exists("imageformats")) {
    QCoreApplication::setLibraryPaths(QStringList() << appDir.absolutePath());
  }

	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("Blurator");
	QCoreApplication::setApplicationVersion(versionString());

  qInstallMessageHandler(msgHandler);

  QCommandLineParser parser;
  parser.setApplicationDescription("Blur license plates and faces.\n\n"
                                   "Supported image formats: jpg, png, tiff");
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

  // Verbosity option
  QCommandLineOption verbosity(QStringList() << "verbose",
                               "Shows extra information.");
  parser.addOption(verbosity);
  
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
  bool verbose = parser.isSet(verbosity);
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
      QDir dir(path);
      QStringList files =
        dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
      foreach (const QString &file, files) {
        if (Util::isSupportedImage(file)) {
          images << dir.absoluteFilePath(file);
        }
      }
    }
  }

  if (images.isEmpty()) {
    qCritical() << "Found no supported images to process!";
    return -1;
  }

  if (noBackup) {
    qWarning() << "Warning no backup files will be created!";
  }

  Detector detector;
  if (!detector.isValid()) {
    qCritical() << "Could not setup detector.";
    return -1;
  }

  QDateTime startDate = QDateTime::currentDateTime();

  const int size = images.size();
  qint64 faceCnt = 0, faceTot = 0, plateCnt = 0, plateTot = 0;
  std::vector<cv::Rect> faces, plates;
  for (int i = 0; i < size; i++) {
    const QString &path = images[i];

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

    if (dFaces) {
      faces = detector.detectFaces(image);
      faceCnt = faces.size();
      faceTot += faceCnt;
    }

    if (dPlates) {
      plates = detector.detectPlates(image);
      plateCnt = plates.size();
      plateTot += plateCnt;
    }

    if (faceCnt > 0 || plateCnt > 0) {
      // Save original to backup file.
      if (!noBackup) {
        QString bpath = Util::getBackupPath(path);
        if (!QFile::copy(path, bpath)) {
          qWarning() << "Could not save backup:" << bpath;
          if (!Util::askProceed("Do you want to proceed?", autoReplyYes.get())) {
            qWarning() << "Aborting..";
            return -1;
          }
        }
      }

      // Blur each of the regions detected.
      Util::blurRegions(image, faces);
      Util::blurRegions(image, plates);

      QImage res;
      if (Util::matToImage(image, res)) {
        if (!res.save(path)) {
          qCritical() << "Could not save blurred image!";
        }
      }
      else {
        qCritical() << "Could not convert mat to image!";
      }
    }

    // Update progress information.
    static int lastLen = 0;
    float progress = float(i+1) / float(size) * 100.0;

    // Use term color codes when supported.
#ifndef WIN
    static const QString nw("\033[0;37m"); // normal and white
    static const QString bw("\033[1;37m"); // bold and white
#else
    static const QString nw, bw;
#endif

    QDateTime now = QDateTime::currentDateTime();
    qint64 elapsed = startDate.msecsTo(now);
    qint64 left = ((100.0 / progress) * elapsed) - elapsed;

    bool last = (i+1 == size);

    QString line1;
    if (verbose) {
      line1 = QString("Processed %1: %2 faces, %3 plates")
        .arg(path).arg(faceCnt).arg(plateCnt);
    }

    QString line2 = QString("[ %1%2%%3 (%4/%5) | %6blurred %7 faces, %8 plates%9 | %10 %11 ]")
      .arg(bw).arg(progress, 0, 'f', 1).arg(nw).arg(i+1).arg(size)
      .arg(bw).arg(faceTot).arg(plateTot).arg(nw)
      .arg(!last ? Util::formatTime(left) : Util::formatTime(elapsed))
      .arg(!last ? "left" : "elapsed");

    QString msg =
      QString("%1%2%3").arg(nw).arg(verbose ? QString("%1\n").arg(line1) : "").arg(line2);

    // Not Windows: Rewind to beginning and output message. If at second line or
    // latter then overwrite everything with empty spaces to "blank"
    // out so no traces are left if the new lines are longer than
    // the last ones.
    {
      using namespace std;
#ifndef WIN
      cout << '\r';
      if (i > 0) {
        cout << QString(lastLen, ' ').toStdString() << '\r';
      }
#else
      msg += "\r\n";
#endif
      cout << msg.toStdString();
      cout.flush();
    }

    lastLen = msg.size();
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
