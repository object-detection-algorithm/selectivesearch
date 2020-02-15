//
// Created by zj on 2020/2/15.
//

#include "../include/selectivesearch/neighbor.h"

namespace segmentation {
    std::ostream &operator<<(std::ostream &os, const Neighbour &n) {
        os << "Neighbour[" << n.from << "->" << n.to << "," << n.similarity << "]";
        return os;
    }
}
