# rc25 gazebo & rviz & ros_controller
本项目使用 catkin build 构建
基于林科潭的环境
这里是3个包，请先在你的工作空间目录下创建src目录
然后cd src/
git clone 本项目
然后使用 rosdepc install --from-paths . --ignore-src
来自动完成对本项目的依赖下载


# package introduction
rc_control 是基本的机器人组成文件
rc_controller 是控制器文件
rc_description 主要是存放了机器人的urdf文件

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
rosdepc install --from-paths . --ignore-src
catkin build
mon launch rc_gazebo sim_launch.launch
mon launch rc_gazebo load_tf_tree.launch
mon launch rc_gazebo load_basketball_field_map.launch

```
需要的数据包在每个机器人命名空间下的sim_action_data话题下。  
发布的速度为世界坐标系下的速度。  
控制速度的话题是每个机器人命名空间下的cmd_vel,为机器人坐标系下的速度。  
发布速度时记得对着包里的角度信息变换一下。  
自定义了一个包SimData,在rc_msg下。  
