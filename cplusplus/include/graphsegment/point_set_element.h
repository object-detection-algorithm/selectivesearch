//
// Created by zj on 2020/2/12.
//

#ifndef CPLUPLUS_POINT_SET_ELEMENT_H
#define CPLUPLUS_POINT_SET_ELEMENT_H

namespace segmentation {

// A point in the sets of points
    class PointSetElement {
    public:
        int p{};
        int size{};

        PointSetElement() {}

        PointSetElement(int p_) {
            p = p_;
            size = 1;
        }
    };
}

#endif //CPLUPLUS_POINT_SET_ELEMENT_H
