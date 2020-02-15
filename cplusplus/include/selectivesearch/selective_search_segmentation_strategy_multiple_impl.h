//
// Created by zj on 2020/2/14.
//

#ifndef CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_MULTIPLE_IMPL_H
#define CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_MULTIPLE_IMPL_H

#include "selective_search_segmentation_strategy_multiple.h"

namespace segmentation {
    class SelectiveSearchSegmentationStrategyMultipleImpl final : public SelectiveSearchSegmentationStrategyMultiple {
    public:
        SelectiveSearchSegmentationStrategyMultipleImpl() {
            name_ = "SelectiveSearchSegmentationStrategyMultiple";
            weights_total = 0;
        }

        virtual void
        setImage(cv::InputArray img, cv::InputArray regions, cv::InputArray sizes, int image_id = -1) override;

        virtual float get(int r1, int r2) override;

        virtual void merge(int r1, int r2) override;

        virtual void addStrategy(std::shared_ptr<SelectiveSearchSegmentationStrategy> g, float weight) override;

        virtual void clearStrategies() override;

    private:
        std::string name_;
        std::vector<std::shared_ptr<SelectiveSearchSegmentationStrategy> > strategies;
        std::vector<float> weights;
        float weights_total;
    };

}

#endif //CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_MULTIPLE_IMPL_H
