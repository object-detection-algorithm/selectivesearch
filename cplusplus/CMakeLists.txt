cmake_minimum_required(VERSION 3.15)
project(cpluplus)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_PREFIX_PATH /home/zj/opencv/opencv-4.2.0/install)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("/home/zj/opencv/opencv-4.2.0/opencv/modules")
include_directories("/home/zj/opencv/opencv-4.2.0/opencv_contrib/modules")

MESSAGE("OpenCV version: ${OpenCV_VERSION}")
MESSAGE("OpenCV OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
MESSAGE("OpenCV OpenCV_LIBS: ${OpenCV_LIBS}")

#图分割算法
add_executable(cpluplus samples/graphsegmentation_custom.cpp
        include/segmentation.h
        include/graphsegment/graphsegmentation.h include/graphsegment/graphsegmentationimpl.h
        include/graphsegment/edge.h include/graphsegment/point_set_element.h include/graphsegment/point_set.h
        src/graphsegmentation.cpp src/graphsegmentationimpl.cpp src/point_set.cpp
        )
#add_executable(cpluplus samples/selectivesearchsegmentation_custom.cpp)
target_link_libraries(cpluplus ${OpenCV_LIBS})
