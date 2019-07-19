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

按照 FPS 游戏的方法，使得玩家在场景中的正常移动。主要逻辑是通过监听用户按下的按键触发相应的逻辑：

```
// 按esc键，关闭窗口
if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
if (Controler::isGameOver) return;
if (!Controler::camera.isLock) {
	Player * player = Player::getInstance();
	// 键盘 W S A D 控制玩家角色移动
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player->onKeyDown(Camera::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) player->onKeyDown(Camera::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) player->onKeyDown(Camera::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) player->onKeyDown(Camera::RIGHT, deltaTime);

	// 空格键，跳起
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !player->isJumping) player->onSpaceKeyDownJump();
}
```

#### 贴图  Texture mapping 

由于场景中并无需要法线贴图的部分，这里采用普通的光照贴图，且目标，地面、子弹和障碍物采用不同的贴图，只需在每次渲染对应部分前激活对应的贴图。

其中目标物体为木板贴图，障碍物和子弹为粗糙贴图，地面为大理石贴图，效果如下：

![](images/texture.png)

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

模型加载部分主要用到了 Assimp 库。寻找了枪、手、子弹等模型，使用三维模型处理软件 (Blender) 处理模型，并实现加载和渲染。Assimp 库能支持三维模型的多种文件类型，其中枪和手的模型是使用 Blender 处理后导出的 obj 文件，子弹模型的文件格式是 gltf。在这部分，我们创建了两个主要的类，分别是 `Mesh` 和 `Model` 类。

总体效果图如下：

![图片1](assets/图片1.png)

通过使用 Assimp，我们可以加载不同的模型到程序中，但是载入后它们都被储存为 Assimp 的数据结构。使用 `Mesh` 类可以将这些数据转换为 OpenGL 能够理解的格式。然后使用 Assimp 来加载模型，并将它转换 (Translate) 至多个 `Mesh` 对象，得到完整的 `Model` 对象。

我们使用枪和手的模型作为玩家 (Player)，因为这个游戏是一款第一人称单人射击游戏，所以玩家的视野就看到手和枪了，于是我们让这个模型跟随着 Camera 来移动，而 Camera 看到的东西就是玩家所看到的东西。

对于子弹模型，它原本的自身朝向是不变的，如果不进行相应的旋转，就会出现很奇怪的画面，比如子弹横着被射出，这显然不是我们想要的。于是在玩家点击射击后，子弹生成 (子弹由子弹管理器类生成和管理) 时，首先根据 Camera 的 pitch 和 yaw 进行旋转，使得子弹射出后自身朝向前方，然后再射出子弹。

### Bonus

#### 重力系统 Gravity System

玩家的按空格键模拟重力跳起和下落。效果如下：

![jump](assets/jump.gif)

实现方法如下：

在 `Controller` 类里设置对空格键的按键响应

```c++
if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !player->isJumping) player->onSpaceKeyDownJump();
```

设定一个向下的重力加速度

```c++
const glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);
```

按下空格键后，设置一个向上的初速度，按匀变速运动的物理公式来计算玩家 (Player) 的所在高度

```c++
const float jumpInitSpeed = 10.0f;

this->jumpVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
```

在玩家 (Player) 的渲染函数中，根据 `deltaTime` 来更新玩家的速度，然后更新玩家的位置。当玩家下落至地板时，跳跃事件结束。在游戏场景中，我们需要调整下面的一些参数，使得玩家的跳跃更加自然。

```c++
if (this->isJumping) {
    // 更新玩家的速度
	jumpVelocity += gravity * Controler::deltaTime * 3.0f;
    // 更新玩家的位置
	this->position += jumpVelocity * 0.04f;

    // 当玩家下落至地板时，跳跃事件结束
	// 3.2f是摄像机初始position的y值
	if (this->position.y < 3.2f) {
		this->position.y = 3.2f;
		this->isJumping = false;
	}
    // 同时更新Camera的位置
	Controler::camera.setPosition(this->position);
}
```

#### 碰撞检测 Collision Detection

