//
// Created by zj on 2020/2/15.
//

#include "../include/selectivesearch/selective_search_segmentation_strategy_texture_impl.h"

namespace segmentation {
    void SelectiveSearchSegmentationStrategyTextureImpl::setImage(
            cv::InputArray img_, cv::InputArray regions_, cv::InputArray sizes_, int image_id) {

        cv::Mat img = img_.getMat();
        cv::Mat regions = regions_.getMat();
        sizes = sizes_.getMat();

        if (image_id == -1 || last_image_id != image_id) {

            std::vector<cv::Mat> img_planes;
            split(img, img_planes);

            int histogram_bins_size = 10;

            float range[] = {0.0, 256.0};

            double min, max;
            minMaxLoc(regions, &min, &max);
            int nb_segs = (int) max + 1;

            histogram_size = histogram_bins_size * img.channels() * 8;

            histograms = cv::Mat_<float>(nb_segs, histogram_size);

            // Compute, for each channels, the 8 gaussians
            std::vector<cv::Mat> img_gaussians;

            for (int p = 0; p < img.channels(); p++) {

                cv::Mat tmp_gradiant;
                cv::Mat tmp_gradiant_pos, tmp_gradiant_neg;
                cv::Mat img_plane_rotated;
                cv::Mat tmp_rot;

                // X, no rot
                Scharr(img_planes[p], tmp_gradiant, CV_32F, 1, 0);
                threshold(tmp_gradiant, tmp_gradiant_pos, 0, 0, cv::THRESH_TOZERO);
                threshold(tmp_gradiant, tmp_gradiant_neg, 0, 0, cv::THRESH_TOZERO_INV);

                img_gaussians.push_back(tmp_gradiant_pos.clone());
                img_gaussians.push_back(tmp_gradiant_neg.clone());

                // Y, no rot
                Scharr(img_planes[p], tmp_gradiant, CV_32F, 0, 1);
                threshold(tmp_gradiant, tmp_gradiant_pos, 0, 0, cv::THRESH_TOZERO);
                threshold(tmp_gradiant, tmp_gradiant_neg, 0, 0, cv::THRESH_TOZERO_INV);

                img_gaussians.push_back(tmp_gradiant_pos.clone());
                img_gaussians.push_back(tmp_gradiant_neg.clone());

                cv::Point2f center(img.cols / 2.0f, img.rows / 2.0f);
                cv::Mat rot = cv::getRotationMatrix2D(center, 45.0, 1.0);
                cv::Rect bbox = cv::RotatedRect(center, img.size(), 45.0).boundingRect();
                rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
                rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

                warpAffine(img_planes[p], img_plane_rotated, rot, bbox.size());

                // X, rot
                Scharr(img_plane_rotated, tmp_gradiant, CV_32F, 1, 0);

                center = cv::Point((int) (img_plane_rotated.cols / 2.0), (int) (img_plane_rotated.rows / 2.0));
                rot = cv::getRotationMatrix2D(center, -45.0, 1.0);
                // Using this bigger box avoids clipping the ends of narrow images
                cv::Rect bbox2 = cv::RotatedRect(center, img_plane_rotated.size(), -45.0).boundingRect();\
                        warpAffine(tmp_gradiant, tmp_rot, rot, bbox2.size());

                // for narrow images, bbox might be less tall or wide than img
                int start_x = std::max(0, (bbox.width - img.cols) / 2);
                int start_y = std::max(0, (bbox.height - img.rows) / 2);
                tmp_gradiant = tmp_rot(cv::Rect(start_x, start_y, img.cols, img.rows));

                threshold(tmp_gradiant, tmp_gradiant_pos, 0, 0, cv::THRESH_TOZERO);
                threshold(tmp_gradiant, tmp_gradiant_neg, 0, 0, cv::THRESH_TOZERO_INV);

                img_gaussians.push_back(tmp_gradiant_pos.clone());
                img_gaussians.push_back(tmp_gradiant_neg.clone());

                // Y, rot
                Scharr(img_plane_rotated, tmp_gradiant, CV_32F, 0, 1);

                center = cv::Point((int) (img_plane_rotated.cols / 2.0), (int) (img_plane_rotated.rows / 2.0));
                rot = cv::getRotationMatrix2D(center, -45.0, 1.0);
                bbox2 = cv::RotatedRect(center, img_plane_rotated.size(), -45.0).boundingRect();\
                        warpAffine(tmp_gradiant, tmp_rot, rot, bbox2.size());

                start_x = std::max(0, (bbox.width - img.cols) / 2);
                start_y = std::max(0, (bbox.height - img.rows) / 2);
                tmp_gradiant = tmp_rot(cv::Rect(start_x, start_y, img.cols, img.rows));

                threshold(tmp_gradiant, tmp_gradiant_pos, 0, 0, cv::THRESH_TOZERO);
                threshold(tmp_gradiant, tmp_gradiant_neg, 0, 0, cv::THRESH_TOZERO_INV);

                img_gaussians.push_back(tmp_gradiant_pos.clone());
                img_gaussians.push_back(tmp_gradiant_neg.clone());

            }

            // Normalisze gaussiaans in 0-255 range (for faster computation of histograms)
            for (int i = 0; i < img.channels() * 8; i++) {

                double hmin, hmax;
                minMaxLoc(img_gaussians[i], &hmin, &hmax);

                cv::Mat tmp;
                img_gaussians[i].convertTo(tmp, CV_8U, (range[1] - 1) / (hmax - hmin),
                                           -(range[1] - 1) * hmin / (hmax - hmin));
                img_gaussians[i] = tmp;

            }

            // We compute histograms manualy, directly addings bins based on the region instead of computing multiple histograms
            // This speedup significantly computations

            std::vector<int> totals;
            totals.resize(nb_segs);

            // Bins for histograms
            cv::Mat_<int> tmp_histograms = cv::Mat_<int>::zeros(nb_segs, histogram_size);

            int *regions_data = (int *) regions.data;

            for (unsigned int x = 0; x < regions.total(); x++) {
                int region = regions_data[x];

                int *histogram = tmp_histograms.ptr<int>(region);

                for (int p = 0; p < img.channels(); p++) {
                    for (unsigned int i = 0; i < 8; i++) {

                        int val = (int) ((unsigned char *) img_gaussians[p * 8 + i].data)[x];

                        int bin = (int) ((float) val / (range[1] / histogram_bins_size));

                        histogram[(p * 8 + i) * histogram_bins_size + bin]++;
                        totals[region]++;
                    }
                }
            }

            // Normalisation per segments
            for (int r = 0; r < nb_segs; r++) {

                float *histogram = histograms.ptr<float>(r);
                int *tmp_histogram = tmp_histograms.ptr<int>(r);

                for (int h_pos2 = 0; h_pos2 < histogram_size; h_pos2++) {
                    histogram[h_pos2] = (float) tmp_histogram[h_pos2] / (float) totals[r];
                }
            }

            if (image_id != -1) { // Save cache if it's apply
                last_histograms = histograms.clone();
                last_image_id = image_id;
            }
        } else { // image_id == last_image_id
            histograms = last_histograms.clone(); // Use cache
        }
    }

    float SelectiveSearchSegmentationStrategyTextureImpl::get(int r1, int r2) {

        float r = 0;
        float *h1 = histograms.ptr<float>(r1);
        float *h2 = histograms.ptr<float>(r2);

        for (int i = 0; i < histogram_size; i++) {
            r += cv::min(h1[i], h2[i]);
        }

        return r;
    }

    void SelectiveSearchSegmentationStrategyTextureImpl::merge(int r1, int r2) {
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