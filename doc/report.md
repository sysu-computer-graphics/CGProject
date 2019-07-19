# 项目报告

## 项目介绍

实现的项目为第一人称单人射击游戏，目标为在一定时间内避开障碍物，射击更多目标物体。基本规则如下：

- 玩家通过WSAD和空格控制角色的移动与跳跃，鼠标左键射击，鼠标的移动控制视角的转动。
- 目标物体与障碍物的贴图不同。集中目标物体左下角分数+1，累计每击中3次障碍物左下角分数-1，没有负分。
- 时间限制在窗口上方，时间为0时游戏结束。
- 角色移动范围控制在游戏区域内，无法移动至区域外。

**实现结果**

见同目录下的演示视频。

## 开发环境

- 环境：Windows
- 工具：Visual Studio
- 第三方库：assimp、glfw3、freetype

## 实现功能

| 功能                                                        | 实现部分                                   | 类型  |
| ----------------------------------------------------------- | ------------------------------------------ | ----- |
| Camera Roaming                                              | 玩家视角摄像机                             | Basic |
| Simple lighting and shading(phong)                          | 地面、目标物体、障碍物的光照               | Basic |
| Texture mapping                                             | 地面、目标物体、障碍物、子弹的贴图         | Basic |
| Shadow mapping                                              | 目标物体、障碍物的阴影                     | Basic |
| Model import & Mesh viewing (Assimp)                        | AK（附手臂）的模型加载                     | Basic |
| Sky Box (天空盒)                                            | 天空盒                                     | Bonus |
| Display Text (显示文字，中文/英文/立体/平面)                | 分数，倒计时                               | Bonus |
| Gravity System and Collision Detection (重力系统与碰撞检测) | 跳跃功能，子弹与物体的碰撞检测             | Bonus |
| Anti-Aliasing (抗锯齿)                                      | 阴影的抗锯齿                               | Bonus |
| Gamma矫正                                                   | 阴影的Gamma矫正                            | Bonus |
| 射击                                                        | 包括子弹的产生、调整、运动、消失，点击响应 | Bonus |

## 对实现功能的简单介绍

### Basic

#### 摄像机 Camera Roaming

按照 FPS 游戏的方法，使得玩家在场景中的正常移动

#### 贴图  Texture mapping 

由于场景中并无需要法线贴图的部分，这里采用普通的光照贴图，且目标，地面、子弹和障碍物采用不同的贴图，只需在每次渲染对应部分前激活对应的贴图。

其中目标物体为木板贴图，障碍物和子弹为粗糙贴图，地面为大理石贴图，效果如下：

![](E:/course/%E8%AE%A1%E7%AE%97%E6%9C%BA%E5%9B%BE%E5%BD%A2/CGProject/doc/images/texture.png)

#### 阴影 Shadow mapping 

**实现**：对目标物体进行阴影映射，并对基本阴影映射进行优化：去除线条样式的阴影失真、去除悬浮（由于目标物体本身悬浮效果不明显）、PCF。

其中，线条样式的阴影失真通过阴影偏移来解决，对表面的深度应用一个偏移量，使得采样点都获得比表面深度都小的深度值。效果见上面的图。

去除悬浮是通过正面剔除（glCullFace(GL_FRONT)），效果见下图

PCF的他一个简单实现是从纹理像素的四周进行多次采样，把结果平均起来得到最终的像素值，这也是抗锯齿的原理，效果见下图。

![](images/shadow1.png) ![](images/shadow2.png)

#### 光照 Simple lighting（Phong）

光照采用Blinn-Phong光照，并增加gamma矫正，gamma值设定为2.2。各个参数在场景初始化中设定。在本次项目中光源位置和光源视角的设置比较麻烦。

![](images/light.png)

#### 模型加载 Model import & Mesh viewing

- 配置和使用 Assimp 库
- 寻找了枪、手、场景、子弹等模型，使用三维模型处理软件 (Blender) 处理模型，并实现加载和渲染

### Bonus

#### 重力系统 Gravity System

玩家的按空格键模拟重力跳起和下落

#### 射击 Shotting

鼠标左键能够沿着摄像机朝向(front)发射子弹；通过 BulletManager 管理子弹的创建、运动、销毁；

#### 天空盒  Sky Box 

添加天空盒，准备多个天空盒资源。天空和运用了多面体的贴图，对一个Cube的六个面分别贴上不同的贴图来模拟天空，这里用到的方法是GL_TEXTURE_CUBE_MAP_POSITIVE_X。

天空盒要始终保持在视野的远方，不随摄像机的移动而发生远近的平移，因此需要在开始就去除viewModel矩阵中的平一部分：

``` c++
glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()))
```

上述代码在glm::mat3时去除了矩阵的平移部分。实现效果：

![](images/sky1.png)

![](images/sky2.png)



## 遇到的问题和解决方案



## 小组成员

| 姓名   | 学号     | 分工                                     | Github                            |
| ------ | -------- | ---------------------------------------- | --------------------------------- |
| 邵梓硕 | 16340189 | 天空盒、贴图映射、阴影映射、简单光照、目标物体、碰撞检测的一部分 |    [lossatsea](https://github.com/lossatsea)                               |
| 伍正涛 | 16340246 | 模型寻找、处理、加载，重力系统，碰撞检测 | [wuzht](https://github.com/wuzht) |
| 沈大伟 | 16340190 | 模型寻找、场景处理，复杂光照处理           | [chain](https://github.com/freesdw)   |
| 李杰泓 | 16340118 |  实现利用鼠标、键盘控制人物移动；实现子弹射击; 显示游戏信息;  | [carolsum](https://github.com/carolsum)   |


