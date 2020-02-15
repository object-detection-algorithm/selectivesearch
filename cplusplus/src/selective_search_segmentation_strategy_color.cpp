//
// Created by zj on 2020/2/14.
//

#include "../include/selectivesearch/selective_search_segmentation_strategy_color.h"
#include "../include/selectivesearch/selective_search_segmentation_strategy_color_impl.h"

namespace segmentation {
    std::shared_ptr<SelectiveSearchSegmentationStrategyColor> createSelectiveSearchSegmentationStrategyColor() {
        std::shared_ptr<SelectiveSearchSegmentationStrategyColor> s = std::make_shared<SelectiveSearchSegmentationStrategyColorImpl>();
        return s;
    }
}