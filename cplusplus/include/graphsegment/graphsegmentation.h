//
// Created by zj on 2020/2/12.
//

#ifndef CPLUPLUS_GRAPHSEGMENTATION_H
#define CPLUPLUS_GRAPHSEGMENTATION_DEMO_H


#include <memory>
#include <opencv2/core.hpp>
// Helpers
#include "edge.h"
#include "point_set_element.h"

namespace segmentation {
    //! @addtogroup ximgproc_segmentation
    //! @{
    /** @brief Graph Based Segmentation Algorithm.
        The class implements the algorithm described in @cite PFF2004 .
     */
    class GraphSegmentation {
    public:

        /** @brief Segment an image and store output in dst
            @param src The input image. Any number of channel (1 (Eg: Gray), 3 (Eg: RGB), 4 (Eg: RGB-D)) can be provided
            @param dst The output segmentation. It's a CV_32SC1 Mat with the same number of cols and rows as input image, with an unique, sequential, id for each pixel.
        */
        virtual void processImage(cv::InputArray src, cv::OutputArray dst) = 0;

        virtual void setSigma(double sigma) = 0;

        virtual double getSigma() = 0;

        virtual void setK(float k) = 0;

        virtual float getK() = 0;

        virtual void setMinSize(int min_size) = 0;

        virtual int getMinSize() = 0;
    };

/** @brief Creates a graph based segmentor
    @param sigma The sigma parameter, used to smooth image
    @param k The k parameter of the algorythm
    @param min_size The minimum size of segments
 */
    std::shared_ptr<GraphSegmentation> createGraphSegmentation(double sigma = 0.5, float k = 300, int min_size = 100);
}

#endif //CPLUPLUS_GRAPHSEGMENTATION_H
