#include <ros/ros.h>
#include <std_msgs/String.h>
#include <vector>
#include <string>
#include <rc_msgs/SimData.h>
#include <rc_msgs/ActionData.h>
#include <sensor_msgs/JointState.h>

double last_x=0.0,last_y=0.0;
ros::Time last_time(0,0);
// 定义回调函数模板
void Sim_Action_Data_Callback(const rc_msgs::ActionData::ConstPtr& msg, const std::string& output_topic,ros::NodeHandle& nh) {
    // 在回调函数中接收到的消息发布到创建的topic
    static ros::Publisher pub = nh.advertise<rc_msgs::SimData>(output_topic, 10);
    rc_msgs::SimData action_msg;
    ros::Time now_time = ros::Time::now();
    if(!last_time.isZero()){
        double dt = (now_time-last_time).toSec();
        action_msg.yaw_angle = msg->yaw_angle;
        action_msg.pose_x = msg->pose_x;
        action_msg.pose_y = msg->pose_y;
        action_msg.yaw_acc = msg->yaw_acc;
        action_msg.vx = (action_msg.pose_x-last_x)/dt;
        action_msg.vy = (action_msg.pose_y-last_y)/dt;
        pub.publish(action_msg);
    }
    last_time = now_time;
    last_x = action_msg.pose_x;
    last_y = action_msg.pose_y;
}

int main(int argc, char **argv) {
    // 初始化ROS节点
    ros::init(argc, argv,"sim_action_data");
    // 订阅输入的topic
    std::string input_topic = "location_action_odom";
    // 创建发布者到新的topic
    std::string output_topic = "sim_action_data";

    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe<rc_msgs::ActionData>(input_topic, 10, std::bind(Sim_Action_Data_Callback, std::placeholders::_1, output_topic, nh));

    // 进入ROS的事件循环
    ros::spin();

    return 0;
}
