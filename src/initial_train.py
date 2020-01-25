# -*- coding: utf-8 -*-

"""
@author: zj
@file:   initial_train.py
@time:   2020-01-22
"""

import logging
import os
from typing import Optional, Union, overload

import cv2
import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torch.utils.data import Dataset
from torch.utils.data import DataLoader
import torchvision.transforms as transforms

logging.basicConfig(format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s', level=logging.DEBUG)

input_size = 32

voc_2007_dir = './data/VOCdevkit/VOC2007/JPEGImages'


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
        return len(self.positive_imageset) + len(self.negative_imageset)

    def _load_data(self, root_dir):
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

                positive_data_array = np.loadtxt(positive_csv_path, delimiter=' ', dtype=np.uint8)
                negative_data_array = np.loadtxt(negative_csv_path, delimiter=' ', dtype=np.uint8)

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

    def _parse_data(self, data_array, origin_img, cate, origin_img_path, is_positive=True, ):
        samples = list()
        targets = list()

        if len(data_array.shape) == 1:
            xmin, ymin, xmax, ymax = data_array
            samples.append(origin_img[ymin:ymax, xmin:xmax])
            targets.append({'cate': cate, 'bndbox': [xmin, ymin, xmax, ymax], 'positive': is_positive,
                            'img_path': origin_img_path})
        else:
            for xmin, ymin, xmax, ymax in data_array:
                samples.append(origin_img[ymin:ymax, xmin:xmax])
                targets.append({'cate': cate, 'bndbox': [xmin, ymin, xmax, ymax], 'positive': is_positive,
                                'img_path': origin_img_path})
        return samples, targets

    def _load_csv(self, root_dir):
        # 正样本边界框
        positive_array = list()
        # 负样本边界框
        negative_array = list()
        # 样本所属类别
        cate_list = os.listdir(root_dir)
        # 样本所属图像
        img_name_list = list()

        for cate in cate_list:
            cate_dir = os.path.join(root_dir, cate)
            positive_dir = os.path.join(cate_dir, 'positive')
            negative_dir = os.path.join(cate_dir, 'negative')

            # 每个类别中的正负样本目录下的csv文件名相同
            csv_list = os.listdir(positive_dir)
            for csv_name in csv_list:
                # 保存边界框所在的图像名
                img_name_list.append(csv_name.split('.')[0])

                positive_csv_path = os.path.join(positive_dir, csv_name)
                negative_csv_path = os.path.join(negative_dir, csv_name)

                positive_array.append(positive_csv_path)
                negative_array.append(negative_csv_path)

        return np.array(cate_list), np.array(img_name_list), np.array(positive_array), np.array(negative_array)


def load_data(root_dir):
    trainsform = transforms.Compose([
        transforms.ToPILImage(),
        transforms.Resize(input_size),
        transforms.ToTensor(),
        transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))
    ])

    train_dataset = SelectiveSearchDataset(root_dir, train=True, transform=trainsform)
    # test_dataset = SelectiveSearchDataset(root_dir, train=False, transform=trainsform)

    img, target = train_dataset.__getitem__(0)
    print(train_dataset.__len__())
    print(img)
    print(target)
    exit(0)

    train_dataloader = DataLoader(train_dataset, batch_size=128, shuffle=True, num_workers=4)
    # test_dataloader = DataLoader(test_dataset, batch_size=128, shuffle=True, num_workers=4)

    # return train_dataloader, test_dataloader
    return train_dataloader


class LeNet5(nn.Module):

    def __init__(self, input_channel=1, num_classes=10):
        super(LeNet5, self).__init__()
        self.conv1 = nn.Conv2d(in_channels=input_channel, out_channels=6, kernel_size=5, stride=1, padding=0, bias=True)
        self.conv2 = nn.Conv2d(in_channels=6, out_channels=16, kernel_size=5, stride=1, padding=0, bias=True)
        self.conv3 = nn.Conv2d(in_channels=16, out_channels=120, kernel_size=5, stride=1, padding=0, bias=True)

        self.pool = nn.MaxPool2d((2, 2), stride=2)

        self.fc1 = nn.Linear(in_features=120, out_features=84, bias=True)
        self.fc2 = nn.Linear(84, num_classes, bias=True)

    def forward(self, inputs):
        x = self.pool(F.relu(self.conv1(inputs)))
        x = self.pool(F.relu(self.conv2(x)))
        x = self.conv3(x)

        x = x.view(-1, self.num_flat_features(x))

        x = F.relu(self.fc1(x))
        return self.fc2(x)

    def num_flat_features(self, x):
        size = x.size()[1:]  # all dimensions except the batch dimension
        num_features = 1
        for s in size:
            num_features *= s
        return num_features


if __name__ == '__main__':
    # train_dataloader, test_dataloader = load_data('./data/VOC_SelectiveSearch')
    train_dataloader = load_data('./data/VOC_SelectiveSearch')
    # num_classes = 2
    #
    # device = "cuda:0" if torch.cuda.is_available() else "cpu"
    #
    # net = LeNet5(input_channel=input_size * input_size * 3, num_classes=num_classes).to(device)
    # print(net)
    # criterion = nn.CrossEntropyLoss().to(device)
    # optimer = optim.SGD(net.parameter(), lr=1e-3, momentum=0.9, nesterov=True)
    #
    # logging.info("开始训练")
    # epoches = 5
    # for i in range(epoches):
    #     num = 0
    #     total_loss = 0
    #     for j, item in enumerate(train_dataloader, 0):
    #         data, labels = item
    #         data = data.to(device)
    #         labels = labels.to(device)
    #
    #         scores = net.forward(data)
    #         loss = criterion.forward(scores, labels)
    #         total_loss += loss.item()
    #
    #         optimer.zero_grad()
    #         loss.backward()
    #         optimer.step()
    #         num += 1
    #     avg_loss = total_loss / num
    #     logging.info('epoch: %d loss: %.6f' % (i + 1, total_loss / num))
    # train_accuracy = compute_accuracy(train_dataloader, net, device)
    # test_accuracy = compute_accuracy(test_dataloader, net, device)
    # logging.info('train accuracy: %f test accuracy: %f' % (train_accuracy, test_accuracy))