子弹 (Bullet) 与目标物体 (Target) 的碰撞检测，当某一个 Bullet 与 Target 发生碰撞时，该 Bullet 消失，Target 随机改变位置。

遍历所有子弹，如果某个子弹与 Target 的距离小于目标物体的大小 (一个阈值) 时，销毁该子弹，并随机改变Target 的位置，准备进行下一次射击。

![shoot](assets/shoot.gif)

#### 射击 Shotting

鼠标左键能够沿着摄像机朝向(front)发射子弹；通过 BulletManager 管理子弹的创建、运动、销毁；在鼠标监听事件回调中触发射击逻辑，主要是获取用户（等价于摄像机）的front向量、及当前位置，然后调用 `bulletManager` 生成新的子弹实例：

```
if (action == GLFW_PRESS) {
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		glm::vec3 front = Controler::camera.getFrontVec();
		glm::vec3 pos = Controler::camera.getPosition();
		Controler::getInstance()->bulletManager->newBullet(front, pos + front * 2.0f);
		break;
	default:
		break;
	}
}
```

#### 天空盒  Sky Box 

添加天空盒，准备多个天空盒资源。天空和运用了多面体的贴图，对一个Cube的六个面分别贴上不同的贴图来模拟天空，这里用到的方法是GL_TEXTURE_CUBE_MAP_POSITIVE_X。

天空盒要始终保持在视野的远方，不随摄像机的移动而发生远近的平移，因此需要在开始就去除viewModel矩阵中的平一部分：

``` c++
glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()))
```

上述代码在glm::mat3时去除了矩阵的平移部分。实现效果：

![](images/sky1.png)

![](images/sky2.png)

#### 倒计时 Text Rendering

使用 FreeType 库加载 .ttf 字体文件，通过 FontRenderer 管理字体加载、相关字符数据保存、渲染调用；主要功能有：

-  加载TrueType字体并为每一个字形生成位图以及计算度量值(Metric)
-  提取出它生成的位图作为字形的纹理，并使用这些度量值指定每个字符的大小和位置

![](images/text.png)


## 遇到的问题和解决方案

### Assimp 配置

在这个项目中，我们使用 Assimp 库来加载模型。然而，对于初次使用者来说，配置 Assimp 库并不是一件简单的事！我们在 [这里](../配置Assimp.md) 总结了 Assimp 的配置方法以及过程中遇到的困难与解决方案。

### 正交投影矩阵

在实验中我们采用的光源的投影矩阵是正交矩阵。在实现过程中曾经出现过一些问题：

- 在一定距离之外的物体没有渲染出来，只有走近之后才会被渲染。
- 有的物体没有阴影在地板上。

**解决方案**

第一次只发现了第二个现象，因此会猜测是不是阴影贴图部分的代码写错了，结果对照教程修改也没有发现什么错误，直到发现了第一个现象，原因就很明显了：是视野投影矩阵的问题。

在项目中我们采用的是正交矩阵来作为光源的：

```c++
//ScenceController.cpp
GLfloat near_plane = 0.1f, far_plane = 200.0f;
glm::mat4 lightProjection = glm::mat4(1.0f);
lightProjection = glm::ortho(-70.0f, 70.0f, -70.0f, 70.0f, near_plane, far_plane);
```

关于没有阴影，有两种情况出现：一个是光源太高，高于far_plane导致没有在渲染空间中，一个是投影角度太大导致阴影超出了地面范围。不管是哪个其实都是投影矩阵的参数问题，因此解决方法其实就是调节参数设置。在项目中，我尽量让光源的位置与天空盒的太阳所在的方向相契合，并且正交参数也会影响到视野，因此最终效果也不能说是两全其美，只能说是尽量满足要求。

另外，正交参数中（left, right, bottom, top）控制光源的正交投影的范围还可以控制地面上阴影的清晰度，因此需要调整到一个适合的值。

### 天空盒贴图错位

在一开始加载天空盒时出现了顶部的天空盒贴图的边缘与周围的贴图边缘错位的问题，贴图本身是完整的，但是边与边的对应关系不对：

