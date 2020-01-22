# -*- coding: utf-8 -*-

"""
@author: zj
@file:   create_initial_dataset.py
@time:   2020-01-21
"""

import xmltodict
import numpy as np
import cv2
import os
import shutil
from torchvision.datasets import VOCDetection
import copy


def compute_iou(bndboxes, truths):
    """
    计算候选边界框和图像上真值边界框的重叠度
    bndboxes和truths大小相同:[N, 4]，其中N表示边框数目，4表示保存的是[xmin, ymin, xmax, ymax]
    """
    xA = np.maximum(bndboxes[:, 0], truths[:, 0])
    yA = np.maximum(bndboxes[:, 1], truths[:, 1])
    xB = np.minimum(bndboxes[:, 2], truths[:, 2])
    yB = np.minimum(bndboxes[:, 3], truths[:, 3])
    # 计算交集面积
    intersection = np.maximum(0.0, xB - xA) * np.maximum(0.0, yB - yA)
    # 计算两个边界框面积
    boxAArea = (bndboxes[:, 2] - bndboxes[:, 0]) * (bndboxes[:, 3] - bndboxes[:, 1])
    boxBArea = (truths[:, 2] - truths[:, 0]) * (truths[:, 3] - truths[:, 1])

    iou = intersection / (boxAArea + boxBArea - intersection)
    return iou


def get_bndbox(target):
    """
    获取标注文件中的边界框（忽略difficult属性为1的边界框）
    """
    bndboxes = dict()
    objects = target['annotation']['object']
    if isinstance(objects, list):
        for obj in objects:
            if int(obj['difficult']) == 0:
                if bndboxes.get(obj['name']) is None:
                    bndboxes[obj['name']] = [
                        [int(obj['bndbox']['xmin']), int(obj['bndbox']['ymin']), int(obj['bndbox']['xmax']),
                         int(obj['bndbox']['ymax'])]]
                else:
                    bndboxes[obj['name']].append(
                        [int(obj['bndbox']['xmin']), int(obj['bndbox']['ymin']), int(obj['bndbox']['xmax']),
                         int(obj['bndbox']['ymax'])])
    elif isinstance(objects, dict):
        if int(objects['difficult']) == 0:
            bndboxes[objects['name']] = [
                [int(objects['bndbox']['xmin']), int(objects['bndbox']['ymin']), int(objects['bndbox']['xmax']),
                 int(objects['bndbox']['ymax'])]]
    return bndboxes


def init_root_dir(root_dir):
    res_dir = os.path.join(root_dir, 'VOC_SelectiveSearch')
    if os.path.exists(res_dir):
        shutil.rmtree(res_dir)
    os.mkdir(res_dir)

    return res_dir


def init_cate_dir(res_dir, cate):
    cate_dir = os.path.join(res_dir, cate)
    if not os.path.exists(cate_dir):
        os.mkdir(cate_dir)
    cate_positive_dir = os.path.join(cate_dir, 'positive')
    if not os.path.exists(cate_positive_dir):
        os.mkdir(cate_positive_dir)
    cate_negative_dir = os.path.join(cate_dir, 'negative')
    if not os.path.exists(cate_negative_dir):
        os.mkdir(cate_negative_dir)

    return cate_dir, cate_positive_dir, cate_negative_dir


def remove_overlapping_area(ious, rects):
    tmp_ious = copy.deepcopy(ious)
    tmp_rects = copy.deepcopy(rects)

    idx = 0
    length = len(tmp_ious)
    # 计算指定区域建议和iou比它小的区域建议之间的重叠率
    while idx < length:
        candidate_ious = compute_iou(tmp_rects[:(length - 1 - idx)], np.array([tmp_rects[length - 1 - idx]]))
        candidate_ious = np.hstack((candidate_ious, np.ones(idx + 1) * 0.5))
        tmp_ious[candidate_ious > 0.7] = 0

        tmp_rects = tmp_rects[tmp_ious > 0]
        tmp_ious = tmp_ious[tmp_ious > 0]

        idx += 1
        length = len(tmp_ious)
    return tmp_ious, tmp_rects


