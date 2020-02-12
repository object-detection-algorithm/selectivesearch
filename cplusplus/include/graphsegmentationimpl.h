//
// Created by zj on 2020/2/12.
//

#ifndef CPLUPLUS_GRAPHSEGMENTATIONIMPL_H
#define CPLUPLUS_GRAPHSEGMENTATIONIMPL_H

#include "point_set.h"
#include "graphsegmentation_demo.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <memory>

using namespace cv;
using namespace std;

namespace segmentation {
    class GraphSegmentationImpl : public GraphSegmentation {
    public:
        GraphSegmentationImpl() {
            sigma = 0.5;
            k = 300;
            min_size = 100;
            name_ = "GraphSegmentation";
        }

        virtual void processImage(InputArray src, OutputArray dst) override;

        virtual void setSigma(double sigma_) override {
            if (sigma_ <= 0) { sigma_ = 0.001; }
            sigma = sigma_;
        }

        virtual double getSigma() override { return sigma; }

        virtual void setK(float k_) override { k = k_; }

        virtual float getK() override { return k; }

        virtual void setMinSize(int min_size_) override { min_size = min_size_; }

        virtual int getMinSize() override { return min_size; }

//        virtual void write(FileStorage &fs) const override {
//            fs << "name" << name_
//               << "sigma" << sigma
//               << "k" << k
//               << "min_size" << (int) min_size;
//        }
//
//        virtual void read(const FileNode &fn) override {
//            CV_Assert((String) fn["name"] == name_);
//
//            sigma = (double) fn["sigma"];
//            k = (float) fn["k"];
//            min_size = (int) (int) fn["min_size"];
//        }

    private:
        double sigma;
        float k;
        int min_size;
        string name_;

        // Pre-filter the image
        void filter(const Mat &img, Mat &img_filtered);

        // Build the graph between each pixels
        void buildGraph(Edge **edges, int &nb_edges, const Mat &img_filtered);

        // Segment the graph
        void segmentGraph(Edge *edges, const int &nb_edges, const Mat &img_filtered, PointSet **es);

        // Remove areas too small
        void filterSmallAreas(Edge *edges, const int &nb_edges, PointSet *es);

        // Map the segemented graph to a Mat with uniques, sequentials ids
        void finalMapping(PointSet *es, Mat &output);
    };
}

#endif //CPLUPLUS_GRAPHSEGMENTATIONIMPL_H
