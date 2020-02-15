//
// Created by zj on 2020/2/15.
//

#include "../include/selectivesearch/selectivesearchsegmentation.h"
#include "../include/selectivesearch/selectivesearchsegmentationimpl.h"

namespace segmentation {
    std::shared_ptr<SelectiveSearchSegmentation> createSelectiveSearchSegmentation() {
        std::shared_ptr<SelectiveSearchSegmentation> s = std::make_shared<SelectiveSearchSegmentationImpl>();
        return s;
    }

}