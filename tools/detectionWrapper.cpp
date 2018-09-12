#include "detectionWrapper.h"

DetectionWrapper::DetectionWrapper(const cz::ml::Rect &detection) {
    m_det_rect = cv::Rect(detection.x, detection.y, detection.width, detection.height);
    m_det_cls = detection.type;
    m_det_score = detection.score;

    m_plate_rect = cv::Rect(0,0,0,0);
    m_speed = 0;
    m_plate_num = "";
    m_rectCenterX = 0;
    m_is_valid = true;
    m_carLaneId = -1;
}

DetectionWrapper::DetectionWrapper(DetectionWrapper const &detectionWrapper) {
    m_det_rect = detectionWrapper.m_det_rect;
    m_det_cls = detectionWrapper.m_det_cls;
    m_det_score = detectionWrapper.m_det_score;

    m_plate_rect = detectionWrapper.m_plate_rect;
    m_speed = detectionWrapper.m_speed;
    m_plate_num = detectionWrapper.m_plate_num;
    m_rectCenterX = detectionWrapper.m_rectCenterX;
    m_is_valid = detectionWrapper.m_is_valid;
    m_carLaneId = detectionWrapper.m_carLaneId;
}

DetectionWrapper& DetectionWrapper::operator=(const DetectionWrapper &detectionWrapper) {
    m_det_rect = detectionWrapper.m_det_rect;
    m_det_cls = detectionWrapper.m_det_cls;
    m_det_score = detectionWrapper.m_det_score;

    m_plate_rect = detectionWrapper.m_plate_rect;
    m_speed = detectionWrapper.m_speed;
    m_plate_num = detectionWrapper.m_plate_num;
    m_rectCenterX = detectionWrapper.m_rectCenterX;
    m_is_valid = detectionWrapper.m_is_valid;
    m_carLaneId = detectionWrapper.m_carLaneId;

    return *this;
}

DetectionWrapper::~DetectionWrapper() {}

void DetectionWrapper::setScore(const float &score) {
    m_det_score = score;
}
void DetectionWrapper::setRect(const cv::Rect &rect) {
    m_det_rect = rect;
}
void DetectionWrapper::setClass(const std::string &cls) {
    m_det_cls = cls;
}
float DetectionWrapper::getScore() const {
    return m_det_score;
}
cv::Rect DetectionWrapper::getRect() const {
    return m_det_rect;
}
std::string DetectionWrapper::getClass() const {
    return m_det_cls;
}

void DetectionWrapper::setCarLaneId(const int &lane_id) {
    m_carLaneId = lane_id;
}
int DetectionWrapper::getCarLaneId() const {
    return m_carLaneId;
}

void DetectionWrapper::setIsValidFlag(const bool &is_valid) {
    m_is_valid = is_valid;
}
bool DetectionWrapper::getIsValidFlag() const {
    return m_is_valid;
}

void DetectionWrapper::setCenterX(const int &rectCenterX) {
    m_rectCenterX = rectCenterX;
}
int DetectionWrapper::getCenterX() const {
    return m_rectCenterX;
}

void DetectionWrapper::setPlateNum(const std::string &pltNum) {
    m_plate_num = pltNum;
}
std::string DetectionWrapper::getPlateNum() const {
    return m_plate_num;
}

void DetectionWrapper::setPlateRect(const cv::Rect &rect) {
    m_plate_rect = rect;
}
cv::Rect DetectionWrapper::getPlateRect() const {
    return m_plate_rect;
}

void DetectionWrapper::setSpeed(const double &speed) {
    m_speed = speed;
}
double DetectionWrapper::getSpeed() const {
    return m_speed;
}

cv::Rect DetectionWrapper::getCvRectFromCzmlRect(const cz::ml::Rect &czml_rect) {
    return cv::Rect(czml_rect.x, czml_rect.y, czml_rect.width, czml_rect.height);
}
