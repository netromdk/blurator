// Copyright (c) Burator 2014-2015
#include <QFile>
#include <QDebug>

#include <vector>

#include "Util.h"
#include "FaceDetector.h"

B_BEGIN_NAMESPACE

bool FaceDetector::setup() {
  bool valid = true;

  // Read data from Qt resources.
  try {
    auto fs = Util::fileToFileStorage(":res/face.xml");
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
    auto fs = Util::fileToFileStorage(":res/profileface.xml");
    if (!fs || !pfaceCas.read(fs->getFirstTopLevelNode())) {
      qCritical() << "Could not read profile faces data.";
      valid = false;
    }
  }
  catch (...) {
    qCritical() << "Invalid profile face cascade!";
    valid = false;
  }

  return valid;
}

QList<FacePtr> FaceDetector::detect(const MatPtr image) {
  QList<FacePtr> results;
  if (!image) {
    return results;
  }

  // Make grayscaled version of the image, and equalize the histogram
  // which normalizes the brightness and increases the contrast in the
  // image.
  cv::Mat grayImg;
  cvtColor(*image.get(), grayImg, cv::COLOR_BGR2GRAY);
  equalizeHist(grayImg, grayImg);

  // Detect faces with scale factor 1.1, minimum 3 neighbors and
  // minimum 80x80 face size.
  std::vector<cv::Rect> faces;
  faceCas.detectMultiScale(grayImg, faces, 1.1, 3, 0, cv::Size(80, 80));
  results.append(createFaces(faces, grayImg));
  faces.clear();

  // Detect profile faces with the left side visible.
  pfaceCas.detectMultiScale(grayImg, faces, 1.1, 3, 0, cv::Size(80, 80));
  results.append(createFaces(faces, grayImg));
  faces.clear();

  // Flip image vertically to detect the right side of faces.
  cv::flip(grayImg, grayImg, 1);
  pfaceCas.detectMultiScale(grayImg, faces, 1.1, 3, 0, cv::Size(80, 80));
  results.append(createFaces(faces, grayImg, true));

  grayImg.release();
  return results;
}

QList<FacePtr> FaceDetector::createFaces(std::vector<cv::Rect> &faces,
                                         const cv::Mat &image, bool vflip) {
  cv::Size imageSize = image.size();
  QList<FacePtr> results;
  for (auto it = faces.begin(); it != faces.end(); ++it) {
    auto &f = *it;

    // Flip vertically.
    if (vflip) {
      f.x = imageSize.width - f.width - f.x;
    }

    auto face = FacePtr(new Face);
    face->setFace(f);

    results << face;
  }
  return results;
}

B_END_NAMESPACE
