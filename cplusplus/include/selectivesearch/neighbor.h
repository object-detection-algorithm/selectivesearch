//
// Created by zj on 2020/2/14.
//

#ifndef CPLUPLUS_NEIGHBOR_H
#define CPLUPLUS_NEIGHBOR_H

#include <iostream>

namespace segmentation {
    class Neighbour {
    public:
        int from;
        int to;
        float similarity;

        friend std::ostream &operator<<(std::ostream &os, const Neighbour &n);

        bool operator<(const Neighbour &n) const {
            return similarity < n.similarity;
        }
    };
}

#endif //CPLUPLUS_NEIGHBOR_H
