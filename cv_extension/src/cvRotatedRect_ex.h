//
// Created by wurui on 18-9-10.
//

#ifndef TOOLS_CXX_LINUX_CVROTATEDRECT_H
#define TOOLS_CXX_LINUX_CVROTATEDRECT_H

#include <iostream>
#include <algorithm>
#include <vector>

#include "opencv2/opencv.hpp"
#include "cvRect_ex.h"

/*
1、角度angle的取值是由RotatedRect的width和X轴的夹角组成的
2、RotatedRect的width和height的选取跟矩形的尺寸无关,并非长的就是height，短的就是width。
3、opencv默认把 水平轴（x轴）逆时针旋转，碰到的第一个边,这个边的边长作为width。
4、angle的取值范围必然是负的，实际angle的取值范围为(-90,0]
 */

namespace cv_ex{
    using namespace cv;

    void CropRotatedRectFromImg(const cv::Mat &image, const cv::RotatedRect &rotatedRect, cv::Mat &cropImg);

    bool IsContainPt(const cv::RotatedRect &rotatedRect, const cv::Point2f &pt);
}


#endif //TOOLS_CXX_LINUX_CVROTATEDRECT_H
