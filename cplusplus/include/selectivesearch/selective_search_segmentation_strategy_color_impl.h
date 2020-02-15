//
// Created by zj on 2020/2/14.
//

#ifndef CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_COLOR_IMPL_H
#define CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_COLOR_IMPL_H

#include <opencv2/opencv.hpp>
#include "selective_search_segmentation_strategy_color.h"

namespace segmentation {
    class SelectiveSearchSegmentationStrategyColorImpl final : public SelectiveSearchSegmentationStrategyColor {
    public:
        SelectiveSearchSegmentationStrategyColorImpl() {
            name_ = "SelectiveSearchSegmentationStrategyColor";
            last_image_id = -1;
        }

        virtual void
        setImage(cv::InputArray img, cv::InputArray regions, cv::InputArray sizes, int image_id = -1) override;

        virtual float get(int r1, int r2) override;

        virtual void merge(int r1, int r2) override;

    private:
        std::string name_;

        cv::Mat histograms; // [Region X Histogram]
        cv::Mat sizes;
        int histogram_size{};

        int last_image_id; // If the image_id is not equal to -1 and the same as the previous call for setImage, computations are used again
        cv::Mat last_histograms;
    };

}

#endif //CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_COLOR_IMPL_H
