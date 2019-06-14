# 项目报告

## 小组成员

| 姓名   | 学号     | 分工                                     | Github                            |
| ------ | -------- | ---------------------------------------- | --------------------------------- |
| 邵梓硕 | 16340189 | 天空盒、贴图映射、阴影映射、简单光照         |    [lossatsea](https://github.com/lossatsea)                               |
| 伍正涛 | 16340246 | 模型寻找、处理、加载，重力系统，碰撞检测 | [wuzht](https://github.com/wuzht) |
| 沈大伟 | 16340190 | 模型寻找、场景处理，复杂光照处理           | [chain](https://github.com/freesdw)   |
| 李杰泓 |          |                                          |                                   |



## 摄像机 Camera Roaming

按照 FPS 游戏的方法，使得玩家在场景中的正常移动



## 模型加载 Model import & Mesh viewing

* 配置和使用 Assimp 库

* 寻找了枪、手、场景、子弹等模型，使用三维模型处理软件 (Blender) 处理模型，并实现加载和渲染



## 重力系统 Gravity System

玩家的按空格键模拟重力跳起和下落



## 天空盒  Sky Box 

添加天空盒，准备多个天空盒资源。

## 贴图  Texture mapping 

- 天空盒贴图采用GL_TEXTURE_CUBE_MAP进行多面体的多图贴图。
- 目标物体采用深度贴图、表面贴图。

## 阴影 Shadow mapping 

对目标物体进行阴影映射，并对基本阴影映射进行优化：去除线条样式的阴影失真、去除悬浮（由于目标物体本身悬浮效果不明显）、PCF。

## 光照 Simple lighting（Phong）

光照采用Phong光照，并提供可控的参数。
