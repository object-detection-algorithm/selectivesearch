//
// Created by zj on 2020/2/15.
//

#include "../include/selectivesearch/selectivesearchsegmentationimpl.h"
#include "../include/selectivesearch/selective_search_segmentation_strategy_color.h"
#include "../include/selectivesearch/selective_search_segmentation_strategy_fill.h"
#include "../include/selectivesearch/selective_search_segmentation_strategy_texture.h"
#include "../include/selectivesearch/selective_search_segmentation_strategy_size.h"
#include "../include/selectivesearch/selective_search_segmentation_strategy_multiple.h"
#include "../include/selectivesearch/rect_comparator.h"
#include "../include/selectivesearch/neighbor.h"

namespace segmentation {
    void SelectiveSearchSegmentationImpl::setBaseImage(cv::InputArray img) {
        base_image = img.getMat();
    }

    void SelectiveSearchSegmentationImpl::addImage(cv::InputArray img) {
        images.push_back(img.getMat());
    }

    void SelectiveSearchSegmentationImpl::clearImages() {
        images.clear();
    }

    void SelectiveSearchSegmentationImpl::addGraphSegmentation(std::shared_ptr<GraphSegmentation> g) {
        segmentations.push_back(g);
    }

    void SelectiveSearchSegmentationImpl::clearGraphSegmentations() {
        segmentations.clear();
    }

    void SelectiveSearchSegmentationImpl::addStrategy(std::shared_ptr<SelectiveSearchSegmentationStrategy> s) {
        strategies.push_back(s);
    }

    void SelectiveSearchSegmentationImpl::clearStrategies() {
        strategies.clear();
    }

    void SelectiveSearchSegmentationImpl::switchToSingleStrategy(int k, float sigma) {
        clearImages();
        clearGraphSegmentations();
        clearStrategies();

        cv::Mat hsv;
        cvtColor(base_image, hsv, cv::COLOR_BGR2HSV);
        addImage(hsv);

        std::shared_ptr<GraphSegmentation> gs = createGraphSegmentation();
        gs->setK((float) k);
        gs->setSigma(sigma);
        addGraphSegmentation(gs);

        std::shared_ptr<SelectiveSearchSegmentationStrategyColor> color = createSelectiveSearchSegmentationStrategyColor();
        std::shared_ptr<SelectiveSearchSegmentationStrategyFill> fill = createSelectiveSearchSegmentationStrategyFill();
        std::shared_ptr<SelectiveSearchSegmentationStrategyTexture> texture = createSelectiveSearchSegmentationStrategyTexture();
        std::shared_ptr<SelectiveSearchSegmentationStrategySize> size = createSelectiveSearchSegmentationStrategySize();

        std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple> m = createSelectiveSearchSegmentationStrategyMultiple(
                color, fill, texture, size);

        addStrategy(m);

    }

    void SelectiveSearchSegmentationImpl::switchToSelectiveSearchFast(int base_k, int inc_k, float sigma) {
        clearImages();
        clearGraphSegmentations();
        clearStrategies();

        cv::Mat hsv;
        cvtColor(base_image, hsv, cv::COLOR_BGR2HSV);
        addImage(hsv);
        cv::Mat lab;
        cvtColor(base_image, lab, cv::COLOR_BGR2Lab);
        addImage(lab);

        for (int k = base_k; k <= base_k + inc_k * 2; k += inc_k) {
            std::shared_ptr<GraphSegmentation> gs = createGraphSegmentation();
            gs->setK((float) k);
            gs->setSigma(sigma);
            addGraphSegmentation(gs);
        }

        std::shared_ptr<SelectiveSearchSegmentationStrategyColor> color = createSelectiveSearchSegmentationStrategyColor();
        std::shared_ptr<SelectiveSearchSegmentationStrategyFill> fill = createSelectiveSearchSegmentationStrategyFill();
        std::shared_ptr<SelectiveSearchSegmentationStrategyTexture> texture = createSelectiveSearchSegmentationStrategyTexture();
        std::shared_ptr<SelectiveSearchSegmentationStrategySize> size = createSelectiveSearchSegmentationStrategySize();

        std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple> m = createSelectiveSearchSegmentationStrategyMultiple(
                color, fill, texture, size);

        addStrategy(m);

        std::shared_ptr<SelectiveSearchSegmentationStrategyFill> fill2 = createSelectiveSearchSegmentationStrategyFill();
        std::shared_ptr<SelectiveSearchSegmentationStrategyTexture> texture2 = createSelectiveSearchSegmentationStrategyTexture();
        std::shared_ptr<SelectiveSearchSegmentationStrategySize> size2 = createSelectiveSearchSegmentationStrategySize();

        std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple> m2 = createSelectiveSearchSegmentationStrategyMultiple(
                fill2, texture2, size2);

        addStrategy(m2);

    }

