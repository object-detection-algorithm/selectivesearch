//
// Created by zj on 2020/2/14.
//

#include "../include/selectivesearch/selective_search_segmentation_strategy_fill.h"
#include "../include/selectivesearch/selective_search_segmentation_strategy_fill_impl.h"

namespace segmentation{

    std::shared_ptr<SelectiveSearchSegmentationStrategyFill> createSelectiveSearchSegmentationStrategyFill() {
        std::shared_ptr<SelectiveSearchSegmentationStrategyFill> s = std::make_shared<SelectiveSearchSegmentationStrategyFillImpl>();
        return s;
    }

}