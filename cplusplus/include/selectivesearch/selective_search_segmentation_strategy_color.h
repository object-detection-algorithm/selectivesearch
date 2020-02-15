//
// Created by zj on 2020/2/14.
//

#ifndef CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_COLOR_H
#define CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_COLOR_H

#include "selective_search_segmentation_strategy.h"
#include <iostream>

namespace segmentation {

    /** @brief Color-based strategy for the selective search segmentation algorithm
    The class is implemented from the algorithm described in @cite uijlings2013selective.
    */
    class SelectiveSearchSegmentationStrategyColor : public SelectiveSearchSegmentationStrategy {};

    /** @brief Create a new color-based strategy */
    std::shared_ptr<SelectiveSearchSegmentationStrategyColor> createSelectiveSearchSegmentationStrategyColor();

}

#endif //CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_COLOR_H
