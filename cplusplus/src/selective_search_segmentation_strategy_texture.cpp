//
// Created by zj on 2020/2/15.
//

#include "../include/selectivesearch/selective_search_segmentation_strategy_texture.h"
#include "../include/selectivesearch/selective_search_segmentation_strategy_texture_impl.h"

namespace segmentation {
    std::shared_ptr<SelectiveSearchSegmentationStrategyTexture> createSelectiveSearchSegmentationStrategyTexture() {
        std::shared_ptr<SelectiveSearchSegmentationStrategyTexture> s = std::make_shared<SelectiveSearchSegmentationStrategyTextureImpl>();
        return s;
    }
}