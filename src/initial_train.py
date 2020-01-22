# -*- coding: utf-8 -*-

"""
@author: zj
@file:   initial_train.py
@time:   2020-01-22
"""

import os
import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torch.utils.data import Dataset
from torch.utils.data import DataLoader
import torchvision.transforms as transforms

input_size = 32


class SelectiveSearchDataset(Dataset):

    def __init__(self, root_dir, train=True, transform=None):
        self.root_dir = root_dir
        self.train = train
        self.transform = transform

        if self.train:
            cate_list, positive_array, negative_array = self._load_csv(root_dir)
            print(cate_list)
            print(positive_array)
            print(negative_array)

    def __getitem__(self, item):
        pass

    def __len__(self):
        pass

    def _load_csv(self, root_dir):
        positive_array = list()
        negative_array = list()

        cate_list = os.listdir(root_dir)
        for cate in cate_list:
            cate_dir = os.path.join(root_dir, cate)
            positive_dir = os.path.join(cate_dir, 'positive')
            negative_dir = os.path.join(cate_dir, 'negative')

            csv_list = os.listdir(positive_dir)
            for csv_name in csv_list:
                positive_csv_path = os.path.join(positive_dir, csv_name)
                negative_csv_path = os.path.join(negative_dir, csv_name)

                positive_array.append(positive_csv_path)
                negative_array.append(negative_csv_path)

        return np.array(cate_list), np.array(positive_array), np.array(negative_array)


def load_data(root_dir):
    trainsform = transforms.Compose([
        transforms.ToPILImage(),
        transforms.Resize(input_size),
        transforms.ToTensor(),
        transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))
    ])

    train_dataset = SelectiveSearchDataset(root_dir, train=True, transform=trainsform)
    # test_dataset = SelectiveSearchDataset(root_dir, train=False, transform=trainsform)

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

    device = "cuda:0" if torch.cuda.is_available() else "cpu"

    net = LeNet5(input_channel=3, num_classes=2).to(device)
    print(net)
    criterion = nn.CrossEntropyLoss().to(device)
    optimer = optim.SGD(net.parameter(), lr=1e-3, momentum=0.9, nesterov=True)
