//
// Created by zj on 2020/2/15.
//

#include "../include/selectivesearch/region.h"

namespace segmentation {
    std::ostream &operator<<(std::ostream &os, const Region &r) {
        os << "Region[WID" << r.id << ", L" << r.level << ", merged to " << r.merged_to << ", R:" << r.rank << ", "
           << r.bounding_box << "]";
        return os;
    }
}