# -*- coding: utf-8 -*-

"""
@author: zj
@file:   bndbox_detect.py
@time:   2020-01-26
"""

import copy
import cv2
import torch
import torch.nn as nn
import torchvision.transforms as transforms

from lenet5 import LeNet5

input_dim = 32


def load_model():
    net = LeNet5(input_channel=3, num_classes=2)
    net.load_state_dict(torch.load('./model/lenet5.pth'))
    net.eval()

    return net


if __name__ == '__main__':
    net = load_model()
    # print(net)
    # print(net.parameters())

    img_path = './data/VOCdevkit/VOC2007/JPEGImages/000007.jpg'
    img = cv2.imread(img_path, cv2.IMREAD_COLOR)

    selectiveSearchSegmentation = cv2.ximgproc.segmentation.createSelectiveSearchSegmentation()
    selectiveSearchSegmentation.setBaseImage(img)
    selectiveSearchSegmentation.switchToSelectiveSearchQuality()
    rects = selectiveSearchSegmentation.process()

    # change [x,y,w,h] to [xmin,ymin,xmax,ymax]
    rects[:, 2] += rects[:, 0]
    rects[:, 3] += rects[:, 1]

    # for xmin, ymin, xmax, ymax in rects:
    #     cv2.rectangle(img, (xmin, ymin), (xmax, ymax), (0, 0, 255), thickness=1)
    # cv2.imshow('img', img)
    # cv2.waitKey(0)

    transform = transforms.Compose([
        transforms.ToPILImage(),
        transforms.Resize((input_dim, input_dim)),
        transforms.ToTensor(),
        transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))
    ])

    res_img = transform(img)
    print(res_img.shape)
    res_img = res_img.unsqueeze(0)
    print(res_img.shape)

    res = net.forward(res_img)
    print(res)

    softmax = nn.Softmax(dim=1)
    res = softmax.forward(res)
    print(res)

    idx = torch.argmax(res).item()
    print('类别是:', idx)

    rec_img = copy.deepcopy(img)
    for xmin, ymin, xmax, ymax in rects:
        data = img[ymin:ymax, xmin:xmax]
        # 格式化输入
        transform_img = transform(data)
        inputs = transform_img.unsqueeze(0)
        # 模型计算
        scores = softmax.forward(inputs)
        idx = torch.argmax(scores).item()
        if idx == 1:
            cv2.rectangle(rec_img, (xmin, ymin), (xmax, ymax), (0, 0, 255), thickness=1)
    cv2.imshow('rec_img', rec_img)
    cv2.waitKey(0)
