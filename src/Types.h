// Copyright (c) Burator 2014-2015
#ifndef B_TYPES_H
#define B_TYPES_H

#include <memory>

#include <opencv2/opencv.hpp>

#include "Global.h"

B_BEGIN_NAMESPACE

typedef std::shared_ptr<bool> BoolPtr;
typedef std::shared_ptr<cv::Mat> MatPtr;
typedef std::shared_ptr<cv::FileStorage> FSPtr;

B_END_NAMESPACE

#endif // B_TYPES_H
