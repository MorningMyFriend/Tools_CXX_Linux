//
// Created by wurui on 18-9-10.
//

#include <vector>
#include "cvRotatedRect_ex.h"

using namespace std;

namespace cv_ex {
    void CropRotatedRectFromImg(const cv::Mat &image, const cv::RotatedRect &rotatedRect, cv::Mat &cropImg) {
        Mat imgshow = image.clone();
        cv::rectangle(imgshow, rotatedRect.boundingRect(), Scalar(0, 255, 0), 2);
        cv::imshow("org", imgshow);

        cv::Point newCenter = static_cast<cv::Point>(rotatedRect.center) - rotatedRect.boundingRect().tl();
        cv::Mat r = cv::getRotationMatrix2D(newCenter, rotatedRect.angle, 1);
        cv::Mat img, newImg;

        cv::Rect bounding = rotatedRect.boundingRect();
        cv_ex::ValidateRect(image, bounding);
        img = image(bounding).clone();

        cv::warpAffine(img, newImg, r, rotatedRect.boundingRect().size(), cv::INTER_CUBIC);

        cv::Rect orgRect(newCenter - Point(rotatedRect.size.width / 2, rotatedRect.size.height / 2), rotatedRect.size);
        cv_ex::ValidateRect(newImg, orgRect);
        cropImg = newImg(orgRect);
        cv::imshow("", cropImg);
        cv::waitKey();
    }

    bool IsContainPt(const cv::RotatedRect &rotatedRect, const cv::Point2f &pt) {
        // 放射矩阵补充一行成为透视矩阵
        cv::Mat r = cv::getRotationMatrix2D(static_cast<cv::Point>(rotatedRect.center), rotatedRect.angle, 1);
        cv::Mat rp(3, 3, CV_32FC1, Scalar(0));
        r.copyTo(rp(Rect(0, 0, 3, 2)));
        rp.at<float>(2, 2) = 1;

//        Point2f ptr[4];
//        rotatedRect.points(ptr);
//        vector<Point2f> corner(ptr, ptr+4);

        vector<Point2f> pts, ptsDes;
        pts.emplace_back(pt);
        cv::perspectiveTransform(pts, ptsDes, rp);

        return (abs(ptsDes[0].x - rotatedRect.center.x) <= rotatedRect.size.width / 2 &&
                abs(ptsDes[0].y - rotatedRect.center.y) <= rotatedRect.size.height / 2);
    }

}