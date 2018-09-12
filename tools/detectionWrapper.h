#ifndef CUIZHOUSECURITY_DETECTIONWRAPPER_H
#define CUIZHOUSECURITY_DETECTIONWRAPPER_H

#include <czml/detectors/Detection.h>

class DetectionWrapper {
public:
    DetectionWrapper() = default;
    DetectionWrapper(const cz::ml::Rect &detection);
    DetectionWrapper(DetectionWrapper const& detectionWrapper);
    DetectionWrapper& operator=(const DetectionWrapper &detectionWrapper);

    static cv::Rect getCvRectFromCzmlRect(const cz::ml::Rect &czml_rect);

    ~DetectionWrapper();

public:
    std::string getClass() const;
    cv::Rect getRect() const;
    float getScore() const;

    void setClass(const std::string& cls);
    void setRect(const cv::Rect& rect);
    void setScore(const float& score);

public:
    double getSpeed() const;
    cv::Rect getPlateRect() const;
    std::string getPlateNum() const;
    int getCenterX() const;
    bool getIsValidFlag() const;
    int getCarLaneId() const;

    void setSpeed(const double& speed);
    void setPlateRect(const cv::Rect &rect);
    void setPlateNum(const std::string &pltNum);
    void setCenterX(const int &rectCenterX);
    void setIsValidFlag(const bool &is_valid);
    void setCarLaneId(const int &lane_id);

private:
    cv::Rect m_det_rect;
    std::string m_det_cls;
    float m_det_score;

    double m_speed;
    cv::Rect m_plate_rect;
    std::string m_plate_num;

    int m_rectCenterX;
    bool m_is_valid;
    int m_carLaneId;
};


#endif //CUIZHOUSECURITY_DETECTIONWRAPPER_H
