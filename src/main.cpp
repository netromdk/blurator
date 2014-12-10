#include <QDebug>
#include <QString>
#include <QStringList>
#include <QCoreApplication>
#include <QCommandLineParser>

#include "Version.h"
using namespace burator::blurator;

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
  	qDebug() << "Must provide path to images!";
  	return 0;
  } 
  QString path = args.at(0);

  // use path...
  return 0;
}
