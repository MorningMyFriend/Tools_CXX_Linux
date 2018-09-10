//
// Created by wurui on 18-9-5.
//

#include <iostream>
#include <math.h>
#include "image_process.h"

using namespace std;
using namespace cv;

inline float dist(const Point2f &p1, const Point2f &p2){
    return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}

void test_perspective(){
    string rawImgPath = "/home/wurui/Pictures/1.png";
    Mat img  = imread(rawImgPath);
    imshow("org", img);

    // roiCorner = [lt rt rb lb]
        Point2f roiCorner[4] = {
                {235, 479},
                {618, 478},
                {620, 559},
                {236, 563}
        };

    Mat templateImage;
    // des coords use mean rect
    int w = (int)((dist(roiCorner[0], roiCorner[1]) + dist(roiCorner[2], roiCorner[3]))/2);
    int h = (int)((dist(roiCorner[0], roiCorner[3]) + dist(roiCorner[1], roiCorner[2]))/2);
    cv::Size dsize(w, h);
    cv_ex::WarpImageToRectPerspective(img, templateImage, roiCorner, dsize, Point(4,2));

    imshow("template", templateImage);
    waitKey(0);
}

void test_rotateRect(){
    Mat image = cv::imread("../../data/rotateRect.png");
    Mat gray,binary;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, binary, 125, 255, cv::THRESH_BINARY);
    cv::imshow("binary", binary);
//    cv::waitKey(0);

    // find contours
    vector<vector<Point2f>> contours;
    vector<Vec4i> hierachy;
    cv::findContours(binary, contours, hierachy, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE);
    for (int i = 0; i < contours.size(); ++i) {
        cv::drawContours(image, contours, i, Scalar(0,255,0), 2);
    }
    cv::imshow("contours", image);
    cv::waitKey(0);

    // rotateRect
    vector<cv::RotatedRect> rects;
    for (int i = 0; i < contours.size(); ++i) {
        cv::RotatedRect rotatedRect = cv::minAreaRect(contours.at(i));
        Point2f center = rotatedRect.center;
        Size size = rotatedRect.size;
        float ang = rotatedRect.angle;
    }
}


int main(){
    test_perspective();
}