//
// Created by zj on 2020/2/12.
//

#ifndef CPLUPLUS_POINT_SET_H
#define CPLUPLUS_POINT_SET_H

#include "point_set_element.h"

namespace segmentation {

    // An object to manage set of points, who can be fusionned
    class PointSet {
    public:
        PointSet(int nb_elements_);

        ~PointSet();

        int nb_elements;

        // Return the main point of the point's set
        int getBasePoint(int p);

        // Join two sets of points, based on their main point
        void joinPoints(int p_a, int p_b);

        // Return the set size of a set (based on the main point)
        int size(unsigned int p) { return mapping[p].size; }

    private:
        PointSetElement *mapping;

    };
}

#endif //CPLUPLUS_POINT_SET_H
