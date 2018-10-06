#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <map>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
//#include "opencv2/core.hpp"
#include "params_parser.h"



class Cube {
public:
    enum class filterMode {TOHSV, TOGRAY};
    enum class detectionMode {BLOB, CONTOURS};
private:
    cv::Mat mWorkingFrame, mOriginalFrame, mBinaryImage;
    cv::Mat colorFilter(cv::Mat frame, filterMode mode);
    std::vector<cv::KeyPoint> blobDetect(cv::Mat frame);
    paramsInput mParams;
    std::vector<cv::Rect> Rects(cv::Mat frame);
    cv::Rect merge(const cv::Rect&, const cv::Rect&);
    bool valid(const cv::Rect&);
    cv::Mat paintTarget(const std::vector<cv::Rect>&);
public:
    Cube(cv::Mat frame, const std::string& filename = "parameters.txt");
    Cube(cv::Mat frame, const paramsInput& params);
    int getPosition(detectionMode mode);
    cv::Mat showFrame();
};
