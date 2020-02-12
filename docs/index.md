# SelectiveSearch

## 目标

学习`OpenCV`实现的选择性搜索算法。分两部分

* 第一部分是图分割算法
* 第二部分是选择性搜索算法

## 背景

当前使用`OpenCV 4.2.0`，其头文件和源文件位于：

```
# 头文件
/path/to/opencv_contrib/modules/ximgproc/include/opencv2/ximgproc/segmentation.hpp
# 源文件
/path/to/opencv_contrib/modules/ximgproc/src/graphsegmentation.cpp
/path/to/opencv_contrib/modules/ximgproc/src/selectivesearchsegmentation.cpp
```

将源文件拆分成多个目标文件，方便进一步的理解