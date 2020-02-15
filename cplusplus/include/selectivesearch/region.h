//
// Created by zj on 2020/2/14.
//

#ifndef CPLUPLUS_REGION_H
#define CPLUPLUS_REGION_H

#include <iostream>
#include <opencv2/opencv.hpp>

namespace segmentation {

    class Region {
    public:
        int id;
        int level;
        int merged_to;
        double rank;
        cv::Rect bounding_box;

        Region() : id(0), level(0), merged_to(0), rank(0) {}

        friend std::ostream &operator<<(std::ostream &os, const Region &n);

        bool operator<(const Region &n) const {
            return rank < n.rank;
        }
    };
}

#endif //CPLUPLUS_REGION_H