![](images/sky3.png)

**解决方法**

其实也没什么，结果也不是代码问题，而是图片本身的角度问题，两种方案：要么在读取图片时进行旋转，要么直接旋转图片文件，我是懒得再去写代码的人因此就只写用ACDsee软件将顶部（top）天空盒文件（.tga文件）旋转至合适的角度，这样直接读取贴图时就是正确的角度了。

### 随机大小障碍物的碰撞检测

对于碰撞检测，我们采用了最简单的距离计算的方法，如果子弹和物体位置之间的距离小于某个阈值的话，我们就认为它们相碰撞了，对于一个正方体来说，我们将阈值设为它的边长的根号3倍，因为这是正方体上的点到正方体的最大距离（最长对角线距离）。

![](images/cube.png)

但是障碍物在生成时边长时随机的，因此不能用一个确定的阈值来判断，否则在体验上会有很多误差。

**解决方法**

在生成障碍物时记录下每个障碍物的大小和位置，由于障碍物不会发生移动，新建和删除，因此这些信息是不会更新的，可以持续利用。由于位置与大小信息是按顺序保存的，在进行碰撞检测时可以同时检测位置和大小，根据每个碰撞五的大小进行距离检测：

``` c++
//遍历子弹
for (auto it_bullet : Controler::getInstance()->bulletManager->getBulletLists()) {
    //获取障碍物位置
    std::vector<glm::vec3> newBarrierPositions = sceneController->getBarrierNewPositions();
    std::vector<float> barrierSizes = sceneController->getBarrierSizeSet();
    //遍历障碍物
    for (int i = 0; i < newBarrierPositions.size(); i++) {
        glm::vec3 barrierPosition = newBarrierPositions[i];
        //针对每个障碍物的大小计算距离
        if (getDistance(barrierPosition, it_bullet->getPosition()) < barrierSizes[i] * PhysicsEngine::thresholdBarrierDistance) {
            Controler::getInstance()->bulletManager->deleteBullet(it_bullet->id);
            return true;
        }
    }

}
```

### 人物活动范围限制

由于场景地面范围有限，人物活动时可能会离开场景。

**解决方法**

增加人物活动范围限制。如下图，设ABCD为四边形的四个顶点。

![](images/sceneControl.png)

若点P在四边形内，则如下公式中，应有a、b、c、d同时大于零。

![](images/formula.png)

在实际场景中，令P为人物的当前位置，则可通过上述原理限制人物活动在场景内。

```c++
void Camera::processKeyBoard(const CameraMovement direction, const float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	
	glm::vec3 tempFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
	glm::vec3 tempRight = glm::normalize(glm::cross(tempFront, worldUp));
	if (direction == FORWARD) position += tempFront * velocity;
	if (direction == BACKWARD) position -= tempFront * velocity;
	if (direction == LEFT) position -= tempRight * velocity;
	if (direction == RIGHT) position += tempRight * velocity;

	if (!isNowInBoundary(this->activeBoundary)) {
		if (direction == FORWARD) position -= tempFront * velocity;
		if (direction == BACKWARD) position += tempFront * velocity;
		if (direction == LEFT) position += tempRight * velocity;
		if (direction == RIGHT) position -= tempRight * velocity;
	}
}
```



## 小组成员

