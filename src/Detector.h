// Copyright (c) Burator 2014-2015
#ifndef B_DETECTOR_H
#define B_DETECTOR_H

#include <vector>

#include <opencv2/opencv.hpp>

#include "Types.h"
#include "Global.h"

B_BEGIN_NAMESPACE

class Detector {
public:
  Detector();

  bool isValid() const { return valid; }

  std::vector<cv::Rect> detectFaces(const MatPtr image);
  std::vector<cv::Rect> detectPlates(const MatPtr image);

private:
  bool valid;
  cv::CascadeClassifier faceCas, plateCas;
};

B_END_NAMESPACE

#endif // B_DETECTOR_H
