#ifndef B_FACE_DETECTOR_H
#define B_FACE_DETECTOR_H

#include <QList>

#include <opencv2/opencv.hpp>

#include "Face.h"
#include "Types.h"
#include "Global.h"

class QString;

B_BEGIN_NAMESPACE

class FaceDetector {
public:
  FaceDetector(const QString &faceCascade, const QString &eyesCascade);

  bool isValid() const { return valid; }
  QList<FacePtr> detect(const MatPtr image);

private:
  bool valid;
  cv::CascadeClassifier faceCas, eyesCas;
};

B_END_NAMESPACE

#endif // B_FACE_DETECTOR_H
