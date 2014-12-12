#include <QFile>
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

int main(int argc, char **argv) {
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("Blurator");
	QCoreApplication::setApplicationVersion(versionString());

  QCommandLineParser parser;
  parser.setApplicationDescription("Blur license plates and faces");
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addPositionalArgument("path", QCoreApplication::translate("main",
  	                        "Path to images or image"));

  parser.process(app);
  const QStringList args = parser.positionalArguments();

  // ensure the arguments are passed
  if (args.isEmpty()) {
  	qCritical() << "Must provide path to images!";
  	return -1;
  } 
  QString path = args.at(0);


  /* Testing */
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

  QString binPath = QCoreApplication::applicationDirPath();
  QString faceFile = binPath + "/../../data/lbpcascade_frontalface.xml",
    eyesFile = binPath + "/../../data/haarcascade_eye_tree_eyeglasses.xml";

  FaceDetector detector(faceFile, eyesFile);
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
  return 0;
}
