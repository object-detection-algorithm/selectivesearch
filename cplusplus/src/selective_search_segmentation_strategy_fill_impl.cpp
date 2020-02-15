//
// Created by zj on 2020/2/15.
//

#include "../include/selectivesearch/selective_search_segmentation_strategy_fill_impl.h"

namespace segmentation {
    void SelectiveSearchSegmentationStrategyFillImpl::setImage(cv::InputArray img_, cv::InputArray regions_,
                                                               cv::InputArray sizes_, int /* image_id */) {
        cv::Mat img = img_.getMat();
        sizes = sizes_.getMat();
        cv::Mat regions = regions_.getMat();

        size_image = img.rows * img.cols;

        // Build initial bouding rects
        double min, max;
        minMaxLoc(regions, &min, &max);

        int nb_segs = (int) max + 1;

        // Build a list of points for each regions
        std::vector<std::vector<cv::Point> > points;

        points.resize(nb_segs);

        for (int i = 0; i < (int) regions.rows; i++) {
            const int *p = regions.ptr<int>(i);

            for (int j = 0; j < (int) regions.cols; j++) {
                points[p[j]].push_back(cv::Point(j, i));
            }
        }

        // Compute bounding rects for each regions
        bounding_rects.resize(nb_segs);

        for (int seg = 0; seg < nb_segs; seg++) {
            bounding_rects[seg] = cv::boundingRect(points[seg]);
        }
    }

    float SelectiveSearchSegmentationStrategyFillImpl::get(int r1, int r2) {

        int size_r1 = sizes.at<int>(r1);
        int size_r2 = sizes.at<int>(r2);
        int bounding_rect_size = (bounding_rects[r1] | bounding_rects[r2]).area();

        return cv::max(cv::min(1.0f - (float) (bounding_rect_size - size_r1 - size_r2) / (float) (size_image), 1.0f),
                       0.0f);
    }

    void SelectiveSearchSegmentationStrategyFillImpl::merge(int r1, int r2) {
        bounding_rects[r1] = bounding_rects[r1] | bounding_rects[r2];
        bounding_rects[r2] = bounding_rects[r1];
    }

}