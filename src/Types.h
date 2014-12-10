#ifndef B_TYPES_H
#define B_TYPES_H

#include <memory>

#include <opencv2/opencv.hpp>

#include "Global.h"

B_BEGIN_NAMEPACE

typedef std::shared_ptr<cv::Mat> MatPtr;

B_END_NAMESPACE

#endif // B_TYPES_H
