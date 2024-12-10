#include <rc_msgs/ChassisCmd.h>
#include <ros/ros.h>
#include <ros/time.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TwistStamped.h>

int main(int argc, char** argv){
    ros::init(argc, argv, "acc_test");
    ros::NodeHandle nh;
    ros::Publisher cmd_pub_r1 = nh.advertise<rc_msgs::ChassisCmd>("robot1/chassis_controller/cmd_chassis", 10);
    ros::Publisher cmd_pub_r2 = nh.advertise<rc_msgs::ChassisCmd>("robot2/chassis_controller/cmd_chassis", 10);
    ros::Publisher cmd_pub_r3 = nh.advertise<rc_msgs::ChassisCmd>("robot3/chassis_controller/cmd_chassis", 10);
    ros::Publisher cmd_pub_r4 = nh.advertise<rc_msgs::ChassisCmd>("robot4/chassis_controller/cmd_chassis", 10);
    ros::Rate rate(50);
    rc_msgs::ChassisCmd cmd;
    cmd.accel.linear.x = 1.5;
    cmd.accel.linear.y = 1.5;
    cmd.accel.angular.z = 2;

    while(ros::ok()){
        cmd_pub_r1.publish(cmd);
        cmd_pub_r2.publish(cmd);
        cmd_pub_r3.publish(cmd);
        cmd_pub_r4.publish(cmd);
        rate.sleep();
    }

    return 0;
}
