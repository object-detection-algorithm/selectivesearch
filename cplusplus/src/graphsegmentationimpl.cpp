//
// Created by zj on 2020/2/12.
//


#include "../include/graphsegmentationimpl.h"
#include <iostream>


namespace segmentation {

    void GraphSegmentationImpl::filter(const Mat &img, Mat &img_filtered) {

        Mat img_converted;

        // Switch to float
        img.convertTo(img_converted, CV_32F);

        // Apply gaussian filter
        GaussianBlur(img_converted, img_filtered, Size(0, 0), sigma, sigma);
    }

    void GraphSegmentationImpl::buildGraph(Edge **edges, int &nb_edges, const Mat &img_filtered) {

        *edges = new Edge[img_filtered.rows * img_filtered.cols * 4];

        nb_edges = 0;

        int nb_channels = img_filtered.channels();

        for (int i = 0; i < (int) img_filtered.rows; i++) {
            const float *p = img_filtered.ptr<float>(i);

            for (int j = 0; j < (int) img_filtered.cols; j++) {

                //Take the right, left, top and down pixel
                for (int delta = -1; delta <= 1; delta += 2) {
                    for (int delta_j = 0, delta_i = 1; delta_j <= 1; delta_j++ || delta_i--) {

                        int i2 = i + delta * delta_i;
                        int j2 = j + delta * delta_j;

                        if (i2 >= 0 && i2 < img_filtered.rows && j2 >= 0 && j2 < img_filtered.cols) {
                            const float *p2 = img_filtered.ptr<float>(i2);

                            float tmp_total = 0;

                            for (int channel = 0; channel < nb_channels; channel++) {
                                tmp_total += pow(p[j * nb_channels + channel] - p2[j2 * nb_channels + channel],
                                                 2);
                            }

                            float diff = 0;
                            diff = sqrt(tmp_total);

                            (*edges)[nb_edges].weight = diff;
                            (*edges)[nb_edges].from = i * img_filtered.cols + j;
                            (*edges)[nb_edges].to = i2 * img_filtered.cols + j2;

                            nb_edges++;
                        }
                    }
                }
            }
        }
    }

    void GraphSegmentationImpl::segmentGraph(Edge *edges, const int &nb_edges, const Mat &img_filtered,
                                             PointSet **es) {

        int total_points = (int) (img_filtered.rows * img_filtered.cols);

        // Sort edges
        std::sort(edges, edges + nb_edges);

        // Create a set with all point (by default mapped to themselves)
        *es = new PointSet(img_filtered.cols * img_filtered.rows);

        // Thresholds
        float *thresholds = new float[total_points];

        for (int i = 0; i < total_points; i++)
            thresholds[i] = k;

        for (int i = 0; i < nb_edges; i++) {

            int p_a = (*es)->getBasePoint(edges[i].from);
            int p_b = (*es)->getBasePoint(edges[i].to);

            if (p_a != p_b) {
                if (edges[i].weight <= thresholds[p_a] && edges[i].weight <= thresholds[p_b]) {
                    (*es)->joinPoints(p_a, p_b);
                    p_a = (*es)->getBasePoint(p_a);
                    thresholds[p_a] = edges[i].weight + k / (*es)->size(p_a);

                    edges[i].weight = 0;
                }
            }
        }

        delete[] thresholds;
    }

    void GraphSegmentationImpl::filterSmallAreas(Edge *edges, const int &nb_edges, PointSet *es) {

        for (int i = 0; i < nb_edges; i++) {

            if (edges[i].weight > 0) {

                int p_a = es->getBasePoint(edges[i].from);
                int p_b = es->getBasePoint(edges[i].to);

                if (p_a != p_b && (es->size(p_a) < min_size || es->size(p_b) < min_size)) {
                    es->joinPoints(p_a, p_b);

                }
            }
        }

    }

    void GraphSegmentationImpl::finalMapping(PointSet *es, Mat &output) {

        int maximum_size = (int) (output.rows * output.cols);

        int last_id = 0;
        int *mapped_id = new int[maximum_size];

        for (int i = 0; i < maximum_size; i++)
            mapped_id[i] = -1;

        int rows = output.rows;
        int cols = output.cols;

        if (output.isContinuous()) {
            cols *= rows;
            rows = 1;
        }

        for (int i = 0; i < rows; i++) {

            int *p = output.ptr<int>(i);

            for (int j = 0; j < cols; j++) {

                int point = es->getBasePoint(i * cols + j);

                if (mapped_id[point] == -1) {
                    mapped_id[point] = last_id;
                    last_id++;
                }

                p[j] = mapped_id[point];
            }
        }

        delete[] mapped_id;
    }

    void GraphSegmentationImpl::processImage(InputArray src, OutputArray dst) {

        Mat img = src.getMat();

        dst.create(img.rows, img.cols, CV_32SC1);
        Mat output = dst.getMat();
        output.setTo(0);

        // Filter graph
        Mat img_filtered;
        filter(img, img_filtered);

        // Build graph
        Edge *edges;
        int nb_edges;

        buildGraph(&edges, nb_edges, img_filtered);

        // Segment graph
        PointSet *es;

        segmentGraph(edges, nb_edges, img_filtered, &es);

        // Remove small areas
        filterSmallAreas(edges, nb_edges, es);

        // Map to final output
        finalMapping(es, output);

        delete[] edges;
        delete es;

    }
}