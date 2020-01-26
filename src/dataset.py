# -*- coding: utf-8 -*-

"""
@author: zj
@file:   dataset.py
@time:   2020-01-25
"""

import os
import numpy as np
import cv2
from torch.utils.data import Dataset


class SelectiveSearchDataset(Dataset):

    def __init__(self, root_dir, train=True, transform=None):
        self.root_dir = root_dir
        self.train = train
        self.transform = transform

        if self.train:
            self.positive_samples, self.positive_targets, self.negative_samples, self.negative_targets = self._load_data(
                root_dir)

    def __getitem__(self, idx):
        positive_length = self.positive_samples.shape[0]

        if idx < positive_length:
            sample, label = self.positive_samples[idx], 1
        else:
            sample, label = self.negative_samples[idx - positive_length], 0

        if self.transform:
            sample = self.transform(sample)

        return sample, label

    def __len__(self):
        return len(self.positive_samples) + len(self.negative_samples)

    def _load_data(self, root_dir, voc_2007_dir='./data/VOCdevkit/VOC2007/JPEGImages'):
        positive_samples = list()
        positive_targets = list()
        negative_samples = list()
        negative_targets = list()

        # 样本所属类别
        cate_list = os.listdir(root_dir)

        for cate in cate_list:
            cate_dir = os.path.join(root_dir, cate)
            positive_dir = os.path.join(cate_dir, 'positive')
            negative_dir = os.path.join(cate_dir, 'negative')

            # 每个类别中的正负样本目录下的csv文件名相同
            csv_list = os.listdir(positive_dir)
            for csv_name in csv_list:
                # 保存边界框所在的图像名
                origin_img_path = os.path.join(voc_2007_dir, csv_name.split('.')[0] + '.jpg')
                origin_img = cv2.imread(origin_img_path, cv2.IMREAD_COLOR)

                positive_csv_path = os.path.join(positive_dir, csv_name)
                negative_csv_path = os.path.join(negative_dir, csv_name)

                positive_data_array = np.loadtxt(positive_csv_path, delimiter=' ').astype(np.int)
                negative_data_array = np.loadtxt(negative_csv_path, delimiter=' ').astype(np.int)

                res_samples, res_targets = self._parse_data(positive_data_array, origin_img, cate, origin_img_path,
                                                            is_positive=True)
                positive_samples.extend(res_samples)
                positive_targets.extend(res_targets)

                res_samples, res_targets = self._parse_data(negative_data_array, origin_img, cate, origin_img_path,
                                                            is_positive=False)
                negative_samples.extend(res_samples)
                negative_targets.extend(res_targets)

        return np.array(positive_samples), np.array(positive_targets), \
               np.array(negative_samples), np.array(negative_targets)

    def _parse_data(self, data_array, origin_img, cate, origin_img_path, is_positive=True):
        samples = list()
        targets = list()

        # 防止出现小于0的坐标
        data_array[data_array < 0] = 0
        if len(data_array.shape) == 1:
            xmin, ymin, xmax, ymax = data_array
            img = origin_img[ymin:ymax, xmin:xmax]
            samples.append(origin_img[ymin:ymax, xmin:xmax])
            targets.append({'cate': cate, 'bndbox': [xmin, ymin, xmax, ymax], 'positive': is_positive,
                            'img_path': origin_img_path})
        else:
            for xmin, ymin, xmax, ymax in data_array:
                img = origin_img[ymin:ymax, xmin:xmax]
                samples.append(origin_img[ymin:ymax, xmin:xmax])
                targets.append({'cate': cate, 'bndbox': [xmin, ymin, xmax, ymax], 'positive': is_positive,
                                'img_path': origin_img_path})
        return samples, targets
