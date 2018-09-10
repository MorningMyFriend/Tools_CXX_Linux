//
// Created by wurui on 18-9-10.
//

#ifndef TOOLS_CXX_LINUX_CVRECT_EX_H
#define TOOLS_CXX_LINUX_CVRECT_EX_H

#include <iostream>
#include <algorithm>
#include <vector>
#include "opencv2/opencv.hpp"

namespace cv_ex {
    cv::Rect BoundingRect(const cv::Mat &image);

    cv::Rect ExtendRect(const cv::Mat &image1, const cv::Mat &image2);

    void ValidateRect(const cv::Mat &image, cv::Rect &rect);

    int Xmid(const cv::Rect &rect);

    int Ymid(const cv::Rect &rect);

    float IOU(const cv::Rect &rect1, const cv::Rect &rect2);

    void ExpandMoveRect(cv::Rect &rect1, int xBorder, int yBorder, int xShift, int yShirt);
}


#endif //TOOLS_CXX_LINUX_CVRECT_EX_H
