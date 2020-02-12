//
// Created by zj on 2020/2/12.
//

#ifndef CPLUPLUS_EDGE_H
#define CPLUPLUS_EDGE_H

namespace segmentation {

// Represent an edge between two pixels
    class Edge {
    public:
        int from;
        int to;
        float weight;

        bool operator<(const Edge &e) const {
            return weight < e.weight;
        }
    };

}

#endif //CPLUPLUS_EDGE_H
