//
// Created by zj on 2020/2/14.
//

#ifndef CPLUPLUS_SELECTIVESEARCHSEGMENTATION_H
#define CPLUPLUS_SELECTIVESEARCHSEGMENTATION_H

#include "selective_search_segmentation_strategy.h"
#include "../segmentation.h"
#include <opencv2/opencv.hpp>

namespace segmentation {
    /** @brief Selective search segmentation algorithm
        The class implements the algorithm described in @cite uijlings2013selective.
    */
    class SelectiveSearchSegmentation {
    public:

        /** @brief Set a image used by switch* functions to initialize the class
            @param img The image
        */
        virtual void setBaseImage(cv::InputArray img) = 0;

        /** @brief Initialize the class with the 'Single stragegy' parameters describled in @cite uijlings2013selective.
            @param k The k parameter for the graph segmentation
            @param sigma The sigma parameter for the graph segmentation
        */
        virtual void switchToSingleStrategy(int k = 200, float sigma = 0.8f) = 0;

        /** @brief Initialize the class with the 'Selective search fast' parameters describled in @cite uijlings2013selective.
            @param base_k The k parameter for the first graph segmentation
            @param inc_k The increment of the k parameter for all graph segmentations
            @param sigma The sigma parameter for the graph segmentation
        */
        virtual void switchToSelectiveSearchFast(int base_k = 150, int inc_k = 150, float sigma = 0.8f) = 0;

        /** @brief Initialize the class with the 'Selective search fast' parameters describled in @cite uijlings2013selective.
            @param base_k The k parameter for the first graph segmentation
            @param inc_k The increment of the k parameter for all graph segmentations
            @param sigma The sigma parameter for the graph segmentation
        */
        virtual void switchToSelectiveSearchQuality(int base_k = 150, int inc_k = 150, float sigma = 0.8f) = 0;

        /** @brief Add a new image in the list of images to process.
            @param img The image
        */
        virtual void addImage(cv::InputArray img) = 0;

        /** @brief Clear the list of images to process
        */
        virtual void clearImages() = 0;

        /** @brief Add a new graph segmentation in the list of graph segementations to process.
            @param g The graph segmentation
        */
        virtual void addGraphSegmentation(std::shared_ptr<GraphSegmentation> g) = 0;

        /** @brief Clear the list of graph segmentations to process;
        */
        virtual void clearGraphSegmentations() = 0;

        /** @brief Add a new strategy in the list of strategy to process.
            @param s The strategy
        */
        virtual void addStrategy(std::shared_ptr<SelectiveSearchSegmentationStrategy> s) = 0;

        /** @brief Clear the list of strategy to process;
        */
        virtual void clearStrategies() = 0;

        /** @brief Based on all images, graph segmentations and stragies, computes all possible rects and return them
            @param rects The list of rects. The first ones are more relevents than the lasts ones.
        */
        virtual void process(CV_OUT std::vector<cv::Rect> &rects) = 0;
    };

    /** @brief Create a new SelectiveSearchSegmentation class.
    */
    std::shared_ptr<SelectiveSearchSegmentation> createSelectiveSearchSegmentation();
}

#endif //CPLUPLUS_SELECTIVESEARCHSEGMENTATION_H
