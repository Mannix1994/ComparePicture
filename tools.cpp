#include "tools.h"

void assertion(const std::string &filePath, const std::string &function,
                     int line, const std::string &info){
    //获取文件名
    unsigned long pos=filePath.find_last_of('/');
    std::string filename(filePath.substr(pos+1));
    std::string err = filename+" "+function+" "+std::to_string(line)+">>  "+info;
    //抛出错误
#ifndef NDEBUG
    throw std::runtime_error(err);
#else
    std::cerr<<err<<std::endl;
    exit(-1);
#endif
}