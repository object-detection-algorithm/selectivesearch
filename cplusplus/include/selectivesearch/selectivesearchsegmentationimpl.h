//
// Created by zj on 2020/2/15.
//

#ifndef CPLUPLUS_SELECTIVESEARCHSEGMENTATIONIMPL_H
#define CPLUPLUS_SELECTIVESEARCHSEGMENTATIONIMPL_H

#include "selectivesearchsegmentation.h"
#include "region.h"

namespace segmentation {
    class SelectiveSearchSegmentationImpl final : public SelectiveSearchSegmentation {
    public:
        SelectiveSearchSegmentationImpl() {
            name_ = "SelectiveSearchSegmentation";
        }

//        ~SelectiveSearchSegmentationImpl() override {
//        };

//        virtual void write(FileStorage& fs) const override {
//            fs << "name" << name_;
//        }
//
//        virtual void read(const FileNode& fn) override {
//            CV_Assert( (String)fn["name"] == name_);
//        }

        virtual void setBaseImage(cv::InputArray img) override;

        virtual void switchToSingleStrategy(int k = 200, float sigma = 0.8) override;

        virtual void switchToSelectiveSearchFast(int base_k = 150, int inc_k = 150, float sigma = 0.8) override;

        virtual void switchToSelectiveSearchQuality(int base_k = 150, int inc_k = 150, float sigma = 0.8) override;

        virtual void addImage(cv::InputArray img) override;

        virtual void clearImages() override;

        virtual void addGraphSegmentation(std::shared_ptr<GraphSegmentation> g) override;

        virtual void clearGraphSegmentations() override;

        virtual void addStrategy(std::shared_ptr<SelectiveSearchSegmentationStrategy> s) override;

        virtual void clearStrategies() override;

        virtual void process(std::vector<cv::Rect> &rects) override;


    private:
        std::string name_;

        cv::Mat base_image;
        std::vector<cv::Mat> images;
        std::vector<std::shared_ptr<GraphSegmentation> > segmentations;
        std::vector<std::shared_ptr<SelectiveSearchSegmentationStrategy> > strategies;

        void hierarchicalGrouping(
                const cv::Mat &img, std::shared_ptr<SelectiveSearchSegmentationStrategy> &s, const cv::Mat &img_regions,
                const cv::Mat_<char> &is_neighbour, const cv::Mat_<int> &sizes, int &nb_segs,
                const std::vector<cv::Rect> &bounding_rects, std::vector<Region> &regions, int region_id);
    };
}

#endif //CPLUPLUS_SELECTIVESEARCHSEGMENTATIONIMPL_H
