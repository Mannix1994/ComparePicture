# required cmake version
cmake_minimum_required(VERSION 2.8)
# CMakeLists.txt for G4CU project
project(ComparePicture)

set(CMAKE_CXX_COMPILER clang++)
#set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++11")

# packages
#set(OpenCV_DIR "/usr/local/include") 	    #修改了这一句，指定opencv头文件的路径
#set(OpenCV_DIR "/usr/local/opencv331/share/OpenCV")
find_package(OpenCV REQUIRED)


#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /home/creator/Projects/C++/REN/run/数据)	#这一句指定可执行文件的位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../run)	#这一句指定可执行文件的位置

include_directories(${OPENCV_INCLUDE_DIRS})
#include_directories("/usr/local/include/freetype2")

file(GLOB CURRENT_HEADERS  include/*.* include/lib/*.h)
file(GLOB CURRENT_SOURCES  source/*.* *.cpp)


add_executable(${PROJECT_NAME} ${CURRENT_HEADERS} ${CURRENT_SOURCES})

target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS}
#        freetype
        )