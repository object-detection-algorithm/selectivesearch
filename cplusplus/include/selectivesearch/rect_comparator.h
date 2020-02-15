//
// Created by zj on 2020/2/15.
//

#ifndef CPLUPLUS_RECT_COMPARATOR_H
#define CPLUPLUS_RECT_COMPARATOR_H

#include <opencv2/opencv.hpp>

namespace segmentation {
    // Comparator to sort cv::rect (used for a std::map).
    struct rectComparator {
        bool operator()(const cv::Rect_<int> &a, const cv::Rect_<int> &b) const {
            if (a.x < b.x) {
                return true;
            }
            if (a.x > b.x) {
                return false;
            }
            if (a.y < b.y) {
                return true;
            }
            if (a.y > b.y) {
                return false;
            }
            if (a.width < b.width) {
                return true;
            }
            if (a.width > b.width) {
                return false;
            }
            if (a.height < b.height) {
                return true;
            }
            if (a.height > b.height) {
                return false;
            }
            return false;
        }
    };

}

#endif //CPLUPLUS_RECT_COMPARATOR_H
