//
// Created by zj on 2020/2/14.
//

#include "../include/selectivesearch/selective_search_segmentation_strategy_size.h"
#include "../include/selectivesearch/selective_search_segmentation_strategy_size_impl.h"

namespace segmentation {
    std::shared_ptr<SelectiveSearchSegmentationStrategySize> createSelectiveSearchSegmentationStrategySize() {
        std::shared_ptr<SelectiveSearchSegmentationStrategySize> s = std::make_shared<SelectiveSearchSegmentationStrategySizeImpl>();
        return s;
    }
}