    void SelectiveSearchSegmentationImpl::switchToSelectiveSearchQuality(int base_k, int inc_k, float sigma) {
        clearImages();
        clearGraphSegmentations();
        clearStrategies();


        cv::Mat hsv;
        cvtColor(base_image, hsv, cv::COLOR_BGR2HSV);
        addImage(hsv);
        cv::Mat lab;
        cvtColor(base_image, lab, cv::COLOR_BGR2Lab);
        addImage(lab);

        cv::Mat I;
        cvtColor(base_image, I, cv::COLOR_BGR2GRAY);
        addImage(I);

        cv::Mat channel[3];
        split(hsv, channel);
        addImage(channel[0]);

        split(base_image, channel);
        std::vector<cv::Mat> channel2 = {channel[2], channel[1], I};

        cv::Mat rgI;
        merge(channel2, rgI);
        addImage(rgI);

        for (int k = base_k; k <= base_k + inc_k * 4; k += inc_k) {
            std::shared_ptr<GraphSegmentation> gs = createGraphSegmentation();
            gs->setK((float) k);
            gs->setSigma(sigma);
            addGraphSegmentation(gs);
        }

        std::shared_ptr<SelectiveSearchSegmentationStrategyColor> color = createSelectiveSearchSegmentationStrategyColor();
        std::shared_ptr<SelectiveSearchSegmentationStrategyFill> fill = createSelectiveSearchSegmentationStrategyFill();
        std::shared_ptr<SelectiveSearchSegmentationStrategyTexture> texture = createSelectiveSearchSegmentationStrategyTexture();
        std::shared_ptr<SelectiveSearchSegmentationStrategySize> size = createSelectiveSearchSegmentationStrategySize();

        std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple> m = createSelectiveSearchSegmentationStrategyMultiple(
                color, fill, texture, size);

        addStrategy(m);

        std::shared_ptr<SelectiveSearchSegmentationStrategyFill> fill2 = createSelectiveSearchSegmentationStrategyFill();
        std::shared_ptr<SelectiveSearchSegmentationStrategyTexture> texture2 = createSelectiveSearchSegmentationStrategyTexture();
        std::shared_ptr<SelectiveSearchSegmentationStrategySize> size2 = createSelectiveSearchSegmentationStrategySize();

        std::shared_ptr<SelectiveSearchSegmentationStrategyMultiple> m2 = createSelectiveSearchSegmentationStrategyMultiple(
                fill2, texture2, size2);

        addStrategy(m2);

        std::shared_ptr<SelectiveSearchSegmentationStrategyFill> fill3 = createSelectiveSearchSegmentationStrategyFill();
        addStrategy(fill3);

        std::shared_ptr<SelectiveSearchSegmentationStrategySize> size3 = createSelectiveSearchSegmentationStrategySize();
        addStrategy(size3);
    }

