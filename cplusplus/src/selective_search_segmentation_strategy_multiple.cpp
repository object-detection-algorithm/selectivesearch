//
// Created by zj on 2020/2/14.
//

#include "../include/selectivesearch/selective_search_segmentation_strategy_multiple.h"
#include "../include/selectivesearch//selective_search_segmentation_strategy_multiple_impl.h"

namespace segmentation {
    std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple> createSelectiveSearchSegmentationStrategyMultiple() {
        std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple> s = std::make_shared<SelectiveSearchSegmentationStrategyMultipleImpl>();
        return s;
    }

    // Helpers to quickly create a multiple stragegy with 1 to 4 equal strageries
    std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple>
    createSelectiveSearchSegmentationStrategyMultiple(std::shared_ptr<SelectiveSearchSegmentationStrategy> s1) {
        std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple> s = std::make_shared<SelectiveSearchSegmentationStrategyMultipleImpl>();

        s->addStrategy(s1, 1.0f);

        return s;
    }

    std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple>
    createSelectiveSearchSegmentationStrategyMultiple(std::shared_ptr<SelectiveSearchSegmentationStrategy> s1,
                                                      std::shared_ptr<SelectiveSearchSegmentationStrategy> s2) {
        std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple> s = std::make_shared<SelectiveSearchSegmentationStrategyMultipleImpl>();

        s->addStrategy(s1, 0.5f);
        s->addStrategy(s2, 0.5f);

        return s;
    }

    std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple>
    createSelectiveSearchSegmentationStrategyMultiple(std::shared_ptr<SelectiveSearchSegmentationStrategy> s1,
                                                      std::shared_ptr<SelectiveSearchSegmentationStrategy> s2,
                                                      std::shared_ptr<SelectiveSearchSegmentationStrategy> s3) {
        std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple> s = std::make_shared<SelectiveSearchSegmentationStrategyMultipleImpl>();

        s->addStrategy(s1, 0.3333f);
        s->addStrategy(s2, 0.3333f);
        s->addStrategy(s3, 0.3333f);

        return s;
    }

    std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple>
    createSelectiveSearchSegmentationStrategyMultiple(std::shared_ptr<SelectiveSearchSegmentationStrategy> s1,
                                                      std::shared_ptr<SelectiveSearchSegmentationStrategy> s2,
                                                      std::shared_ptr<SelectiveSearchSegmentationStrategy> s3,
                                                      std::shared_ptr<SelectiveSearchSegmentationStrategy> s4) {
        std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple> s = std::make_shared<SelectiveSearchSegmentationStrategyMultipleImpl>();

        s->addStrategy(s1, 0.25f);
        s->addStrategy(s2, 0.25f);
        s->addStrategy(s3, 0.25f);
        s->addStrategy(s4, 0.25f);

        return s;
    }

}