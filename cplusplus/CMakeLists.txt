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
#add_executable(cpluplus samples/graphsegmentation_custom.cpp
#        include/segmentation.h
#        include/graphsegment/graphsegmentation.h include/graphsegment/graphsegmentationimpl.h
#        include/graphsegment/edge.h include/graphsegment/point_set_element.h include/graphsegment/point_set.h
#        src/graphsegmentation.cpp src/graphsegmentationimpl.cpp src/point_set.cpp
#        )
add_executable(cpluplus samples/selectivesearchsegmentation_custom.cpp
        include/selectivesearch/selectivesearchsegmentation.h
        include/selectivesearch/selectivesearchsegmentationimpl.h

        # Helpers
        include/selectivesearch/region.h include/selectivesearch/neighbor.h
        include/selectivesearch/selective_search_segmentation_strategy.h
        include/selectivesearch/rect_comparator.h

        # Strategy
        include/selectivesearch/selective_search_segmentation_strategy_color.h
        include/selectivesearch/selective_search_segmentation_strategy_size.h
        include/selectivesearch/selective_search_segmentation_strategy_texture.h
        include/selectivesearch/selective_search_segmentation_strategy_fill.h
        include/selectivesearch/selective_search_segmentation_strategy_multiple.h

        include/selectivesearch/selective_search_segmentation_strategy_color_impl.h
        include/selectivesearch/selective_search_segmentation_strategy_size_impl.h
        include/selectivesearch/selective_search_segmentation_strategy_texture_impl.h
        include/selectivesearch/selective_search_segmentation_strategy_fill_impl.h
        include/selectivesearch/selective_search_segmentation_strategy_multiple_impl.h

        src/selective_search_segmentation_strategy_color_impl.cpp
        src/selective_search_segmentation_strategy_color.cpp
        src/selective_search_segmentation_strategy_multiple.cpp
        src/selective_search_segmentation_strategy_multiple_impl.cpp
        src/selective_search_segmentation_strategy_size.cpp
        src/selective_search_segmentation_strategy_size_impl.cpp
        src/selective_search_segmentation_strategy_fill.cpp
        src/selective_search_segmentation_strategy_fill_impl.cpp
        src/selective_search_segmentation_strategy_texture.cpp
        src/selective_search_segmentation_strategy_texture_impl.cpp
        src/selectivesearchsegmentation.cpp
        src/selectivesearchsegmentationimpl.cpp
        src/region.cpp
        src/neighbor.cpp

        # 图分割算法
        include/graphsegment/graphsegmentation.h include/graphsegment/graphsegmentationimpl.h
        include/graphsegment/edge.h include/graphsegment/point_set_element.h include/graphsegment/point_set.h
        src/graphsegmentation.cpp src/graphsegmentationimpl.cpp src/point_set.cpp
        )
target_link_libraries(cpluplus ${OpenCV_LIBS})
