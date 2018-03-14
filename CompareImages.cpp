//
// Created by creator on 18-3-13.
//

#include "CompareImages.h"
#include "tools.h"

using namespace cv;
using namespace std;


CompareImages::CompareImages(const cv::Mat mat1, const cv::Mat mat2) :
        _sameCount(0), _differentCount(0), _same(true) {
    ASSERT(!mat1.empty(),"图片为空");
    ASSERT(mat1.size() == mat2.size(), "两图大小不一致");
    ASSERT(mat1.type() == mat2.type(), "两图类型不一致");
//    _mask = Mat(mat1.size(),CV_8UC3,Scalar(0,0,0));
    _mask = mat1.clone();
    compare(mat1, mat2);
}

std::string CompareImages::report() {
    return "是否相同:" + to_string(_same) + ", 相同点数量:" + to_string(_sameCount) + ", 不同点数量:" + to_string(_differentCount);
}

bool CompareImages::same() {
    return _same;
}

void CompareImages::compare(const Mat mat1, const Mat mat2) {
    switch (mat1.type()){
        case CV_8UC3:
            for (int i = 0; i < mat1.cols; i++) {
                for (int j = 0; j < mat1.rows; j++) {
                    if (equal(mat1.at<Vec3b>(j, i), mat2.at<Vec3b>(j, i))) {
                        _sameCount++;
                    } else {
                        _differentCount++;
                        _differentPoints.emplace_back(Point(i, j));
                        _mask.at<Vec3b>(j, i) = Vec3b(0, 255, 255);
                    }
                }
            }
            break;
        case CV_8UC1:
            for (int i = 0; i < mat1.cols; i++) {
                for (int j = 0; j < mat1.rows; j++) {
                    if (mat1.at<uchar>(j, i) == mat2.at<uchar>(j, i)) {
                        _sameCount++;
                    } else {
                        _differentCount++;
                        _differentPoints.emplace_back(Point(i, j));
                        _mask.at<uchar>(j, i) = 255;
                    }
                }
            }
            break;
        case CV_32FC2:
            for (int i = 0; i < mat1.cols; i++) {
                for (int j = 0; j < mat1.rows; j++) {
                    if (equal(mat1.at<Vec2f>(j, i), mat2.at<Vec2f>(j, i))) {
                        _sameCount++;
                    } else {
                        _differentCount++;
                        _differentPoints.emplace_back(Point(i, j));
                        _mask.at<Vec2f>(j, i) = Vec2f(0, 0);
                    }
                }
            }
            break;
        default:
            ASSERT(false, "不支持的图片类型");
            break;
    }

    if (_differentCount > 0) {
        _same = false;
    }
}

cv::Mat CompareImages::mask() {
    return _mask;
}

inline bool CompareImages::equal(cv::Vec3b a, cv::Vec3b b) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

inline bool CompareImages::equal(cv::Vec2f a, cv::Vec2f b) {
    return abs(a[0]-b[0])<1e-5 && abs(a[1]-b[1])<1e-5;
}
