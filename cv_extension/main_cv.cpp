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

int main(){
    test_perspective();
}