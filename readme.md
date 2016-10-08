# EDC2016 host - Qt test

电设上位机 - Qt 前期开发文件

## 编译

> **[Warning]** 为了团队开发Qt的方便，我们在一些分支中 gitignore 了 `pro1.vcxproj` 文件。如果需要，可以进入tag `integrate-0` 拷贝和修正之。

- VS2015
- Qt 5 for VS2015_64
- OpenCV 2

## 功能块

主要功能都已经模块化，作为.hpp文件，分别放在以下文件夹中

- include/camera 摄像头
- include/communication 串口，udpsocket
- include/locator 色块定位
- include/logic 游戏逻辑
