//
// Created by zj on 2020/2/14.
//

#ifndef CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_FILL_IMPL_H
#define CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_FILL_IMPL_H

#include "selective_search_segmentation_strategy_fill.h"

namespace segmentation {
    class SelectiveSearchSegmentationStrategyFillImpl final : public SelectiveSearchSegmentationStrategyFill {
    public:
        SelectiveSearchSegmentationStrategyFillImpl() {
            name_ = "SelectiveSearchSegmentationStrategyFill";
        }

        virtual void
        setImage(cv::InputArray img, cv::InputArray regions, cv::InputArray sizes, int image_id = -1) override;

        virtual float get(int r1, int r2) override;

        virtual void merge(int r1, int r2) override;

    private:
        std::string name_;

        cv::Mat sizes;
        int size_image;
        std::vector<cv::Rect> bounding_rects;
    };
}

#endif //CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_FILL_IMPL_H
