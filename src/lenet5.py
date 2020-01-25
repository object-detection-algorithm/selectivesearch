# -*- coding: utf-8 -*-

"""
@author: zj
@file:   lenet5.py
@time:   2020-01-25
"""

import torch.nn as nn
import torch.nn.functional as F


class LeNet5(nn.Module):

    def __init__(self, input_channel=3, num_classes=10):
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
    net = LeNet5(input_channel=3, num_classes=10)
    print(net)
