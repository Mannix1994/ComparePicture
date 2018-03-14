//
// Created by creator on 18-3-13.
//

#ifndef COMPAREIMAGES_H
#define COMPAREIMAGES_H

#include <opencv2/opencv.hpp>

class CompareImages {
public:
    CompareImages(cv::Mat mat1, cv::Mat mat2);
    std::string report();
    bool same();
    cv::Mat mask();

private:
    void compare(cv::Mat mat1, cv::Mat mat2);
    inline bool equal(cv::Vec3b a, cv::Vec3b b);
    inline bool equal(cv::Vec2f a, cv::Vec2f b);

private:
    long long _sameCount;
    long long _differentCount;
    bool _same;
    std::vector<cv::Point> _differentPoints;
    cv::Mat _mask;
};


#endif //COMPAREIMAGES_H
