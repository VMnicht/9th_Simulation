#include <rc_msgs/ChassisCmd.h>
#include <ros/ros.h>
#include <ros/time.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TwistStamped.h>
#include <geometry_msgs/Accel.h>



int main(int argc, char** argv){
    ros::init(argc, argv, "MATLAB_acc");
    ros::NodeHandle nh;
    ros::Publisher cmd_pub_r1 = nh.advertise<rc_msgs::ChassisCmd>("robot1/chassis_controller/cmd_chassis", 10);
    ros::Publisher cmd_pub_r2 = nh.advertise<rc_msgs::ChassisCmd>("robot2/chassis_controller/cmd_chassis", 10);
    ros::Publisher cmd_pub_r3 = nh.advertise<rc_msgs::ChassisCmd>("robot3/chassis_controller/cmd_chassis", 10);
    ros::Publisher cmd_pub_r4 = nh.advertise<rc_msgs::ChassisCmd>("robot4/chassis_controller/cmd_chassis", 10);
    rc_msgs::ChassisCmd cmd;
    rc_msgs::ChassisCmd cmd_r3;
    rc_msgs::ChassisCmd cmd_r4;
    ros::Subscriber sub_r3 = nh.subscribe<geometry_msgs::Accel>("MATLAB_acc_r3", 10, [&](const geometry_msgs::Accel::ConstPtr& msg) {
        cmd_r3.accel.linear.x = msg->linear.x;
        cmd_r3.accel.linear.y = msg->linear.y;
        cmd_r3.accel.angular.z = msg->angular.z;
    });

    ros::Subscriber sub_r4 = nh.subscribe<geometry_msgs::Accel>("MATLAB_acc_r4", 10, [&](const geometry_msgs::Accel::ConstPtr& msg) {
        cmd_r4.accel.linear.x = msg->linear.x;
        cmd_r4.accel.linear.y = msg->linear.y;
        cmd_r4.accel.angular.z = msg->angular.z;
    });
    ros::Rate rate(50);

    cmd.accel.linear.x = 1.5;
    cmd.accel.linear.y = 1.5;
    cmd.accel.angular.z = 1.5;
    while(ros::ok()){
        ros::spinOnce();
        cmd_pub_r1.publish(cmd);
        cmd_pub_r2.publish(cmd);
        cmd_pub_r3.publish(cmd_r3);
        cmd_pub_r4.publish(cmd_r4);
        ros::spinOnce();
        rate.sleep();
    }
}