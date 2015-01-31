// Copyright (c) Burator 2014-2015
#ifndef B_FACE_DETECTOR_H
#define B_FACE_DETECTOR_H

#include <QList>

#include <opencv2/opencv.hpp>

#include "Face.h"
#include "Types.h"
#include "Global.h"

B_BEGIN_NAMESPACE

class FaceDetector {
public:
  FaceDetector();

  bool isValid() const { return valid; }
  QList<FacePtr> detect(const MatPtr image);

private:
  QList<FacePtr> createFaces(std::vector<cv::Rect> &faces, const cv::Mat &image,
                             bool vflip = false);

  bool valid;
  cv::CascadeClassifier faceCas, pfaceCas, eyesCas;
};

B_END_NAMESPACE

#endif // B_FACE_DETECTOR_H
