// Copyright (c) Burator 2014-2015
#include <QFile>
#include <QDebug>

#include "Util.h"
#include "Detector.h"

B_BEGIN_NAMESPACE

Detector::Detector() {
  valid = true;

  // Read data from Qt resources.
  try {
    auto fs = Util::fileToFileStorage(":res/faces.xml");
    if (!fs || !faceCas.read(fs->getFirstTopLevelNode())) {
      qCritical() << "Could not read faces data.";
      valid = false;
    }
  }
  catch (...) {
    qCritical() << "Invalid face cascade!";
    valid = false;
  }

  try {
    auto fs = Util::fileToFileStorage(":res/plates.xml");
    if (!fs || !plateCas.read(fs->getFirstTopLevelNode())) {
      qCritical() << "Could not read plates data.";
      valid = false;
    }
  }
  catch (...) {
    qCritical() << "Invalid plate cascade!";
    valid = false;
  }
}

std::vector<cv::Rect> Detector::detectFaces(const MatPtr image) {
  std::vector<cv::Rect> faces;
  if (!image) {
    return faces;
  }

  // Make grayscaled version of the image, and equalize the histogram
  // which normalizes the brightness and increases the contrast in the
  // image.
  cv::Mat grayImg;
  cvtColor(*image.get(), grayImg, cv::COLOR_BGR2GRAY);
  equalizeHist(grayImg, grayImg);

  // Detect faces with scale factor 1.1, minimum 3 neighbors and
  // minimum 80x80 face size.
  faceCas.detectMultiScale(grayImg, faces, 1.1, 3, 0, cv::Size(80, 80));

  return faces;
}

std::vector<cv::Rect> Detector::detectPlates(const MatPtr image) {
  std::vector<cv::Rect> plates;
  if (!image) {
    return plates;
  }

  cv::Mat grayImg;
  cvtColor(*image.get(), grayImg, cv::COLOR_BGR2GRAY);
  equalizeHist(grayImg, grayImg);

  // Detect plates with scale factor 1.1, minimum 3 neighbors and
  // minimum 80x80 plate size.
  plateCas.detectMultiScale(grayImg, plates, 1.1, 3, 0, cv::Size(80, 80));

  return plates;
}

B_END_NAMESPACE