    void SelectiveSearchSegmentationImpl::process(std::vector<cv::Rect> &rects) {

        std::vector<Region> all_regions;

        int image_id = 0;

        for (std::vector<cv::Mat>::iterator image = images.begin(); image != images.end(); ++image) {
            for (std::vector<std::shared_ptr<GraphSegmentation> >::iterator gs = segmentations.begin();
                 gs != segmentations.end(); ++gs) {

                cv::Mat img_regions;
                cv::Mat_<char> is_neighbour;
                cv::Mat_<int> sizes;

                // Compute initial segmentation
                (*gs)->processImage(*image, img_regions);

                // Get number of regions
                double min, max;
                minMaxLoc(img_regions, &min, &max);
                int nb_segs = (int) max + 1;

                // Compute bouding rects and neighbours
                std::vector<cv::Rect> bounding_rects;
                bounding_rects.resize(nb_segs);

                std::vector<std::vector<cv::Point> > points;

                points.resize(nb_segs);

                is_neighbour = cv::Mat::zeros(nb_segs, nb_segs, CV_8UC1);
                sizes = cv::Mat::zeros(nb_segs, 1, CV_32SC1);

                const int *previous_p = NULL;

                // 遍历图分割结果，设置相邻区域
                for (int i = 0; i < (int) img_regions.rows; i++) {
                    const int *p = img_regions.ptr<int>(i);

                    for (int j = 0; j < (int) img_regions.cols; j++) {

                        // 保存每个区域的点集
                        points[p[j]].push_back(cv::Point(j, i));
                        // 保存分割结果中每个区域的大小
                        sizes.at<int>(p[j], 0) = sizes.at<int>(p[j], 0) + 1;

                        // 设置相邻区域
                        if (i > 0 && j > 0) {

                            // 当前点所属区域与左边点所属区域
                            is_neighbour.at<char>(p[j], p[j - 1]) = 1;
                            // 当前点所属区域与上面点所属区域
                            is_neighbour.at<char>(p[j], previous_p[j]) = 1;
                            // 当前点所属区域与左上角点所属区域
                            is_neighbour.at<char>(p[j], previous_p[j - 1]) = 1;

                            is_neighbour.at<char>(p[j - 1], p[j]) = 1;
                            is_neighbour.at<char>(previous_p[j], p[j]) = 1;
                            is_neighbour.at<char>(previous_p[j - 1], p[j]) = 1;
                        }
                    }
                    previous_p = p;
                }

                // 计算每个区域边框所属位置和大小
                for (int seg = 0; seg < nb_segs; seg++) {
                    bounding_rects[seg] = cv::boundingRect(points[seg]);
                }

                // 调用不同的相似性度量
                for (std::vector<std::shared_ptr<SelectiveSearchSegmentationStrategy> >::iterator strategy = strategies.begin();
                     strategy != strategies.end(); ++strategy) {
                    std::vector<Region> regions;
                    // 通过分层分组算法计算所有候选区域
                    hierarchicalGrouping(*image, *strategy, img_regions, is_neighbour, sizes, nb_segs, bounding_rects,
                                         regions, image_id);

                    for (std::vector<Region>::iterator region = regions.begin(); region != regions.end(); ++region) {
                        all_regions.push_back(*region);
                    }
                }

                image_id++;
            }
        }

        // 按秩排序
        std::sort(all_regions.begin(), all_regions.end());

        std::map<cv::Rect, char, rectComparator> processed_rect;

        rects.clear();

        // Remove duplicate in rect list
        for (std::vector<Region>::iterator region = all_regions.begin(); region != all_regions.end(); ++region) {
            if (processed_rect.find((*region).bounding_box) == processed_rect.end()) {
                processed_rect[(*region).bounding_box] = true;
                rects.push_back((*region).bounding_box);
            }
        }

    }

