//
// Created by zj on 2020/2/14.
//

#include "../include/selectivesearch/selective_search_segmentation_strategy_color_impl.h"

namespace segmentation {
    void SelectiveSearchSegmentationStrategyColorImpl::setImage(cv::InputArray img_, cv::InputArray regions_, cv::InputArray sizes_,
                                                                int image_id) {

        cv::Mat img = img_.getMat();
        cv::Mat regions = regions_.getMat();
        sizes = sizes_.getMat();

        if (image_id == -1 || last_image_id != image_id) {

            std::vector<cv::Mat> img_planes;
            split(img, img_planes);

            int histogram_bins_size = 25;

            float range[] = {0, 256};
            const float *histogram_ranges = {range};

            double min, max;
            minMaxLoc(regions, &min, &max);
            int nb_segs = (int) max + 1;

            histogram_size = histogram_bins_size * img.channels();

            histograms = cv::Mat_<float>(nb_segs, histogram_size);

            for (int r = 0; r < nb_segs; r++) {

                // Generate mask
                cv::Mat mask = regions == r;

                // Compute histogram for each channels
                float tt = 0;

                cv::Mat tmp_hists = cv::Mat(histogram_size, 1, CV_32F);
                float *tmp_histogram = tmp_hists.ptr<float>(0);
                int h_pos = 0;
                cv::Mat tmp_hist;

                for (int p = 0; p < img.channels(); p++) {

                    calcHist(&img_planes[p], 1, 0, mask, tmp_hist, 1, &histogram_bins_size, &histogram_ranges);

                    float *tmp_hist_ = tmp_hist.ptr<float>(0);

                    // Copy local histogram to global histogram
                    for (int pos = 0; pos < histogram_bins_size; pos++) {
                        tmp_histogram[pos + h_pos] = tmp_hist_[pos];
                        tt += tmp_histogram[pos + h_pos];
                    }
                    h_pos += histogram_bins_size;
                }

                // Normalize historgrams
                float *histogram = histograms.ptr<float>(r);

                for (int h_pos2 = 0; h_pos2 < histogram_size; h_pos2++) {
                    histogram[h_pos2] = tmp_histogram[h_pos2] / tt;
                }
            }

            // Save cache if we have an image id
            if (image_id != -1) {
                last_histograms = histograms.clone();
                last_image_id = image_id;
            }
        } else { // last_image_id == image_id
            // Use cache
            histograms = last_histograms.clone();
        }
    }

    float SelectiveSearchSegmentationStrategyColorImpl::get(int r1, int r2) {

        float r = 0;
        float *h1 = histograms.ptr<float>(r1);
        float *h2 = histograms.ptr<float>(r2);

        for (int i = 0; i < histogram_size; i++) {
            r += cv::min(h1[i], h2[i]);
        }

        return r;
    }

    void SelectiveSearchSegmentationStrategyColorImpl::merge(int r1, int r2) {
        int size_r1 = sizes.at<int>(r1);
        int size_r2 = sizes.at<int>(r2);

        float *h1 = histograms.ptr<float>(r1);
        float *h2 = histograms.ptr<float>(r2);

        for (int i = 0; i < histogram_size; i++) {
            h1[i] = (h1[i] * size_r1 + h2[i] * size_r2) / (size_r1 + size_r2);
            h2[i] = h1[i];
        }
    }
}