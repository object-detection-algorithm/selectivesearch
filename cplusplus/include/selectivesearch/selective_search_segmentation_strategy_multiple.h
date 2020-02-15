//
// Created by zj on 2020/2/14.
//

#ifndef CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_MULTIPLE_H
#define CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_MULTIPLE_H

#include "selective_search_segmentation_strategy.h"

namespace segmentation {

    /** @brief Regroup multiple strategies for the selective search segmentation algorithm
    */
    class SelectiveSearchSegmentationStrategyMultiple : public SelectiveSearchSegmentationStrategy {
    public:

        /** @brief Add a new sub-strategy
            @param g The strategy
            @param weight The weight of the strategy
        */
        virtual void addStrategy(std::shared_ptr<SelectiveSearchSegmentationStrategy> g, float weight) = 0;

        /** @brief Remove all sub-strategies
        */
        virtual void clearStrategies() = 0;
    };

    /** @brief Create a new multiple strategy */
    std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple> createSelectiveSearchSegmentationStrategyMultiple();

    /** @brief Create a new multiple strategy and set one subtrategy
        @param s1 The first strategy
    */
    std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple>
    createSelectiveSearchSegmentationStrategyMultiple(std::shared_ptr<SelectiveSearchSegmentationStrategy> s1);

    /** @brief Create a new multiple strategy and set two subtrategies, with equal weights
        @param s1 The first strategy
        @param s2 The second strategy
    */
    std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple>
    createSelectiveSearchSegmentationStrategyMultiple(std::shared_ptr<SelectiveSearchSegmentationStrategy> s1,
                                                      std::shared_ptr<SelectiveSearchSegmentationStrategy> s2);


    /** @brief Create a new multiple strategy and set three subtrategies, with equal weights
        @param s1 The first strategy
        @param s2 The second strategy
        @param s3 The third strategy
    */
    std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple>
    createSelectiveSearchSegmentationStrategyMultiple(std::shared_ptr<SelectiveSearchSegmentationStrategy> s1,
                                                      std::shared_ptr<SelectiveSearchSegmentationStrategy> s2,
                                                      std::shared_ptr<SelectiveSearchSegmentationStrategy> s3);

    /** @brief Create a new multiple strategy and set four subtrategies, with equal weights
        @param s1 The first strategy
        @param s2 The second strategy
        @param s3 The third strategy
        @param s4 The forth strategy
    */
    std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple>
    createSelectiveSearchSegmentationStrategyMultiple(std::shared_ptr<SelectiveSearchSegmentationStrategy> s1,
                                                      std::shared_ptr<SelectiveSearchSegmentationStrategy> s2,
                                                      std::shared_ptr<SelectiveSearchSegmentationStrategy> s3,
                                                      std::shared_ptr<SelectiveSearchSegmentationStrategy> s4);
}

#endif //CPLUPLUS_SELECTIVE_SEARCH_SEGMENTATION_STRATEGY_MULTIPLE_H