def draw_rect(img, positive_bndboxes, negative_bndboxes):
    tmp = copy.deepcopy(img)
    for xmin, ymin, xmax, ymax in negative_bndboxes:
        cv2.rectangle(tmp, (xmin, ymin), (xmax, ymax), (0, 0, 255), thickness=1)
    for xmin, ymin, xmax, ymax in positive_bndboxes:
        cv2.rectangle(tmp, (xmin, ymin), (xmax, ymax), (0, 255, 0), thickness=1)
    return tmp


if __name__ == '__main__':
    root_dir = './data'
    res_dir = init_root_dir(root_dir)

    dataset = VOCDetection(root_dir, year='2007', image_set='train')
    selectiveSearchSegmentation = cv2.ximgproc.segmentation.createSelectiveSearchSegmentation()

    for idx, item in enumerate(dataset, 0):
        data, target = item
        img = np.array(data)
        file_name = target['annotation']['filename'].split('.')[0]
        print(idx, target)

        # 提取正样本 - 标注边界框
        bndboxes = get_bndbox(target)
        # 提取负样本 - IoU在[0.2, 0.5]的候选区域
        selectiveSearchSegmentation.setBaseImage(img)
        selectiveSearchSegmentation.switchToSelectiveSearchQuality()
        rects = selectiveSearchSegmentation.process()
        # change [x,y,w,h] to [xmin,ymin,xmax,ymax]
        rects[:, 2] += rects[:, 0]
        rects[:, 3] += rects[:, 1]

        # 计算同一图像中不同类别的标注和区域建议之间的IoU
        for cate, bndbox_list in bndboxes.items():
            # tmp_img = draw_rect(img, bndbox_list, rects)
            # cv2.imshow("origin", img)
            # cv2.imshow('bndboxes', tmp_img)

            candidate_ious = list()
            candidate_rects = None
            for bndbox in bndbox_list:
                candidate_ious.extend(compute_iou(rects, np.array([bndbox])))
                if candidate_rects is None:
                    candidate_rects = rects
                else:
                    candidate_rects = np.vstack((candidate_rects, rects))

            candidate_ious = np.array(candidate_ious)
            candidate_ious[candidate_ious < 0.2] = 0
            candidate_ious[candidate_ious > 0.5] = 0
            # 计算符合iou在[0.2, 0.5]之间的区域建议边界框
            candidate_rects = candidate_rects[candidate_ious > 0]
            candidate_ious = candidate_ious[candidate_ious > 0]
            # 按iou大小排序
            ious_idxs = np.argsort(candidate_ious)
            candidate_ious = candidate_ious[ious_idxs]
            candidate_rects = candidate_rects[ious_idxs]

            # tmp_img = draw_rect(np.array(data), bndbox_list, candidate_rects)
            # cv2.imshow('20%~50%', tmp_img)

            # 按iou从大到小开始计算，去除重叠率超过70%的区域建议
            _, negative_rects = remove_overlapping_area(candidate_ious, candidate_rects)

            # tmp_img = draw_rect(np.array(data), bndbox_list, negative_rects)
            # cv2.imshow('70%', tmp_img)
            # cv2.waitKey(0)
            # exit(0)

            # 保存每张图片中每类目标的标注边界框和区域建议边界框
            cate_dir, cate_positive_dir, cate_negative_dir = init_cate_dir(res_dir, cate)
            bndbox_list = np.array(bndbox_list)
            print(cate, bndbox_list.shape, negative_rects.shape)

            positive_path = os.path.join(cate_positive_dir, file_name + ".csv")
            negative_path = os.path.join(cate_negative_dir, file_name + ".csv")
            np.savetxt(positive_path, bndbox_list, delimiter=" ")
            np.savetxt(negative_path, negative_rects, delimiter=' ')
