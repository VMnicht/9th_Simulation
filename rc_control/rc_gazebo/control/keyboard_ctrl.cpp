#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <stdio.h>
#include <termios.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>

void keyboard_callback(const std_msgs::String::ConstPtr& msg);
geometry_msgs::Twist base_cmd;
static float linear_vel = 0.1;
static float angular_vel = 0.1;
static int k_vel = 30;
int ctrl_robot = 1;
int exit_flag = 0;
int loop_cntx = 0,loop_cnty = 0,loop_cntw = 0;
int main(int argc, char** argv)
{
  ros::init(argc, argv, "keyboard_ctrl");

  printf("键盘控制机器人： \n");
  printf("w - 向前加速 \n");
  printf("s - 向后加速 \n");
  printf("a - 向左加速 \n");
  printf("d - 向右加速 \n");
  printf("q - 左旋加速 \n");
  printf("e - 右旋加速 \n");
  printf("空格 - 刹车 \n");
  printf("x - 退出 \n");
  printf("数字键切换机器人 \n");
  printf("当前操作机器人%d \n",ctrl_robot);
  printf("------------- \n");

  ros::NodeHandle n;
  ros::Publisher cmd1_vel_pub = n.advertise<geometry_msgs::Twist>("robot1/cmd_vel", 10);
  ros::Publisher cmd2_vel_pub = n.advertise<geometry_msgs::Twist>("robot2/cmd_vel", 10);
  ros::Publisher cmd3_vel_pub = n.advertise<geometry_msgs::Twist>("robot3/cmd_vel", 10);
  ros::Publisher cmd4_vel_pub = n.advertise<geometry_msgs::Twist>("robot4/cmd_vel", 10);
  ros::Subscriber sub = n.subscribe("keyboard_info", 10, keyboard_callback);
  ros::Rate loop_rate(1000);

  base_cmd.linear.x = 0;
  base_cmd.linear.y = 0;
  base_cmd.angular.z = 0;

  while(n.ok())
  {
    ros::spinOnce(); // 检查并调用回调函数
    // 在这里可以执行其他任务
    loop_cntx++;
    loop_cnty++;
    loop_cntw++;
    if(loop_cntx>=80000){
      base_cmd.linear.x = 0;
      loop_cntx = 0;
    }
    if(loop_cnty>=80000){
      base_cmd.linear.y = 0;
      loop_cnty = 0;
    }
    if(loop_cntw>=80000){
      base_cmd.angular.z = 0;
      loop_cntw = 0;
    }
    switch (ctrl_robot)
    {
      case 1:
        cmd1_vel_pub.publish(base_cmd);
        break;
      case 2:
        cmd2_vel_pub.publish(base_cmd);
        break;
      case 3:
        cmd3_vel_pub.publish(base_cmd);
        break;
      case 4:
        cmd4_vel_pub.publish(base_cmd);
        break;
      default:
        break;
    }
    if(exit_flag==1){
      base_cmd.linear.x = 0;
      base_cmd.linear.y = 0;
      base_cmd.angular.z = 0;
      cmd1_vel_pub.publish(base_cmd);
      cmd2_vel_pub.publish(base_cmd);
      cmd3_vel_pub.publish(base_cmd);
      cmd4_vel_pub.publish(base_cmd);
      return 0;
    }
    loop_rate.sleep(); // 等待直到下一个循环迭代
  }

  return 0;
}


// void keyboard_callback(const std_msgs::Int32::ConstPtr& msg){
void keyboard_callback(const std_msgs::String::ConstPtr& msg){
  std::string data = msg->data;
  for (char cKey : data)
  {
    if(cKey=='w')
    {
      loop_cntx = 0;
      base_cmd.linear.x += linear_vel;
      if(base_cmd.linear.x > linear_vel*k_vel)
        base_cmd.linear.x = linear_vel*k_vel;
      printf(" - linear.x= %.2f linear.y= %.2f angular.z= %.2f \n",base_cmd.linear.x,base_cmd.linear.y,base_cmd.angular.z);
    } 
    else if(cKey=='s')
    {
      loop_cntx = 0;
      base_cmd.linear.x += -linear_vel;
      if(base_cmd.linear.x < -linear_vel*k_vel)
        base_cmd.linear.x = -linear_vel*k_vel;
      printf(" - linear.x= %.2f linear.y= %.2f angular.z= %.2f \n",base_cmd.linear.x,base_cmd.linear.y,base_cmd.angular.z);
    } 
    else if(cKey=='a')
    {
      loop_cnty = 0;
      base_cmd.linear.y += linear_vel;
      if(base_cmd.linear.y > linear_vel*k_vel)
        base_cmd.linear.y = linear_vel*k_vel;
      printf(" - linear.x= %.2f linear.y= %.2f angular.z= %.2f \n",base_cmd.linear.x,base_cmd.linear.y,base_cmd.angular.z);
    }
    else if(cKey=='d')
    {
      loop_cnty = 0;
      base_cmd.linear.y += -linear_vel;
      if(base_cmd.linear.y < -linear_vel*k_vel)
        base_cmd.linear.y = -linear_vel*k_vel;
      printf(" - linear.x= %.2f linear.y= %.2f angular.z= %.2f \n",base_cmd.linear.x,base_cmd.linear.y,base_cmd.angular.z);
    } 
    else if(cKey=='q')
    {
      loop_cntw = 0;
      base_cmd.angular.z += angular_vel;
      if(base_cmd.angular.z > angular_vel*k_vel)
        base_cmd.angular.z = angular_vel*k_vel;
      printf(" - linear.x= %.2f linear.y= %.2f angular.z= %.2f \n",base_cmd.linear.x,base_cmd.linear.y,base_cmd.angular.z);
    } 
    else if(cKey=='e')
    {
      loop_cntw = 0;
      base_cmd.angular.z += -angular_vel;
      if(base_cmd.angular.z < -angular_vel*k_vel)
        base_cmd.angular.z = -angular_vel*k_vel;
      printf(" - linear.x= %.2f linear.y= %.2f angular.z= %.2f \n",base_cmd.linear.x,base_cmd.linear.y,base_cmd.angular.z);
    } 
    else if(cKey==' ')
    {
      loop_cntx = 0;
      loop_cnty = 0;
      loop_cntw = 0;
      base_cmd.linear.x = 0;
      base_cmd.linear.y = 0;
      base_cmd.angular.z = 0;
      printf(" - linear.x= %.2f linear.y= %.2f angular.z= %.2f \n",base_cmd.linear.x,base_cmd.linear.y,base_cmd.angular.z);
    } 
    else if(cKey=='x')
    {
      exit_flag = 1;
      printf("退出！ \n");
    } 
    else if(cKey=='1' || cKey=='2'
         || cKey=='3' || cKey=='4')
    {
      ctrl_robot=cKey-'0';
      printf("当前操作机器人%d \n",ctrl_robot);
    }
    else
    {
       printf(" - 未定义指令%d \n",cKey);
    }
  }
}