    /**
     * 计算相邻区域之间的相似度，然后找出相似度最高的相邻区域对进行合并，迭代上述过程直到分割集合并成整个图像
     * @param img：不同颜色空间下的原始图像
     * @param s：相似度计算
     * @param img_regions：图分割算法得到的初始分割集
     * @param is_neighbour：邻接矩阵
     * @param sizes_：各个区域大小
     * @param nb_segs：初始分割集区域个数
     * @param bounding_rects：各个区域所属边框的坐标和大小
     * @param regions：保存这次分层分组算法计算得到的候选区域
     * @param image_id：是否位于同一颜色空间以及同一图分割对象
     */
    void SelectiveSearchSegmentationImpl::hierarchicalGrouping(const cv::Mat &img,
                                                               std::shared_ptr<SelectiveSearchSegmentationStrategy> &s,
                                                               const cv::Mat &img_regions,
                                                               const cv::Mat_<char> &is_neighbour,
                                                               const cv::Mat_<int> &sizes_, int &nb_segs,
                                                               const std::vector<cv::Rect> &bounding_rects,
                                                               std::vector<Region> &regions, int image_id) {

        cv::Mat sizes = sizes_.clone();

        std::vector<Neighbour> similarities;
        regions.clear();

        /////////////////////////////////////////

        s->setImage(img, img_regions, sizes, image_id);

        // Compute initial similarities
        for (int i = 0; i < nb_segs; i++) {
            Region r;
            // 所属区域
            r.id = i;
            // 初始区域等级为１，表示经过一次分割，在后续的合并操作中，每次合并后新区域的level+1
            r.level = 1;
            r.merged_to = -1;
            // 区域所属边框大小
            r.bounding_box = bounding_rects[i];
            // 加入初始分割区域
            regions.push_back(r);
            // 根据邻接矩阵计算当前区域与相邻区域的相似度
            for (int j = i + 1; j < nb_segs; j++) {
                if (is_neighbour.at<char>(i, j)) {
                    Neighbour n;
                    n.from = i;
                    n.to = j;
                    n.similarity = s->get(i, j);

                    similarities.push_back(n);
                }
            }
        }

        while (similarities.size() > 0) {
            // 根据相似度大小排序
            std::sort(similarities.begin(), similarities.end());

            // for(std::vector<Neighbour>::iterator similarity = similarities.begin(); similarity != similarities.end(); ++similarity) {
            //     std::cout << *similarity << std::endl;
            // }
            // 获取最高相似度的相邻区域对
            Neighbour p = similarities.back();
            similarities.pop_back();

            Region region_from = regions[p.from];
            Region region_to = regions[p.to];
            // 合并相邻区域，
            Region new_r;
            new_r.id = std::min(region_from.id, region_to.id); // Should be the smalest, working ID
            new_r.level = std::max(region_from.level, region_to.level) + 1;
            new_r.merged_to = -1;
            // 计算合并区域所属边框
            new_r.bounding_box = region_from.bounding_box | region_to.bounding_box;

            regions.push_back(new_r);

            regions[p.from].merged_to = (int) regions.size() - 1;
            regions[p.to].merged_to = (int) regions.size() - 1;

            // Merge
            s->merge(region_from.id, region_to.id);

            // Update size
            sizes.at<int>(region_from.id, 0) += sizes.at<int>(region_to.id, 0);
            sizes.at<int>(region_to.id, 0) = sizes.at<int>(region_from.id, 0);

            std::vector<int> local_neighbours;
            // 重新计算合并区域的相似度
            for (std::vector<Neighbour>::iterator similarity = similarities.begin();
                 similarity != similarities.end();) {
                // 如果当前相似区域对包含了之前合并的区域，则重新计算
                if ((*similarity).from == p.from || (*similarity).to == p.from || (*similarity).from == p.to ||
                    (*similarity).to == p.to) {
                    int from = 0;

                    if ((*similarity).from == p.from || (*similarity).from == p.to) {
                        from = (*similarity).to;
                    } else {
                        from = (*similarity).from;
                    }

                    bool already_neighboor = false;

                    for (std::vector<int>::iterator local_neighbour = local_neighbours.begin();
                         local_neighbour != local_neighbours.end(); local_neighbour++) {
                        if (*local_neighbour == from) {
                            already_neighboor = true;
                        }
                    }

                    if (!already_neighboor) {
                        local_neighbours.push_back(from);
                    }
                    // 擦除原先的相似对
                    similarity = similarities.erase(similarity);
                } else {
                    similarity++;
                }
            }

            for (std::vector<int>::iterator local_neighbour = local_neighbours.begin();
                 local_neighbour != local_neighbours.end(); local_neighbour++) {

                Neighbour n;
                n.from = (int) regions.size() - 1;
                n.to = *local_neighbour;
                n.similarity = s->get(regions[n.from].id, regions[n.to].id);

                similarities.push_back(n);
            }
        }

        // Compute regions' rank
        // 调用随机数计算区域排名
        for (std::vector<Region>::iterator region = regions.begin(); region != regions.end(); ++region) {
            // 属性level表明了区域合并次数
            // Note: this is inverted from the paper, but we keep the lover region first so it's works
            (*region).rank = ((double) rand() / (RAND_MAX)) * ((*region).level);
        }

    }

}