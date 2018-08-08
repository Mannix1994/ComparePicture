
#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;

/**
 * condition为false时抛出错误，错误信息为error_message
 */
#define ASSERT(condition,error_message) ((condition)?0:(assertion(__FILE__, __func__, __LINE__,error_message),-1))

void assertion(const std::string &filePath, const std::string &function, int line, const std::string &info);

#endif //TOOLS_H
