//
// Created by zj on 2020/2/14.
//

#ifndef CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_SIZE_H
#define CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_SIZE_H

#include "selective_search_segmentation_strategy.h"

namespace segmentation {
    /** @brief Size-based strategy for the selective search segmentation algorithm
    The class is implemented from the algorithm described in @cite uijlings2013selective.
    */
    class SelectiveSearchSegmentationStrategySize : public SelectiveSearchSegmentationStrategy {
    };

    /** @brief Create a new size-based strategy */
    std::shared_ptr<SelectiveSearchSegmentationStrategySize> createSelectiveSearchSegmentationStrategySize();
}

#endif //CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_SIZE_H
