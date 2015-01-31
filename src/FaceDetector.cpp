// Copyright (c) Burator 2014-2015
#include <QFile>
#include <QDebug>

#include <vector>

#include "Util.h"
#include "FaceDetector.h"

B_BEGIN_NAMESPACE

FaceDetector::FaceDetector() {
  valid = true;

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

  try {
    auto fs = Util::fileToFileStorage(":res/eyes.xml");
    if (!fs || !eyesCas.read(fs->getFirstTopLevelNode())) {
      qCritical() << "Could not read eyes data.";
      valid = false;
    }
  }
  catch (...) {
    qCritical() << "Invalid eyes cascade!";
    valid = false;
  }
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

    // Detect two eyes for each face with scale factor 1.1, 3
    // min. neighbors and min size of 30x30.
    cv::Mat facePart = image(f);
    std::vector<cv::Rect> eyes;
    eyesCas.detectMultiScale(facePart, eyes, 1.1, 3, 0, cv::Size(30, 30));
    if (eyes.size() >= 2) {
      // Take the two largest.
      auto &eye1 = eyes[0], &eye2 = eyes[1];
      int area1 = eye1.width * eye1.height,
        area2 = eye2.width * eye2.height;
      if (area1 > area2) {
        qSwap<cv::Rect>(eye1, eye2);
        qSwap<int>(area1, area2);
      }
      for (size_t i = 2; i < eyes.size(); i++) {
        const auto &eye = eyes[i];
        int area = eye.width * eye.height;
        if (area > area1 && area < area2) {
          eye1 = eye;
          area1 = area;
        }
        else if (area > area2 && area < area1) {
          eye2 = eye;
          area2 = area;
        }
        else if (area > area1 && area > area2) {
          eye1 = eye;
          area1 = area;
          qSwap<cv::Rect>(eye1, eye2);
          qSwap<int>(area1, area2);
        }
      }

      // Since we are only looking at the sub-region of the face we
      // need to convert into image coordinates.
      eye1.x += f.x; eye1.y += f.y;
      eye2.x += f.x; eye2.y += f.y;
      face->setEyes(eye1, eye2);
    }

    results << face;
  }
  return results;
}

B_END_NAMESPACE
