//
// Created by wurui on 18-6-21.
//

#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include "opencv2/opencv.hpp"
#include "detectionWrapper.h"
//#include "detector/DeepNetDetector.h"
//#include "cvRect_pro.h"
//#include "detection_pro.h"
#include "Markup.h"

using namespace std;
//using namespace boost::filesystem;
using namespace cv;
//
//// global var
//DeepNetDetector detector_roi = DeepNetDetector();
//
//void init(string mode) {
////    detector_roi.init("/home/wurui/gitREPO/ocr-nameplates-cuizhou/model/dz/key/test.prototxt",
////                      "/home/wurui/gitREPO/ocr-nameplates-cuizhou/model/dz/key/dz_key_100000.caffemodel",
////                      "/home/wurui/gitREPO/ocr-nameplates-cuizhou/model/dz/key/classes_name.txt");
//    if (mode=="pva") {
//        detector_roi.init("/home/wurui/gitREPO/ocr-nameplates-cuizhou/model/dz/value-v2/test.prototxt",
//                          "/home/wurui/gitREPO/ocr-nameplates-cuizhou/model/dz/value-v2/dz-value-v2_iter_100000.caffemodel",
//                          "/home/wurui/gitREPO/ocr-nameplates-cuizhou/model/dz/value-v2/classes_name.txt",
//        "PvaNetDetector");
////        detector_roi.init("/home/wurui/gitREPO/ocr-nameplates-cuizhou/model/dz/value/test.prototxt",
////                          "/home/wurui/gitREPO/ocr-nameplates-cuizhou/model/dz/value/DzValueII_iter_130000.caffemodel",
////                          "/home/wurui/gitREPO/ocr-nameplates-cuizhou/model/dz/value//classes_name.txt",
////        "PvaNetDetector");
////        detector_roi.init("/home/wurui/gitREPO/ocr-nameplates-cuizhou/model/lincon/value-v3+/test.prototxt",
////                          "/home/wurui/gitREPO/ocr-nameplates-cuizhou/model/lincon/value-v3+/lincon-value-v3+_iter_150000.caffemodel",
////                          "/home/wurui/gitREPO/ocr-nameplates-cuizhou/model/lincon/value-v3+/classes_name.txt",
////        "PvaNetDetector");
//    } else if (mode=="ssd") {
//        detector_roi.init("/home/wurui/dl_rpository/easy-train-ssd-classify/ssd_train/train_output/mbv1_ssd/final_res/MobileNetSSD_deploy.prototxt",
//                          "/home/wurui/dl_rpository/easy-train-ssd-classify/ssd_train/train_output/mbv1_ssd/final_res/mbv1_ssd_deploy.caffemodel",
//                          "/home/wurui/dl_rpository/easy-train-ssd-classify/ssd_train/train_output/mbv1_ssd/classes_name.txt",
//        "MobileNetDetector");
//    }
//}

void outPutXml(std::vector<DetectionWrapper> &dets, string output_root_path, string filename, int imgWidth, int imgHeight)
{
    CMarkup xml;
    xml.AddElem("annotation");
    xml.IntoElem();

    xml.AddElem("folder", "img");
    xml.AddElem("filename", filename);
    xml.AddElem("path", "./");

    xml.AddElem("source");
    xml.IntoElem();
    xml.AddElem("database", "Unknown");
    xml.OutOfElem();

    xml.AddElem("size");
    xml.IntoElem();
    xml.AddElem("width", imgWidth);
    xml.AddElem("height", imgHeight);
    xml.AddElem("depth", 3);
    xml.OutOfElem();

    xml.AddElem("segmented", 0);

    for(int i = 0; i < dets.size(); ++i) {
        xml.AddElem("object");
        xml.IntoElem();
        xml.AddElem("name", dets[i].getClass());
        xml.AddElem("pose", "Unspecified");
        xml.AddElem("truncated", 0);
        xml.AddElem("difficult", 0);
        xml.AddElem("bndbox");
        xml.IntoElem();
        cv::Rect rect = dets[i].getRect();
        xml.AddElem("xmin", rect.x);
        xml.AddElem("ymin", rect.y);
        xml.AddElem("xmax", rect.x + rect.width);
        xml.AddElem("ymax", rect.y + rect.height);
        xml.OutOfElem();
        xml.OutOfElem();
    }
    xml.Save(output_root_path + filename.substr(0, filename.length() - 4) + ".xml");
}
//
//void DetectAndOutputXml(){
//    string imgDir = "/home/wurui/Desktop/dz/value-v3/img/";
//    string xmlDir = "/home/wurui/Desktop/dz/value-v3/xml/";
//    int count = 0;
//    for(directory_iterator itr = directory_iterator(imgDir); itr != directory_iterator(); ++itr) {
//        if(count%4!=0){
//            continue;
//        }
//        string imgPath = itr->path().string();
//        string imgName = itr->path().filename().string();
//        Mat image = cv::imread(imgPath, cv::IMREAD_COLOR);
//        if (image.empty()) {
//            cout << "imread empty: " << imgPath << endl;
//            continue;
//        }
//        cout << "Processing img: " << imgName << endl;
//
//        // detect
//        vector<DetectionWrapper> detections;
//        detector_roi.detect(image, detections, 0.6);
//        if (detections.size() < 1) {
//            cout << "no detection !" << endl;
//            continue;
//        }
//        cz::eliminateDetectionsByNMS(detections, 0.45);
//
//        // output xml
//        outPutXml(detections, xmlDir, imgName, image.cols, image.rows);
//
//        // display
//        cz::drawDetections(image, detections);
//        cv::imwrite("/media/wurui/WorkSpace/OCR/CarBrandData/binli/testImg-1/result-enhance/"+imgName, image);
//        cv::imshow("", image);
//        cv::waitKey(0);
//    }
//
//}
//
//int main() {
//    init("pva");
//    DetectAndOutputXml();
//}

int main() {
    DetectionWrapper det = DetectionWrapper();
    det.setClass("target");
    det.setRect(cv::Rect(10,10,15,20));
    det.setScore(0.9);
    vector<DetectionWrapper> dets(3, det);
    outPutXml(dets, "./", "result.xml", 1920,1080);
}

