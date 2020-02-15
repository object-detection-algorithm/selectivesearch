//
// Created by zj on 2020/2/14.
//

#include "../include/selectivesearch/selective_search_segmentation_strategy_size_impl.h"

namespace segmentation {
    void SelectiveSearchSegmentationStrategySizeImpl::setImage(
            cv::InputArray img_, cv::InputArray, cv::InputArray sizes_, int /* image_id */) {
        cv::Mat img = img_.getMat();
        size_image = img.rows * img.cols;
        sizes = sizes_.getMat();
    }

    float SelectiveSearchSegmentationStrategySizeImpl::get(int r1, int r2) {

        int size_r1 = sizes.at<int>(r1);
        int size_r2 = sizes.at<int>(r2);

        return cv::max(cv::min(1.0f - (float) (size_r1 + size_r2) / (float) (size_image), 1.0f), 0.0f);
    }

    void SelectiveSearchSegmentationStrategySizeImpl::merge(int /* r1 */, int /* r2 */) {
        // Nothing to do (sizes are merged at parent level)
    }

}