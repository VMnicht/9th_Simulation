# rc25 gazebo & rviz & ros_controller
本项目使用 catkin build 构建
这里是3个包，请先在你的工作空间目录下创建src目录
然后cd src/
git clone 本项目
然后使用 rosdepc install --from-paths . --ignore-src
来自动完成对本项目的依赖下载


# package introduction
**rc_control**: 是基本的机器人组成文件
**rc_controller**: 是控制器文件
**rc_description**: 主要是存放了机器人的urdf文件

在开始之前，先添加环境变量
```shell
sudo vim ~/.bashrc
```
然后在文件末尾添加
```shell
export ROBOT_TYPE_1=rc_r2
export ROBOT_TYPE_2=rabbit
export ROBOT_TYPE_3=rc_r2
export ROBOT_TYPE_4=rabbit

```
退出，回到工作空间下，先刷新一下
```shell
source ~/.bashrc
```
然后再做下面的操作

```shell
cd src/
git clone git@github.com:KetenBieber/rc25_description.git
cd ..
rosdepc install --from-paths . --ignore-src
catkin build
mon launch rc_gazebo sim_launch.launch
mon launch odom_sensor test_plugin.launch

```

# 2024 11.27 add
添加了 air_joy_control move_control odom_sensor tf_broadcast 四个新包
rc_path的编写方式将会以Nodelet的结构进行编程，好处是动态加载，而且同进程内的数据传输将会比不同进程间数据传输更加省资源，并且会减少时间的消耗

## air_joy_control 
这个包的更新顺带着对rc_ibus包的更新,将rc_ibus修改为通用类，后续需要增加设备只需要在launch文件中显式指定参数即可
目前可以实现：单遥控下对仿真环境中四台车的控制（通过拨杆切换控制对象）
            多遥控下对仿真环境中四台车的分别控制
            并且均支持世界坐标系速度和机器人坐标系速度的分别控制

## move_control
这个包将会接入move_base，目前仅有四台车的底盘控制器启动plugin
## tf_broadcast
这个包会从action仿真控制器读取坐标数据，发布从action装配位置向地图坐标系odom的TF变换
## odom_sensor
这个包将会通过位置微分来得到机器人底盘速度，填补action控制器不能给出速度以及底盘关节控制器难以获取准确关节速度的缺口
并将其数据打包为odom标准类型发布，以供以后接入move_base框架
# 自动防守仿真  
使用虚拟机进行仿真，如果是实体机需要在实体机上安装MATLAB  
虚拟机网络选择桥接模式  
## 虚拟机内添加ROS_IP  
打开控制台，输入
```shell
ifconfig
```
找到信息中最上面的192.168开头的IP地址并复制  
打开.bashrc文件，在最后加上  
```shell
#后面是刚刚你复制的IP
ROS_IP=192.168.xx.xxx 
```
## 主机内添加ROS_IP及ROS_MASTER地址
打开自动防守的MATLAB文件夹内的ROS_init.m文件  
在win的cmd中输入ipconfig 
```shell
ipconfig 
```
从信息中复制'以太网适配器 以太网:'下的IPv4 地址填到ROS_IP后  
在虚拟机端启动roscore，从弹出的信息中找到ROS_MASTER_URI地址填入文件中   
## 启动仿真
在虚拟机内输入  
```shell
mon launch rc_gazebo sim_launch.launch
mon launch odom_sensor test_plugin.launch
```
主机MATLAB端运行run_it.m文件  
启动手柄控制  
```shell
roslaunch rc_gazebo joy_contrl.launch
```
如果仿真有问题，重启所有节点即可  
自动控制的是r3,r4即蓝方机器人  
手动控制的为r2,手柄中速挡为2m/s，低速为1m/s，高速为3m/s