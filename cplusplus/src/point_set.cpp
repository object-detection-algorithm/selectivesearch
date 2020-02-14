//
// Created by zj on 2020/2/12.
//

#include "../include/graphsegment/point_set.h"
#include <map>

namespace segmentation {

    PointSet::PointSet(int nb_elements_) {
        nb_elements = nb_elements_;

        mapping = new PointSetElement[nb_elements];

        for (int i = 0; i < nb_elements; i++) {
            mapping[i] = PointSetElement(i);
        }
    }

    PointSet::~PointSet() {
        delete[] mapping;
    }

    int PointSet::getBasePoint(int p) {

        int base_p = p;

        while (base_p != mapping[base_p].p) {
            base_p = mapping[base_p].p;
        }

        // Save mapping for faster acces later
        mapping[p].p = base_p;

        return base_p;
    }

    void PointSet::joinPoints(int p_a, int p_b) {

        // Always target smaller set, to avoid redirection in getBasePoint
        if (mapping[p_a].size < mapping[p_b].size)
            std::swap(p_a, p_b);

        mapping[p_b].p = p_a;
        mapping[p_a].size += mapping[p_b].size;

        nb_elements--;
    }

}