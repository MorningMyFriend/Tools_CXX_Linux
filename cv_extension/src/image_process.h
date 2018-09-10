//
// Created by wurui on 18-9-10.
//

#ifndef TOOLS_CXX_LINUX_IMAGE_PROCESS_H
#define TOOLS_CXX_LINUX_IMAGE_PROCESS_H

#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d.hpp"

namespace cv_ex {

    // image crop resize
    void ImgResizeAndFill(const cv::Mat &img, const int newWidth, const int newHeight, cv::Mat &outImg);

    void ImgResizeAndFill(const cv::Mat &img, const cv::Size &newSize, cv::Mat &outImg);

    void ImgResizeAndFill(const cv::Mat &img, const cv::Size &newSize, cv::Mat &outImg, cv::Rect &orgRect);

    void ImgResizeAndFill(const cv::Mat &img, const int newWidth, const int newHeight, cv::Mat &outImg, cv::Rect &orgRect);

    // rotate
    void imgRotate(cv::Mat const &img, cv::Mat &outImg, double angleInDegree);

    // feature match
    void RansacMatch(const std::vector<cv::KeyPoint> &keyPtsQuery, // input
                     const std::vector<cv::KeyPoint> &keyPtsTrain, // input
                     const std::vector<cv::DMatch> &featureMatches, // input
                     std::vector<cv::KeyPoint> &ransacKeyPtsTrain, // output
                     std::vector<cv::KeyPoint> &ransacKeyPtsQuery,
                     std::vector<cv::DMatch> &ransacMatches);

    void FeatureMatch(const cv::Mat &inputImageRGB, const cv::Mat &templateImageRGB,
                      std::vector<cv::KeyPoint> &keyPtsTemplate, std::vector<cv::KeyPoint> &keyPtsInput,
                      std::vector<cv::DMatch> &goodMatches,
                      float goodmatchThresh = 3.0, std::string feature = "SIFT");

    // perspective transform image
    void WarpImageToRectPerspective(const cv::Mat &inputImage, cv::Mat &outputImage,
                                    cv::Point2f *rectCornersInInputImage, cv::Size destSize,
                                    cv::Point border = cv::Point(30, 30));

    void
    WarpImageUseFeatureMatchPerspective(cv::Mat const &inputImage, cv::Mat const &templateImage, cv::Mat &outputImage,
                                        float zoom = 0.6, float goodmatchThresh = 3.0, int xOutputExtend = 0,
                                        int yOutputExtend = 0, std::string feature = "SIFT");

    void
    WarpImageUseFeatureMatchPerspective(cv::Mat const &inputImage, cv::Mat const &templateImage, cv::Mat &imageToWarp,
                                        cv::Mat &outputImage,
                                        float zoom = 0.6, float goodmatchThresh = 3.0, int xOutputExtend = 0,
                                        int yOutputExtend = 0, std::string feature = "SIFT");

    // polynomial transform image
    void getTransform12Param(std::vector<cv::Point2f> &srcPts, std::vector<cv::Point2f> &desPts, cv::Mat &transformMat);

    void operateTransformToMat(cv::Mat &map1, cv::Mat &map2, cv::Mat &transformMat);

    void polynomialTransform(cv::Mat &inputImage, cv::Mat &outputImage, cv::Mat &transformMat);

    void
    WarpImageUseFeatureMatchPolynomial(cv::Mat const &inputImage, cv::Mat const &templateImage, cv::Mat &outputImage,
                                       float zoom = 0.6, std::string feature = "SIFT");


};


#endif //TOOLS_CXX_LINUX_IMAGE_PROCESS_H
