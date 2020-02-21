//
// Created by zj on 2020/2/14.
//

#include "../include/selectivesearch/selective_search_segmentation_strategy_color_impl.h"

namespace segmentation {

    /*
     * 计算每个区域的颜色直方图
     * 1. 分离各通道
     * 2. 计算每个区域的颜色直方图，大小为25 bin
     * 3. 连接各通道的颜色直方图，并进行标准化（除以所有bin的和）
     */
    void SelectiveSearchSegmentationStrategyColorImpl::setImage(cv::InputArray img_, cv::InputArray regions_, cv::InputArray sizes_,
                                                                int image_id) {

        cv::Mat img = img_.getMat();
        cv::Mat regions = regions_.getMat();
        sizes = sizes_.getMat();

        if (image_id == -1 || last_image_id != image_id) {

            // 分离颜色通道
            std::vector<cv::Mat> img_planes;
            split(img, img_planes);

            int histogram_bins_size = 25;

            float range[] = {0, 256};
            const float *histogram_ranges = {range};

            double min, max;
            minMaxLoc(regions, &min, &max);
            int nb_segs = (int) max + 1;

            histogram_size = histogram_bins_size * img.channels();
            // 保存每个区域的颜色直方图
            histograms = cv::Mat_<float>(nb_segs, histogram_size);

            for (int r = 0; r < nb_segs; r++) {

                // Generate mask
                // 设置掩码，计算每个区域的直方图
                cv::Mat mask = regions == r;

                // Compute histogram for each channels
                float tt = 0;

                cv::Mat tmp_hists = cv::Mat(histogram_size, 1, CV_32F);
                float *tmp_histogram = tmp_hists.ptr<float>(0);
                int h_pos = 0;
                cv::Mat tmp_hist;

                // 逐通道计算每个区域的直方图并保存（25*通道数）
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
                // 直方图标准化
                float *histogram = histograms.ptr<float>(r);

                for (int h_pos2 = 0; h_pos2 < histogram_size; h_pos2++) {
                    histogram[h_pos2] = tmp_histogram[h_pos2] / tt;
                }
            }

            // Save cache if we have an image id
            // 如果使用的是同一个颜色空间，同一个图分割对象实现的初始分割集，则利用缓存进行处理
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

        // 比较两个区域直方图每一维的大小，进行累加
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