| 姓名   | 学号     | 分工                                     | Github                            |
| ------ | -------- | ---------------------------------------- | --------------------------------- |
| 邵梓硕 | 16340189 | 天空盒、贴图映射、阴影映射、简单光照、目标物体、碰撞检测的一部分 |    [lossatsea](https://github.com/lossatsea)                               |
| 伍正涛 | 16340246 | 项目构建、配置环境、模型寻找、处理、加载，重力系统，碰撞检测 | [wuzht](https://github.com/wuzht) |
| 沈大伟 | 16340190 | 模型寻找、场景处理，复杂光照处理           | [chain](https://github.com/freesdw)   |
| 李杰泓 | 16340118 |  实现利用鼠标、键盘控制人物移动；实现子弹射击; 显示游戏信息;  | [carolsum](https://github.com/carolsum)   |



## 个人报告 —— 沈大伟

在本次项目中，我主要负责模型的寻找、场景的搭建和处理、多张纹理贴图的处理以及复杂光照的处理。

在光照模型上，我使用了Blinn-Phong光照模型，该模型也是分为环境光、漫反射和镜面反射三个部分。Phong光照模型的镜面反射由人的视角线和反射光的夹角决定，会出现高光断层现象。Blinn-Phong模型对此进行改进，通过计算半程向量，使用视角和半程向量来计算镜面反射，从而解决了高光断层现象。同时，为显示增加了gamma矫正，gamma值设定为2.2

在场景搭建这部分工作，历程较为曲折。最初计划是搭建一个地表形状复杂，且地表平面具有较多的遮挡物来遮挡住目标，从而使得玩家要去寻找射击的目标。为此我找了一个较为复杂的地形模型，并导入作为场景。但后来发现，因为地表凹凸不平，且遮挡物的形状都很复杂，所以碰撞检测实现难度很高，故放弃了以模型作为场景的想法。后来便以一个平面作为加上贴图作为基本场景，控制目标物体随机在一些位置上出现，同时使用带贴图的正方体作为障碍物，随机生成障碍物来增加玩家寻找目标的难度。同时，通过向量间余弦值的规律，限制玩家的活动范围在地面场景之内。

在这次项目中，我充分认识到了项目前期规划的重要性以及小组成员之间相互交流的重要性。正是因为我们前期规划只是设想了想要实现的效果，却没有想到可能遇到的技术难题以及这些难题能否解决，才导致了后面不得不重新构建场景，而曾经的设想也得不到好的实现。此外，由于小组成员之间缺乏足够的交流，项目进度较慢，导致后来更改需求后没有足够的时间再来完成更改后的设想。

## 个人报告 —— 伍正涛

在本项目中，我的任务是项目构建、配置环境、模型寻找、处理、加载，重力系统，碰撞检测等。

在项目开始时，我搭建了整个项目并配置了好所需的环境，实现好常用的类，方便队友们后面进行开发，并使用 Github 来管理我们的项目。我配置了 GLFW, ImGui, stb_image, Assimp 等库，其中我配置 Assimp 时，遇到的困难比较多，为了配置它，我花费了不少时间，最后我在 [这里](../配置Assimp.md) 总结了 Assimp 的配置方法以及过程中遇到的困难与解决方案。

首先，我设计了一个单例类 `Controller`，它是整个项目的控制器，封装了 GLFW、ImGui 中的各种常用操作，并且各种输入 (鼠标和键盘) 的相应事件都在这个控制器类内定义和注册，它还管理 camera 对象。然后，我还添加了`Camera`, `Mesh`, `Model` 类。

寻找模型以及它的处理也不是一件容易的事，在网上有很多炫酷的模型，但并不是下载下来就能立刻使用的。比如我们的枪和手的模型，它们原本是一个三维的动画，我们无法将它直接加载到项目中去，于是使用 Blender 软件对它进行编辑，调整模型的形状和位置，并添加纹理，最后导出 obj 文件用于在项目中加载。

我们这个项目是一个第一人称射击游戏，子弹与目标的碰撞检测是一个重点，由于我们的项目的碰撞对象与事件比较简单，而且对象数量较少，我并没有使用一些碰撞检测库，而是使用了一些简单的方法来解决这个问题。我们的碰撞场景是，当一个 Bullet 与 Target 发生碰撞时，它们两者都应该消失，并更新分数；当 Bullet 与障碍物碰撞时，Bullet 消失，障碍物不消失。我编写了 `PhysicsEngine` 类，遍历所有子弹，如果某个子弹与 Target 的距离小于目标物体的大小 (一个阈值) 时，销毁该子弹，被射中的 Target 也会消失，准备进行下一次射击。当然，如果我们要构建一个多人射击对战这样的复杂游戏时，就不能用这些简单的方法来实现碰撞检测了，当游戏对象变得更多时，这个方法的计算量会非常大。对于重力系统的实现，还是比较简单的，当空格键被按下，设置一个向上的初速度和一个向下的加速度，按照匀变速运动的公式，通过渲染循环中计算得到的 `deltaTime` 来更新 Player 的位置即可。

在这个项目中，是计算机图形学的综合项目，我们在这里结合使用了本学期学到的各种图形学知识，实现了我们设想中的小游戏。虽然做出来的效果并不是很棒，但做的过程中还是非常有趣的，因为这个项目融合了这个学期学到的许多内容，而且还包含了课程上没有学到的知识。对于这次项目，我觉得这是对本学期的学习内容的一次非常好的总结和提升。

## 个人报告 —— 邵梓硕

在本次项目中我负责的部分有很多是基础功能，虽然是上课的时候就讲过的内容，实际在项目中会根据需求不同，应用的场景不同，实现方式也不同，也会出现不知道怎么实现，出现各种bug的情况，因此即使是基础功能也不是一开始想的一下就能过的简单工作。从项目的初始阶段计划到最后的成品，可以说除了游戏类型之外没什么是一样的，即使一开始计划的内容很好，在制作过程中也会逐渐发现难度完全不是我们想的那么简单，加上其他课程的作业，时间也变得很紧张，最后还有学校的夏令营，就结果而言砍掉了很多内容。结果基本做好的粒子系统也是没有时间将它和项目合在一起，只能说是非常遗憾......作为计算机图形学的最后一个项目，虽然是个很简单的小游戏，但还是尽量结合了我在这个学期学习到的知识，是个很好的经验。

## 个人报告 -- 李杰泓
本次大作业是计算机图形学这门课程的一个实践，从寻找模型、场景建模，到 Camera Roaming，到光照、阴影、着色，到纹理映射，这些课程的基础内容，再到，天空盒设计、文字渲染、重力系统，物体的碰撞检测，阴影抗锯齿、Gamma矫正。即是对课程内容的巩固，也是计图项目开发实操。在这次大项目中，我主要负责以下内容：

- 实现 Camera Roaming，	并添加单实例 Player 类，controller 监听鼠标、键盘事件分发给 Player；然后将Camera 绑定到 Player 上，通过 Player 更新 Camera 位置，并且与 Camera 的 yaw 同步，从而实现视角转动时改变 Player 自身朝向；支持上帝视角模式；
- 实现射击操作，添加鼠标点击事件监听，左键点击时触发子弹发射；采用 ** 工厂模式 **，通过 BulletManager 维护子弹的生成、渲染、销毁；实现子弹与障碍物碰撞检测；
- 使用 FreeType 库加载 .ttf 字体文件，通过 FontRenderer 管理字体加载、相关字符数据保存、渲染调用；
- ...

比较尴尬的是，我们这次项目展示的时候与上一组展示的内容有点类似，场面一度尴尬😅。然而我们的所有功能点确实都是自己摸索自己实现的，只能说围绕几个要求点展开内容选题时有点容易撞车吧。

在这次期末项目开发过程中，我们还是遇到了蛮多问题的，比如一开始搭建项目环境，每个人的 vs 配置不同，然后在引用库文件的时候会有一些找不到xxx之类的报错，这个挺影响开发效率的，后面我们把项目用到的库、ddl、其他配置等都写在项目中，让vs运行时优先读取项目目录下的配置；还有比如 C++ 头文件循环引用的问题，我们几个组员在同时开发的过程中向不同的类的头文件加了通用工具类的声明头时触发了循环引用的问题，导致 vs 产生了很奇怪的报错，当时也是很坑。后面将一些公用的类抽到一个专门的 .h 头文件中引用解决了这个问题。总而言之，这次期末项目在运用计算机图形学课程的知识的同时，也很好的锻炼了我们vs开发C++项目的能力，虽然还有一些功能实现了但是最终没上（比如我做了一个火焰爆炸粒子效果，考试后去实习没时间集成到项目里）有些遗憾，但还是挺有收获的。
