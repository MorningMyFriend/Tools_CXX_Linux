//
// Created by wurui on 18-9-10.
//

#include "cvRect_ex.h"

namespace cv_ex {
    cv::Rect BoundingRect(const cv::Mat &image) {
        return cv::Rect(0, 0, image.cols, image.rows);
    }


    cv::Rect ExtendRect(const cv::Mat &image1, const cv::Mat &image2) {
        return BoundingRect(image1) | BoundingRect(image2);
    }

    void ValidateRect(const cv::Mat &image, cv::Rect &rect) {
        rect &= BoundingRect(image);
    }

    int Xmid(const cv::Rect &rect) {
        return rect.x + (rect.width - 1) / 2;
    }

    int Ymid(const cv::Rect &rect) {
        return rect.y + (rect.height - 1) / 2;
    }

    float IOU(const cv::Rect &rect1, const cv::Rect &rect2) {
        int areaIntersection = (rect1 & rect2).area();
        return static_cast<float>(areaIntersection) / (rect1.area() + rect2.area() - areaIntersection);
    }

    void ExpandMoveRect(cv::Rect &rect1, int xBorder, int yBorder, int xShift, int yShft) {
        rect1 += cv::Point(xShift, yShft);
        rect1 += cv::Size(xBorder, yBorder);
    }
}