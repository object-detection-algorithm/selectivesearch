//
// Created by zj on 2020/2/14.
//

#ifndef CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_TEXTURE_H
#define CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_TEXTURE_H

#include "selective_search_segmentation_strategy.h"

namespace segmentation {
    /** @brief Texture-based strategy for the selective search segmentation algorithm
    The class is implemented from the algorithm described in @cite uijlings2013selective.
    */
    class SelectiveSearchSegmentationStrategyTexture : public SelectiveSearchSegmentationStrategy {
    };

    /** @brief Create a new size-based strategy */
    std::shared_ptr<SelectiveSearchSegmentationStrategyTexture> createSelectiveSearchSegmentationStrategyTexture();
}

#endif //CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_TEXTURE_H
