# 2024程序组笔试题

## 1. 软件配置/注册（必做）
### 1. GitHub
  1. 注册账号：https://github.com/
  3. 下载git: https://git-scm.com/downloads
  4. 在GitHub账户中绑定ssh
  5. 学习git基本使用（自行上网查找）
  6. 学习markdown基本用法(使用于`README.md`)（自行上网查找）
  7. 克隆仓库`https://github.com/SJTU-VEX/24-Interview-1.git`, 将仓库上传至个人仓库（Public）,并且新建2个分支
        1. 分支1: 名称为姓名全拼, eg. `LiaoHuaizhi`
            2. 分支2: 名称为姓名全拼-dev, eg. `LiaoHuaizhi-dev`
### 2. VSCode

1. 自行配置C++编译环境

2. 安装VEX插件, 确保程序可以正常编译

3. C++语言规范`SJTUVEXCppStyleGuide.md`

## A卷(A/B二选一)

###### 底盘电机示意图

<img src="D:\VEX\24-Interview-1\img\底盘样图.jpg" style="zoom: 25%;" />

###### 定位轮示意图

<img src="D:\VEX\24-Interview-1\img\定位轮.jpg" style="zoom:50%;" />

1. **学习顺序**: `main.cpp(注释已标好)->robot-config.h/.cpp(注释已标好)->Chassis.h/.cpp->position.h/.cpp(不做要求)->usercontrol.h/.cpp`
2. 按照上述学习顺序，学习理解整体代码框架，在面试中进行功能解释。
  3. 全场定位的原理(可选，若选做此题，面试过程中依此进行提问)
  4. PID.cpp文件补充：PID.h文件已有，PID.cpp文件已有基本框架，在此基础上完善PID.cpp文件。
  5. 在robot-config.h/.cpp文件中定义一个电机，端口号为21，在userControl.cpp文件中写映射函数完成`按下A键，电机正转（or反转），按下B键，电机反转（or正转）`。
  6. readme.md第一行写上自己的名字。
  7. 完成上述任务，git上传所有代码到main分支，在本仓库创建issue，将个人仓库链接粘贴至issue下。

## B卷（A/B二选一）

stm32F103C8T6最小系统板，PCB绘制。

将工程文件转为立创EDA或Altium Designer工程文件，将整个工程压缩为`stm32Test.zip`并上传至自己的git仓库。

readme.md第一行写上自己的名字。

完成上述任务，git上传所有代码到main分支，在本仓库创建issue，将个人仓库链接粘贴至issue下。



## DDL

9月28号（周六）中午12点

## 参考资料（有任何问题QQ群中直接提问）
1. 普渡大学VEX官网: https://wiki.purduesigbots.com/
2. VEX 官方论坛: https://www.vexforum.com/
3. 官方函数库: https://api.vexcode.cloud/v5/

