# -*- coding: utf-8 -*-

"""
@author: zj
@file:   initial_train.py
@time:   2020-01-22
"""

import logging
import torch
from torch.utils.data import DataLoader
import torchvision.transforms as transforms
from dataset import SelectiveSearchDataset
from lenet5 import LeNet5

logging.basicConfig(format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s', level=logging.DEBUG)

input_size = 32


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


if __name__ == '__main__':
    # train_dataloader, test_dataloader = load_data('./data/VOC_SelectiveSearch')
    # train_dataloader = load_data('./data/VOC_SelectiveSearch')
    num_classes = 2

    device = "cuda:0" if torch.cuda.is_available() else "cpu"

    net = LeNet5(input_channel=3, num_classes=num_classes).to(device)
    print(net)
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
