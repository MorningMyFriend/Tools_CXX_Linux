//
// Created by wurui on 18-9-10.
//

#include "image_process.h"

#define __SHOW_DEBUG__ 0

namespace cv_ex {
    using namespace std;
    using namespace cv;

    // resize the image while preserve its width-height ratio
    void ImgResizeAndFill(const cv::Mat &img, const int newWidth, const int newHeight, cv::Mat &outImg) {
        if (img.cols == newWidth && img.rows == newHeight) return;

        outImg.release();

        if (img.channels() == 3) {
            cv::Mat outImg(newHeight, newWidth, CV_8UC3, cv::Scalar(0, 0, 0));
        } else if (img.channels() == 1) {
            cv::Mat outImg(newHeight, newWidth, CV_8UC1, cv::Scalar(0));
        } else {
            cerr << "ImgResizeAndFill: input image channels is not 1 or 3 ! exit" << endl;
            return;
        }

        float wScale = static_cast<float>(newWidth) / img.cols;
        float hScale = static_cast<float>(newHeight) / img.rows;

        float unifiedScale = wScale < hScale ? wScale : hScale;

        cv::Mat tempImg;
        cv::resize(img, tempImg,
                   cv::Size(static_cast<int>(unifiedScale * img.cols), static_cast<int>(unifiedScale * img.rows)));

        int xShift = (outImg.cols - tempImg.cols) / 2;
        int yShift = (outImg.rows - tempImg.rows) / 2;
        tempImg.copyTo(outImg(cv::Rect(xShift, yShift, tempImg.cols, tempImg.rows)));
    }

    void ImgResizeAndFill(const Mat &img, const Size &newSize, cv::Mat &outImg) {
        ImgResizeAndFill(img, newSize.width, newSize.height, outImg);
    }

    // resize image, record org image rect in new image
    void ImgResizeAndFill(const Mat &img, const Size &newSize, cv::Mat &outImg, cv::Rect &orgRect) {
        return ImgResizeAndFill(img, newSize.width, newSize.height, outImg, orgRect);
    }

    void ImgResizeAndFill(const Mat &img, const int newWidth, const int newHeight, cv::Mat &outImg, cv::Rect &orgRect) {
        if (img.cols == newWidth && img.rows == newHeight) return;

        outImg.release();

        if (img.channels() == 3) {
            cv::Mat outImg(newHeight, newWidth, CV_8UC3, cv::Scalar(0, 0, 0));
        } else if (img.channels() == 1) {
            cv::Mat outImg(newHeight, newWidth, CV_8UC1, cv::Scalar(0));
        } else {
            cerr << "ImgResizeAndFill: input image channels is not 1 or 3 ! exit" << endl;
            return;
        }

        float wScale = static_cast<float>(newWidth) / img.cols;
        float hScale = static_cast<float>(newHeight) / img.rows;

        float unifiedScale = wScale < hScale ? wScale : hScale;

        cv::Mat tempImg;
        cv::resize(img, tempImg,
                   cv::Size(static_cast<int>(unifiedScale * img.cols), static_cast<int>(unifiedScale * img.rows)));

        int xShift = (outImg.cols - tempImg.cols) / 2;
        int yShift = (outImg.rows - tempImg.rows) / 2;
        tempImg.copyTo(outImg(cv::Rect(xShift, yShift, tempImg.cols, tempImg.rows)));

        orgRect.x = xShift;
        orgRect.y = yShift;
        orgRect.width = tempImg.cols;
        orgRect.height = tempImg.rows;
    }

    // rotate
    void imgRotate(cv::Mat const &img, cv::Mat &outImg, double angleInDegree) {
        cv::Mat r = cv::getRotationMatrix2D(cv::Point2d(img.cols / 2.0, img.rows / 2.0), angleInDegree, 1.0);
        cv::warpAffine(img, outImg, r, img.size());
    }

