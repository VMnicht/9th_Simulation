#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>


int ctrl_robot = 1;
geometry_msgs::Twist joy_vel;
void JOY_Data_Callback(const sensor_msgs::Joy::ConstPtr& msg, const std::string& output_topic,ros::NodeHandle& nh) {
    static int last_buttonX = 0,last_buttonY [[maybe_unused]] = 0,last_buttonA = 0,
    last_buttonB [[maybe_unused]] = 0,last_buttonLB [[maybe_unused]] = 0,last_buttonRB [[maybe_unused]] = 0,speed=10;
    static ros::Time last_time_A(0,0),last_time_X(0,0);
    int buttonA = msg->buttons[0],buttonB = msg->buttons[1],buttonX = msg->buttons[2]
        ,buttonY [[maybe_unused]] = msg->buttons[3],buttonLB [[maybe_unused]] = msg->buttons[4],buttonRB [[maybe_unused]] = msg->buttons[5];
    joy_vel.linear.x = msg->axes[1]*speed;
    joy_vel.linear.y = msg->axes[0]*speed;
    joy_vel.angular.z = msg->axes[3]*speed;
    if(buttonA == 1 && last_buttonA == 0
        && (ros::Time::now()-last_time_A).toSec()>0.4){
        ctrl_robot ++;
        if(ctrl_robot>4) ctrl_robot = 1;
        printf("切换至机器人%d\n",ctrl_robot);
        last_time_A = ros::Time::now();
    }
    if(buttonB == 1){
        joy_vel.linear.x = 0;
        joy_vel.linear.y = 0;
        joy_vel.angular.z = 0;
        printf("机器人%d刹车！\n",ctrl_robot);
    }
    if(buttonX == 1 && last_buttonX == 0 
    && (ros::Time::now()-last_time_X).toSec()>0.4){
        switch (speed)
        {
        case 10:
            speed = 15;
            printf("切换至高速！\n");
            break;
        case 15:
            speed = 5;
            printf("切换至低速！\n");
            break;
        case 5:
            speed = 10;
            printf("切换至中速！\n");
            break;
        
        default:
            break;
        }
        last_time_X = ros::Time::now();
        
    }


}

int main(int argc, char **argv) {
    // 初始化ROS节点
    ros::init(argc, argv,"joy_ctrl");
    // 订阅输入的topic
    std::string input_topic = "joy";
    // 创建发布者到新的topic
    std::string output_topic = "cmd_vel";

    ros::NodeHandle nh;
    ros::Publisher pub1 = nh.advertise<geometry_msgs::Twist>("/robot1/"+output_topic, 10);
    ros::Publisher pub2 = nh.advertise<geometry_msgs::Twist>("/robot2/"+output_topic, 10);
    ros::Publisher pub3 = nh.advertise<geometry_msgs::Twist>("/robot3/"+output_topic, 10);
    ros::Publisher pub4 = nh.advertise<geometry_msgs::Twist>("/robot4/"+output_topic, 10);
    ros::Rate loop_rate(50);
    ros::Subscriber sub = nh.subscribe<sensor_msgs::Joy>(input_topic, 10, std::bind(JOY_Data_Callback, std::placeholders::_1, output_topic, nh));

    printf("手柄控制机器人： \n");
    printf("左摇杆 - 线速度控制 \n");
    printf("右摇杆 - 角速度控制 \n");
    printf("A按键 - 切换机器人 \n");
    printf("B按键 - 刹车 \n");
    printf("X按键 - 换档 \n");
    printf("当前操作机器人%d \n当前档位：中速\n",ctrl_robot);
    printf("------------- \n");
    while (ros::ok())
    {
        ros::spinOnce();
        switch (ctrl_robot){
            case 1:
                pub1.publish(joy_vel);
                break;
            case 2:
                pub2.publish(joy_vel);
                break;
            case 3:
                pub3.publish(joy_vel);
                break;
            case 4:
                pub4.publish(joy_vel);
                break;
            default:
                printf("机器人%d不存在！\n",ctrl_robot);
                break;
            }
            loop_rate.sleep();
    }

    return 0;
}