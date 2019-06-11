# CGProject

> A computer graphics project



## 更新天空盒	邵梓硕

#### 文件变动：

- 新增文件夹`resourse`

  资源文件夹，目前只有skybox，已经预备了四个天空盒来使用，之后的模型资源都可以放到这个文件夹中。

- 新增类

  SkyBox.h：天空盒头文件。

  SkyBox.cpp：天空盒源文件。

- 修改

  修改main文件，加入天空盒操作，去除了贝塞尔曲线部分。

#### 天空盒的使用

**有的天空盒底部根本没有图片，因此摄像机的仰角俯角最好限制在一定角度之间，这部分交给Camera Roaming的同学。**

1. 在main.cpp文件中include天空盒头文件。

2. 初始化天空盒，确定要使用哪个天空盒：

   ``` c++
   //skybox, 这里使用的是envmap_miramar天空盒
   SkyBox skybox("envmap_miramar");
   ```

3. 在主循环中调用skybox.render()函数，参数是摄像机：

   ``` c++
   //注意这步不能缺
   glDepthFunc(GL_LEQUAL);
   //render
   skybox.render(Controler::getInstance()->camera);
   ```

---

## 运行方法

> 运行前，请确认已安装：
>
> - Windows SDK 版本：10.0.177.63.0
> - 平台工具集：Visual Studio 2017 (v141)

步骤：

1. 打开命令行，将项目克隆到本地

   ```sh
   git clone https://github.com/sysu-computer-graphics/CGProject.git
   ```

2. 打开 `CGProject.sln`

   如果出现以下窗口，`Windows SDK 版本` 和 `平台工具集` 都选择 `无升级`

   ![2](assets/2.png)

3. 选择 x86

   ![1](assets/1.png)

4. 运行



## 文件结构

文件夹

* `Includes`

  存放 `GLFW, glad, glm, KHR, std_image.h` 等文件

* `Libs`

  存放 `glfw3.lib`

* `ImGui`

  ImGui 的代码

* `GLSL`

  存放着色器代码

文件

* 已经写好 `Camera`，`Shader` 类
* `Controler` 类是控制器类，初始化 GLFW 以及绑定各种回调函数，定义回调函数的行为等
* 渲染循环在 `main.cpp` 里



## References

OpenGL 环境配置参考：[【PT君的OpenGL】第一天【VS2017+OpenGL环境的配置(固定管线+可编程管线)】](https://blog.csdn.net/qq_19003345/article/details/76098781)

