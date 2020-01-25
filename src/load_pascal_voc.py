# -*- coding: utf-8 -*-

"""
@author: zj
@file:   load_pascal_voc.py.py
@time:   2020-01-25
"""

from torchvision.datasets import VOCDetection

if __name__ == '__main__':
    dataset = VOCDetection('./data', year='2007', image_set='train', download=True, transform=None)

