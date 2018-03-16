//
// Created by creator on 18-3-13.
//

#include "CompareMats.h"
#include "tools.h"
#include <fstream>

using namespace cv;
using namespace std;


CompareMats::CompareMats(cv::Mat mat0,const cv::Mat mat1, const cv::Mat mat2) :
        _sameCount(0), _differentCount(0), _same(true) {
    ASSERT(!mat0.empty(), "mat0指定的图片为空");
    ASSERT(!mat1.empty(), "mat1指定的图片为空");
    ASSERT(!mat2.empty(), "mat2指定的图片为空");
    ASSERT(mat0.size() == mat1.size() && mat1.size() == mat2.size(), "图的大小不一致");
    ASSERT(mat0.type() == mat1.type() && mat1.type() == mat2.type(), "图的类型不一致");
    _origin = mat0;
    compare(mat1, mat2);
}

std::string CompareMats::report() {
    if(_same)
        return "是否相同:是, 相同点数量:" + to_string(_sameCount) + ", 不同点数量:" + to_string(_differentCount);
    else
        return "是否相同:否, 相同点数量:" + to_string(_sameCount) + ", 不同点数量:" + to_string(_differentCount);

}

bool CompareMats::same() {
    return _same;
}

void CompareMats::compare(const Mat mat1, const Mat mat2) {
    switch (mat1.type()) {
        case CV_8UC3://uchar3
        {
            _mask = mat1.clone();
            for (int i = 0; i < mat1.cols; i++) {
                for (int j = 0; j < mat1.rows; j++) {
                    const auto &c1 = mat1.at<Vec3b>(j, i);
                    const auto &c2 = mat2.at<Vec3b>(j, i);
                    if (equal(c1, c2)) {
                        _sameCount++;
                    } else {
                        _differentCount++;
                        _differentPoints.emplace_back(Point(i, j));

                        Vec3b c0 = _origin.at<Vec3b>(j,i);
                        bool tag0 = equal(c0,c1);
                        bool tag1 = equal(c0,c2);

                        setMaskColor(i,j,tag0,tag1);
                    }
                }
            }
            break;
        }
        case CV_8UC1://uchar
        {
            //合并为三通道的图像
            vector<Mat> masks;
            masks.push_back(mat1);
            masks.push_back(mat1);
            masks.push_back(mat1);
            merge(masks, _mask);

            for (int i = 0; i < mat1.cols; i++) {
                for (int j = 0; j < mat1.rows; j++) {
                    uchar c1 = mat1.at<uchar>(j, i);
                    uchar c2 = mat2.at<uchar>(j, i);
                    if (c1==c2) {
                        _sameCount++;
                    } else {
                        _differentCount++;
                        _differentPoints.emplace_back(Point(i, j));

                        uchar c0 = _origin.at<uchar>(j,i);
                        bool tag0 = (c0==c1);
                        bool tag1 = (c0==c2);
                        setMaskColor(i,j,tag0,tag1);
                    }
                }
            }
            break;
        }
        case CV_32FC2://float2
        {
            _mask = Mat(mat1.size(), CV_8UC3, Scalar(0, 0, 0));
            for (int i = 0; i < mat1.cols; i++) {
                for (int j = 0; j < mat1.rows; j++) {
                    const auto &c1 = mat1.at<Vec2f>(j, i);
                    const auto &c2= mat2.at<Vec2f>(j, i);
                    if (equal(c1,c2)) {
                        _sameCount++;
                    } else {
                        _differentCount++;
                        _differentPoints.emplace_back(Point(i, j));

                        Vec2f c0 = _origin.at<Vec2f>(j,i);
                        bool tag0 = equal(c0,c1);
                        bool tag1 = equal(c0,c2);
                        setMaskColor(i,j,tag0,tag1);
                    }
                }
            }
            break;
        }
        case CV_32FC1://float
        {
            _mask = Mat(mat1.size(), CV_8UC3, Scalar(0, 0, 0));
            for (int i = 0; i < mat1.cols; i++) {
                for (int j = 0; j < mat1.rows; j++) {
                    float c1 = mat1.at<float>(j, i);
                    float c2 = mat2.at<float>(j, i);
                    if (abs(c1 - c2) < 1e-5) {
                        _sameCount++;
                    } else {
                        _differentCount++;
                        _differentPoints.emplace_back(Point(i, j));

                        float c0 = _origin.at<float>(j,i);
                        bool tag0 = (abs(c0-c1)<1e-5);
                        bool tag1 = (abs(c0-c2)<1e-5);
                        setMaskColor(i,j,tag0,tag1);
                    }
                }
            }
            break;
        }
        case CV_32SC1://int
        {
            _mask = Mat(mat1.size(), CV_8UC3, Scalar(0, 0, 0));
            for (int i = 0; i < mat1.cols; i++) {
                for (int j = 0; j < mat1.rows; j++) {
                    int c1 = mat1.at<int>(j, i);
                    int c2 = mat2.at<int>(j, i);
                    if (c1 == c2) {
                        _sameCount++;
                    } else {
                        _differentCount++;
                        _differentPoints.emplace_back(Point(i, j));

                        int c0 = _origin.at<int>(j,i);
                        bool tag0 = (c0==c1);
                        bool tag1 = (c0==c2);
                        setMaskColor(i,j,tag0,tag1);
                    }
                }
            }
            break;
        }
        default:
            ASSERT(false, "不支持的Mat类型");
            break;
    }

    if (_differentCount > 0) {
        _same = false;
    }
}

Mat CompareMats::mask() {
    return _mask;
}

inline bool CompareMats::equal(cv::Vec3b a, cv::Vec3b b) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

inline bool CompareMats::equal(cv::Vec2f a, cv::Vec2f b) {
    return abs(a[0] - b[0]) < 1e-5 && abs(a[1] - b[1]) < 1e-5;
}

vector<cv::Point> CompareMats::points() {
    return _differentPoints;
}

long long CompareMats::sameCount() {
    return _sameCount;
}

long long CompareMats::differentCount() {
    return _differentCount;
}

void CompareMats::saveReport(std::string fileName) {
    ofstream o(fileName+".txt");

    ASSERT(o.is_open(), "新建文件失败");

    o << report() << endl;

    if (_differentPoints.empty()) {
        o.close();
        return;
    }

    o << "不同点坐标:" << endl;

    for (int i = 0; i < _differentPoints.size(); i++) {
        o << _differentPoints[i] << ",\t";
        if ((i+1) % 10 == 0)//存10个点就换行
            o << endl;
    }
    o.close();

    imwrite(fileName+".bmp",_mask);
    
    cout<<"报告已保存到"+fileName+".txt和"+fileName+".bmp"<<endl;
}

void CompareMats::setMaskColor(int i,int j,bool tag0, bool tag1) {
    if(!tag0 && !tag1){//两个都不同，标记为黄色点
        _mask.at<Vec3b>(j, i) = Vec3b(0, 255, 255);
    }
    else if(!tag0){//path1指定的图和原图不同
        _mask.at<Vec3b>(j, i) = Vec3b(0, 255, 0);
    }
    else if(!tag1){//path2指定的图和原图不同
        _mask.at<Vec3b>(j, i) = Vec3b(255, 0, 0);
    }
}
