# CGProject

> A computer graphics project



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

