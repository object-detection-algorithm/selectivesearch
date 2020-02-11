# selectivesearch

[![Documentation Status](https://readthedocs.org/projects/selectivesearch/badge/?version=latest)](https://selectivesearch.readthedocs.io/zh_CN/latest/?badge=latest) [![standard-readme compliant](https://img.shields.io/badge/standard--readme-OK-green.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme) [![Conventional Commits](https://img.shields.io/badge/Conventional%20Commits-1.0.0-yellow.svg)](https://conventionalcommits.org) [![Commitizen friendly](https://img.shields.io/badge/commitizen-friendly-brightgreen.svg)](http://commitizen.github.io/cz-cli/)

> selectivesearch算法实现

学习论文[Selective Search for Object Recognition](http://xueshu.baidu.com/usercenter/paper/show?paperid=b689fcf3ed998dfbb4213687367b6175&site=xueshu_se)以及`OpenCV`实现

## 内容列表

- [背景](#背景)
- [安装](#安装)
- [用法](#用法)
- [主要维护人员](#主要维护人员)
- [参与贡献方式](#参与贡献方式)
- [许可证](#许可证)

## 背景

`selectivesearch`（选择性搜索）是`R-CNN`使用的候选区域检测算法，其使用基于图的分割算法，并通过不同的颜色空间组合+多样化特征提取+分组分层合并算法生成候选区域

## 安装

本地编译文档需要预先安装以下工具：

```
$ pip install mkdocs
```

## 用法

### 文档浏览

有两种使用方式

1. 在线浏览文档：[selectivesearch](https://github.com/zjZSTU/selectivesearch.git)

2. 本地浏览文档，实现如下：

    ```
    $ git clone https://github.com/zjZSTU/selectivesearch.git
    $ cd selectivesearch
    $ mkdocs serve
    ```
    启动本地服务器后即可登录浏览器`localhost:8000`

## 主要维护人员

* zhujian - *Initial work* - [zjZSTU](https://github.com/zjZSTU)

## 参与贡献方式

欢迎任何人的参与！打开[issue](https://github.com/zjZSTU/selectivesearch/issues)或提交合并请求。

注意:

* `GIT`提交，请遵守[Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0-beta.4/)规范
* 语义版本化，请遵守[Semantic Versioning 2.0.0](https://semver.org)规范
* `README`编写，请遵守[standard-readme](https://github.com/RichardLitt/standard-readme)规范

## 许可证

[Apache License 2.0](LICENSE) © 2020 zjZSTU