    // feature match
    void FeatureMatch(const Mat &templateImageRGB, // input query image
                      const Mat &inputImageRGB, // input train image
                      vector<KeyPoint> &keyPtsTemplate, vector<KeyPoint> &keyPtsInput, // output
                      std::vector<DMatch> &goodMatches, // output
                      float goodmatchThresh, string feature) {
        // goodmatchThresh is the time of minimum of feature distance
        // match function : keyPtsQuery-queryImage, keyPtsTrain-trainImage, output matches has the same num of keyPtsQuery

        cv::Mat imageInput, imageTemplate;

        // search feature point
        cvtColor(inputImageRGB, imageInput, CV_BGR2GRAY);
        cvtColor(templateImageRGB, imageTemplate, CV_BGR2GRAY);

        // nFeaures menas the number of best keypoints that meet the two thresholds
        // if nFeatures is equal to 0 , all keypoints that meet the two thresholds are retrained
        Ptr<FeatureDetector> f2d;
        if (feature == "SIFT") {
            f2d = cv::xfeatures2d::SIFT::create(0, 3, 0.04, 10, 1.6); // use default parameters in cv
        } else if (feature == "SURF") {
            f2d = cv::xfeatures2d::SURF::create(); // default
        }
        f2d->detect(imageInput, keyPtsInput);
        f2d->detect(imageTemplate, keyPtsTemplate);

#if __SHOW_DEBUG__
        // draw key points
    cv::drawKeypoints(inputImageRGB, keyPtsInput, inputImageRGB, Scalar(255,255,0),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    cv::drawKeypoints(templateImageRGB, keyPtsTemplate, templateImageRGB, Scalar(255,255,0),cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    cout << "feature points num inputImage: " << keyPtsInput.size()<<endl;
    cout << "feature points num templateImage: "<< keyPtsTemplate.size()<<endl;
    imshow("featureInputImage", inputImageRGB);
    imshow("featureTemplate", templateImageRGB);
    waitKey(0);
#endif

        // compute describe vector
        Mat descriptorsTemplate, descriptorsInput;

        f2d->compute(imageInput, keyPtsInput, descriptorsInput);
        f2d->compute(imageTemplate, keyPtsTemplate, descriptorsTemplate);

        // matching of feature vector
        FlannBasedMatcher matcher;
        vector<DMatch> matches;
        matcher.match(descriptorsTemplate, descriptorsInput, matches);

        if (matches.size() < 1) {
            cout << "warning: no matches !" << endl;
            return;
        }

#if __SHOW_DEBUG__
        // draw matching result
    cv::Mat matchShow;
    cv::drawMatches(templateImageRGB, keyPtsTemplate, inputImageRGB, keyPtsInput, matches, matchShow);
    cout << "matches num: "<<matches.size()<<endl;
    cv::resize(matchShow, matchShow, Size(720, 360));
    imshow("match", matchShow);
    waitKey(0);
#endif

        // remove bad matches, compute the Euclid distance of two matched vectors
        double maxDist, minDist;
        for (int i = 0; i < descriptorsTemplate.rows; i++) {
            double dist = matches[i].distance;
            if (i == 0) {
                maxDist = dist;
                minDist = dist;
            } else {
                if (dist < minDist) minDist = dist;
                if (dist > maxDist) maxDist = dist;
            }
        }

        // good match: distance < 3*minDist
        // warning : if minDist=0, add all matches
        if (minDist > 0) {
            for (int i = 0; i < descriptorsTemplate.rows; i++) {
                if (matches[i].distance < goodmatchThresh * minDist)
                    goodMatches.emplace_back(matches[i]);
            }
        } else {
            for (int i = 0; i < descriptorsTemplate.rows; i++) {
                goodMatches.emplace_back(matches[i]);
            }
        }

#if __SHOW_DEBUG__
        // show good matches
    cout << "---Worst match distance:" << maxDist << endl;
    cout << "---Best match distance:" << minDist << endl;
    Mat goodMatchesToShow;
    drawMatches(templateImageRGB, keyPtsTemplate, inputImageRGB, keyPtsInput, goodMatches, goodMatchesToShow,
                Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    cout << "good match num "<< goodMatches.size() <<endl;
    cv::resize(goodMatchesToShow, goodMatchesToShow, Size(720, 360));
    imshow("Good Matches", goodMatchesToShow);
    waitKey(0);
#endif
    }

    void RansacMatch(const std::vector<cv::KeyPoint> &keyPtsQuery, // input
                     const std::vector<cv::KeyPoint> &keyPtsTrain, // input
                     const std::vector<cv::DMatch> &featureMatches, // input
                     vector<KeyPoint> &ransacKeyPtsTrain, // output
                     vector<KeyPoint> &ransacKeyPtsQuery,
                     std::vector<cv::DMatch> &ransacMatches) {
        // 1. feature points alignment and convert to float
        // 2. cv::findFundamentalMat, get RansacStatus
        // 3. delete the points pair where RansacStatis[i]=0

        // feature alignment
        vector<KeyPoint> ptsQuery, ptsTrain;
        for (cv::DMatch match: featureMatches) {
            ptsQuery.emplace_back(keyPtsQuery.at(match.queryIdx));
            ptsTrain.emplace_back(keyPtsTrain.at(match.trainIdx));
        }

        // convert to float
        vector<Point2f> ptsQuery2f, ptsTrain2f;
        for (int i = 0; i < featureMatches.size(); ++i) {
            ptsQuery2f.emplace_back(keyPtsQuery.at(i).pt);
            ptsTrain2f.emplace_back(keyPtsTrain.at(i).pt);
        }

        // eliminate bad matches using findFundamentaMat
        vector<uchar> ransacStatus;
        Mat fundamentalMat = cv::findFundamentalMat(ptsQuery2f, ptsTrain2f, ransacStatus, FM_RANSAC);

        // generate new KeyPoint and matches
        int index = 0;
        for (std::size_t i = 0; i < featureMatches.size(); ++i) {
            if (ransacStatus.at(i) == 0) continue;
            ransacKeyPtsQuery.push_back(keyPtsQuery.at(i));
            ransacKeyPtsTrain.push_back(keyPtsTrain.at(i));
            DMatch tempMatch = featureMatches[i];
            tempMatch.queryIdx = index;
            tempMatch.trainIdx = index;
            ransacMatches.push_back(tempMatch);
            index++;
        }
    }

    // perspective transform
    void WarpImageToRectPerspective(const cv::Mat &inputImage, cv::Mat &outputImage,
                                    cv::Point2f *rectCornersInInputImage, cv::Size destSize,
                                    cv::Point border) {
        // select 4 corner points, assume them as a rectangle on new image, so that we can get a set of perspective params,
        // and use the params to warp inputImage to outputImg
        // destSize: usually is the mean rect of rectCornersInInputImage

        cv::Rect destRc(border, destSize);

        cv::Point2f rectCorners[] = {
                destRc.tl(),
                cv::Point(destRc.x + destRc.width, destRc.y),
                destRc.br(),
                cv::Point(destRc.x, destRc.y + destRc.height)
        };


        cv::Mat &&perspectiveM = cv::getPerspectiveTransform(rectCorners, rectCornersInInputImage);

        cv::Size sizeWithBorder = destSize + cv::Size(2 * border.x, 2 * border.y);
        cv::warpPerspective(inputImage, outputImage, perspectiveM, sizeWithBorder,
                            cv::INTER_LINEAR + cv::WARP_INVERSE_MAP, cv::BORDER_REPLICATE);
    }

    void WarpImageUseFeatureMatchPerspective(cv::Mat const &inputImage, cv::Mat const &templateImage,
                                             cv::Mat &outputImage, float zoom, float goodmatchThresh,
                                             int xOutputExtend, int yOutputExtend, std::string feature) {
        // input RGB image and template image, feature match, ransac, perspective warp image
        Mat inputImageRGB = inputImage.clone();
        Mat templateImageRGB = templateImage.clone();
        resize(inputImageRGB, inputImageRGB, Size(inputImageRGB.cols * zoom, inputImageRGB.rows * zoom));
        resize(templateImageRGB, templateImageRGB, Size(templateImageRGB.cols * zoom, templateImageRGB.rows * zoom));

        // feature match
        vector<cv::KeyPoint> keyPtsTemplate, keyPtsInput;
        vector<DMatch> goodMatches;
        FeatureMatch(templateImageRGB, inputImageRGB, keyPtsTemplate, keyPtsInput, goodMatches, goodmatchThresh,
                     feature);

        // compute the homography matrix and locate the object
        vector<Point2f> conjugatePointsTemplate;
        vector<Point2f> conjugatePointsInput;

        for (DMatch goodmatch: goodMatches) {
            conjugatePointsTemplate.push_back(keyPtsTemplate[goodmatch.queryIdx].pt / zoom);
            conjugatePointsInput.push_back(keyPtsInput[goodmatch.trainIdx].pt / zoom);
        }

        // RANSAC
        // find the homographu matrix using RANSAC, only works for flann transformation
        Mat perspectiveMat = findHomography(conjugatePointsTemplate, conjugatePointsInput, RANSAC);

        // get the corner of the templateImageRGB
        vector<Point2f> templateCorners = {
                cvPoint(0, 0),
                cvPoint(templateImage.cols, 0),
                cvPoint(templateImage.cols, templateImage.rows),
                cvPoint(0, templateImage.rows)
        };

        // compute the corresponding corners in the scenimage
        vector<Point2f> tempCornerInInputImage(4);
        cv::perspectiveTransform(templateCorners, tempCornerInInputImage, perspectiveMat);

#if __SHOW_DEBUG__
        //    Mat finalROI = inputImageRGB.clone();
//for(int i=0; i<3; i++){
//    cv::line(finalROI, tempCornerInInputImage[i], tempCornerInInputImage[(i+1)%4], Scalar(0, 255, 0), 4);
//}
//imshow("roi", finalROI);
//waitKey(0);
#endif

        Mat warppedImage;
        cv::warpPerspective(inputImage, warppedImage, perspectiveMat,
                            inputImage.size(), INTER_CUBIC + cv::WARP_INVERSE_MAP, cv::BORDER_REPLICATE);

        Rect roi(Point(0, 0), templateImage.size());
        if (xOutputExtend < 0 || yOutputExtend < 0) {
            // reuturn image has same size of template image
            roi.width = xOutputExtend < 0 ? warppedImage.cols : templateImage.cols;
            roi.height = yOutputExtend < 0 ? warppedImage.rows : templateImage.rows;

        } else if (xOutputExtend > 0 && yOutputExtend > 0) {
            // return image has extend x y area
            roi.width = xOutputExtend += templateImage.cols;
            roi.height = yOutputExtend += templateImage.rows;
        }
        // else return whole warped image
        roi &= cv::Rect(0, 0, warppedImage.cols, warppedImage.rows);
        outputImage = warppedImage(roi).clone();

#if __SHOW_DEBUG__
        imshow("warped image", warppedImage);
imshow("outputImage", outputImage);
waitKey(0);
#endif

    }


    void WarpImageUseFeatureMatchPerspective(cv::Mat const &inputImage, cv::Mat const &templateImage,
                                             cv::Mat &imageToWarp, cv::Mat &outputImage, float zoom,
                                             float goodmatchThresh, int xOutputExtend, int yOutputExtend,
                                             std::string feature) {
        // use inputImage and templateImage to feature match and get transform mat
        // transform imageToWarp with transform mat
        // normally, inputImage is the same image as imageToWarp, or inputImage is roi image of imageToWarp
        // input RGB image and template image, feature match, ransac, perspective warp image
        Mat inputImageRGB = inputImage.clone();
        Mat templateImageRGB = templateImage.clone();
        if (abs(zoom - 1.0) > 0.01) {
            resize(inputImageRGB, inputImageRGB, Size(inputImageRGB.cols * zoom, inputImageRGB.rows * zoom));
            resize(templateImageRGB, templateImageRGB,
                   Size(templateImageRGB.cols * zoom, templateImageRGB.rows * zoom));
        }

        // feature match
        vector<cv::KeyPoint> keyPtsTemplate, keyPtsInput;
        vector<DMatch> goodMatches;
        FeatureMatch(templateImageRGB, inputImageRGB, keyPtsTemplate, keyPtsInput, goodMatches, goodmatchThresh,
                     feature);

        // compute the homography matrix and locate the object
        vector<Point2f> conjugatePointsTemplate;
        vector<Point2f> conjugatePointsInput;

        for (DMatch goodmatch: goodMatches) {
            conjugatePointsTemplate.push_back(keyPtsTemplate[goodmatch.queryIdx].pt / zoom);
            conjugatePointsInput.push_back(keyPtsInput[goodmatch.trainIdx].pt / zoom);
        }

        // RANSAC
        // find the homographu matrix using RANSAC, only works for flann transformation
        Mat perspectiveMat = findHomography(conjugatePointsTemplate, conjugatePointsInput, RANSAC);

        // get the corner of the templateImageRGB
        vector<Point2f> templateCorners = {
                cvPoint(0, 0),
                cvPoint(templateImage.cols, 0),
                cvPoint(templateImage.cols, templateImage.rows),
                cvPoint(0, templateImage.rows)
        };

        // compute the corresponding corners in the scenimage
        vector<Point2f> tempCornerInInputImage(4);
        cv::perspectiveTransform(templateCorners, tempCornerInInputImage, perspectiveMat);

#if __SHOW_DEBUG__
        Mat finalROI = inputImageRGB.clone();
for(int i=0; i<3; i++){
    cv::line(finalROI, tempCornerInInputImage[i], tempCornerInInputImage[(i+1)%4], Scalar(0, 255, 0), 4);
}
imshow("roi", finalROI);
waitKey(0);
#endif

        // transform , get warped image
        Mat warppedImage;
        cv::warpPerspective(imageToWarp, warppedImage, perspectiveMat,
                            imageToWarp.size(), INTER_CUBIC + cv::WARP_INVERSE_MAP, cv::BORDER_REPLICATE);

        Rect roi(Point(0, 0), templateImage.size());
        if (xOutputExtend < 0 || yOutputExtend < 0) {
            // reuturn image has same size of template image
            roi.width = xOutputExtend < 0 ? warppedImage.cols : templateImage.cols;
            roi.height = yOutputExtend < 0 ? warppedImage.rows : templateImage.rows;
            roi &= cv::Rect(0, 0, warppedImage.cols, warppedImage.rows);

        } else if (xOutputExtend > 0 && yOutputExtend > 0) {
            // return image has extend x y area
            roi.width = xOutputExtend += templateImage.cols;
            roi.height = yOutputExtend += templateImage.rows;
            roi &= cv::Rect(0, 0, warppedImage.cols, warppedImage.rows);
        } else {
            // else return whole warped image
            roi.width = warppedImage.cols;
            roi.height = warppedImage.rows;
        }
        outputImage = warppedImage(roi).clone();

#if __SHOW_DEBUG__
        imshow("warped image", warppedImage);
imshow("outputImage", outputImage);
waitKey(0);
#endif
    }

    // polynomial transform
    void getTransform12Param(std::vector<Point2f> &srcPts, std::vector<Point2f> &desPts, cv::Mat &transformMat) {
        // srcPoint(x,y)   desPoint(x',y')
        // x1' = [1 x1 y1 x1y1 x1^2 y1^2 0 0 0 0 0 0] * [a1 b1 c1 d1 e1 f1 a2 b2 c2 d2 e2 f2]T
        // y1' = [0 0 0 0 0 0 1 x1 y1 x1y1 x1^2 y1^2] * [a1 b1 c1 d1 e1 f1 a2 b2 c2 d2 e2 f2]T

        // ensure points' num is same
        int pointNum = std::min(srcPts.size(), desPts.size());
        if (srcPts.size() != desPts.size()) {
            for (int i = 0; i < pointNum - srcPts.size(); ++i) {
                srcPts.pop_back();
            }
            for (int i = 0; i < pointNum - desPts.size(); ++i) {
                desPts.pop_back();
            }
        }

        // create Mats to be used : A(2n*12) * X(12*1) = B(2n*1)
        cv::Mat A(2 * pointNum, 12, CV_32FC1);
        cv::Mat X(12, 1, CV_32FC1);
        cv::Mat B(2 * pointNum, 1, CV_32FC1);

        for (int i = 0; i < pointNum; ++i) {
            float a1[12] = {1, srcPts[i].x, srcPts[i].y, srcPts[i].x * srcPts[i].y, std::pow(srcPts[i].x, 2),
                            std::pow(srcPts[i].y, 2), 0, 0, 0, 0, 0, 0};
            float a2[12] = {0, 0, 0, 0, 0, 0, 1, srcPts[i].x, srcPts[i].y, srcPts[i].x * srcPts[i].y,
                            std::pow(srcPts[i].x, 2), std::pow(srcPts[i].y, 2)};
            for (int j = 0; j < 12; ++j) {
                A.at<float>(2 * i, j) = a1[j];
                A.at<float>(2 * i + 1, j) = a2[j];
            }
            B.at<float>(2 * i, 0) = desPts[i].x;
            B.at<float>(2 * i + 1, 0) = desPts[i].y;
        }

        // solve the formulation
        // can not use cv::solvePoly, because the transform formulation has item as x*y
        cv::solve(A, B, X, DECOMP_NORMAL);
        transformMat = X.clone();
    }

    void operateTransformToMat(cv::Mat &map1, cv::Mat &map2, cv::Mat &transformMat) {
        // x1' = [1 x1 y1 x1y1 x1^2 y1^2 0 0 0 0 0 0] * [a1 b1 c1 d1 e1 f1 a2 b2 c2 d2 e2 f2]T
        // y1' = [0 0 0 0 0 0 1 x1 y1 x1y1 x1^2 y1^2] * [a1 b1 c1 d1 e1 f1 a2 b2 c2 d2 e2 f2]T
        // remap(src, des, map1, map2) : src.size=map1.size=map2.size
        // map1 is new x(col),map2 is new y(row)

        if (map1.empty() || map2.empty()) {
            cerr << "cv::remap() input map_x,map_y empty! need init as CV_32FC1" << endl;
            return;
        }
        if (map1.size != map2.size) map2 = map1.clone();

        for (int i = 0; i < map1.rows; ++i) {
            if (i == 1) cout << endl;
            for (int j = 0; j < map1.cols; ++j) {
                cv::Mat A(2, 12, CV_32FC1);
                cv::Mat B(2, 1, CV_32FC1);
                float x = j;
                float y = i;
                float a1[12] = {1, x, y, x * y, std::pow(x, 2), std::pow(y, 2), 0, 0, 0, 0, 0, 0};
                float a2[12] = {0, 0, 0, 0, 0, 0, 1, x, y, x * y, std::pow(x, 2), std::pow(y, 2)};
                for (int j = 0; j < 12; ++j) {
                    A.at<float>(0, j) = a1[j];
                    A.at<float>(1, j) = a2[j];
                }
                B = A * transformMat;
                map1.at<float>(i, j) = B.at<float>(0, 0);
                map2.at<float>(i, j) = B.at<float>(1, 0);
            }
        }
    }

    void polynomialTransform(cv::Mat &inputImage, cv::Mat &outputImage, cv::Mat &transformMat) {
        Mat map1(inputImage.rows, inputImage.cols, CV_32FC1);
        Mat map2(inputImage.rows, inputImage.cols, CV_32FC1);
        operateTransformToMat(map1, map2, transformMat);
        cv::remap(inputImage, outputImage, map1, map2, CV_INTER_CUBIC);
        imshow("polynomial transform", outputImage);
        waitKey(0);
    }

    void WarpImageUseFeatureMatchPolynomial(cv::Mat const &inputImage, cv::Mat const &templateImage,
                                            cv::Mat &outputImage, float zoom, std::string feature) {
        // input RGB image and template image, feature match, ransac, perspective warp image
        Mat inputImageRGB = inputImage.clone();
        Mat templateImageRGB = templateImage.clone();
        resize(inputImageRGB, inputImageRGB, Size(inputImageRGB.cols * zoom, inputImageRGB.rows * zoom));
        resize(templateImageRGB, templateImageRGB, Size(templateImageRGB.cols * zoom, templateImageRGB.rows * zoom));


        // feature match
        vector<cv::KeyPoint> keyPtsTemplate, keyPtsInput;
        vector<DMatch> goodMatches;
        FeatureMatch(templateImageRGB, inputImageRGB, keyPtsTemplate, keyPtsInput, goodMatches, 3.0, feature);

        // RANSAC
        vector<KeyPoint> RkeyPtsTemplate, RkeyPtsInput;
        vector<DMatch> Rmatches;
        RansacMatch(keyPtsTemplate, keyPtsInput, goodMatches, RkeyPtsTemplate, RkeyPtsInput, Rmatches);

#if __SHOW_DEBUG__
        // show ransac matches
    Mat ransacMatchImage;
    cout << "ransac matches num : " << Rmatches.size() << endl;
    Mat RmatchesToShow;
    drawMatches(templateImageRGB, keyPtsTemplate, inputImageRGB, keyPtsInput, Rmatches, RmatchesToShow,
                Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    cv::resize(ransacMatchImage, ransacMatchImage, Size(720,360));
    imshow("ransac Matches", RmatchesToShow);
    waitKey(0);
#endif

        // convert to float coordinates in origin image
        vector<Point2f> conjugatePointsTemplate;
        vector<Point2f> conjugatePointsInput;

        for (DMatch match: Rmatches) {
            conjugatePointsTemplate.push_back(RkeyPtsTemplate[match.queryIdx].pt / zoom);
            conjugatePointsInput.push_back(RkeyPtsInput[match.trainIdx].pt / zoom);
        }

        Mat transformMat;
        getTransform12Param(conjugatePointsInput, conjugatePointsTemplate, transformMat);
        cout << transformMat << endl;

        // transform image
        Mat warpedImage;
        polynomialTransform(inputImageRGB, warpedImage, transformMat);

    }
}