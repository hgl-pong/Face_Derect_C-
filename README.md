# Face_Derect_C-

### 简介
***
使用opencv ffmpeg对视频进行解码得到yuv格式图像帧（**Decoder.h**），再由face_detector对图像帧（从**YUV**转为**RGB**，再转成灰度图进行处理）进行人脸检测并保存人脸矩形信息。然后根据**face_detector**返回的值使用OpenGL进行滤镜渲染。（目前可以对视频的一帧进行检测、滤镜添加）

将进行的优化：
1. 完整视频检测
2. 内存优化
3. 采用多线程提高效率

### 环境配置
***
opencv 3.4.1版本
下载链接：<https://sourceforge.net/projects/opencvlibrary/files/opencv-win/3.4.1/opencv-3.4.1-vc14_vc15.exe/download>

### 目录结构描述
***
```
│  Face_Detect.gitignore
│  README.md
│  tree.txt //目录结构
├─.github
├─.vs
└─Face_Detect_C
    │              
    ├─Face_Detect_C
    │  │  decoder.h  //视频解码器
    │  │  face_detector.h  //人脸检测器
    │  │  FilerAdder.h  //滤镜添加
    │  │  main.cpp  //主程序
    │  │  Saver.h  //视频保存
    │  │  shape_predictor_68_face_landmarks.dat  //人脸形状预测数据库
    │  │  
    │  └─x64
    │                  
    ├─packages
    │                          
    ├─video
    │      test.mp4
    │      
    └─x64

```

### 运行截图
![运行截图](./Face_Detect_C/test.png "运行截图")