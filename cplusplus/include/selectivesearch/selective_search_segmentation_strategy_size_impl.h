//
// Created by zj on 2020/2/14.
//

#ifndef CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_SIZE_IMPL_H
#define CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_SIZE_IMPL_H

#include "selective_search_segmentation_strategy_size.h"

namespace segmentation {
    class SelectiveSearchSegmentationStrategySizeImpl final : public SelectiveSearchSegmentationStrategySize {
    public:
        SelectiveSearchSegmentationStrategySizeImpl() {
            name_ = "SelectiveSearchSegmentationStrategySize";
        }

        virtual void
        setImage(cv::InputArray img, cv::InputArray regions, cv::InputArray sizes, int image_id = -1) override;

        virtual float get(int r1, int r2) override;

        virtual void merge(int r1, int r2) override;

    private:
        std::string name_;

        cv::Mat sizes;
        int size_image;
    };
}

#endif //CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_SIZE_IMPL_H
