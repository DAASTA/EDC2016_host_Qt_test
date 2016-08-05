# EDC2016 host - Qt test

电设上位机 - Qt 前期开发文件

## 编译

- VS2015
- Qt5 for VS2015
- OpenCV

如有必要，按照以下操作安装

- 安装 VS2015 Community
- 下载 Qt5.6 opensource windows msvc2015 64
- 进入 VS2015，工具->拓展和更新=>联机，搜索安装 Qt5Package；重启VS2015
- 进入 VS2015，Qt5->Qt Options=>Add，路径格式类似 `D:\Qt\Qt5.6.1\5.6\msvc2015_64`

现在可以打开一个普通的Qt工程了，但是我们还需要OpenCV的依赖。

OpenCV的配置方案比较多，这里简要概括。

> 文件夹下已经有了一个`opencv2410.props`文件，此文件需要一个名为`OPENCV_ROOT`，值为类似`D:\OpenCV\opencv\build`的环境变量才能工作。
> <br>如果版本不同，直接打开之，替换`2410`为你的版本号。
> <br>如果不需要此文件，可以在VS中，视图->其他窗口->属性管理器=>展开所有目录，将`opencv2410`删除。

- 下载安装 OpenCV 2.x (暂时不要安装OpenCV 3)
- 设置环境变量，将类似`D:\OpenCV\opencv2410\build\x64\vc12\bin`的目录添加到`PATH`下
- 设置项目的编译
    - 直接在视图->其他窗口->属性管理器中用props文件完成
    - 或者 更新项目的属性
        - VC++目录=>包含目录 或 C/C++=>附加包含目录
        - VC++目录=>库目录 或 连接器=>附加库目录
        - 连接器=>输入=>附加依赖项

