//
// Created by zj on 2020/2/14.
//

#ifndef CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_H
#define CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_H

#include <opencv2/opencv.hpp>

namespace segmentation {
    class SelectiveSearchSegmentationStrategy {
    public:
        /** @brief Set a initial image, with a segmentation.
            @param img The input image. Any number of channel can be provided
            @param regions A segmentation of the image. The parameter must be the same size of img.
            @param sizes The sizes of different regions
            @param image_id If not set to -1, try to cache pre-computations. If the same set og (img, regions, size) is used, the image_id need to be the same.
        */
        virtual void setImage(cv::InputArray img, cv::InputArray regions, cv::InputArray sizes, int image_id = -1) = 0;

        /** @brief Return the score between two regions (between 0 and 1)
            @param r1 The first region
            @param r2 The second region
        */
        virtual float get(int r1, int r2) = 0;

        /** @brief Inform the strategy that two regions will be merged
            @param r1 The first region
            @param r2 The second region
        */
        virtual void merge(int r1, int r2) = 0;
    };

}

#endif //CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_H
