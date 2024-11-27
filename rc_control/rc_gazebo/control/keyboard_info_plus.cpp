#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <thread>
#include "ncurses.h"

// 定义全局变量以在多线程环境中共享
bool keepRunning = true;
std::string keyPressed;

// 函数用于设置终端为非阻塞模式
void set_terminal_mode(bool enable) {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    if (enable) {
        // 设置非规范模式
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    } else {
        // 恢复规范模式
        term.c_lflag |= (ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
        fcntl(STDIN_FILENO, F_SETFL, 0);
    }
}

void ncursesInputThread() {
    // 初始化ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // 使getch()非阻塞

    int ch;
    while (keepRunning) {
        ch = getch(); // 非阻塞读取按键
        if (ch != ERR) { // 如果有按键被按下
            keyPressed = std::string(1, (char)ch);
            ROS_INFO("Key pressed: %s", keyPressed.c_str());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 稍微休息一下，避免CPU过热
    }

    endwin(); // 结束ncurses模式
}

// 函数用于检查按键状态
std::vector<char> get_pressed_keys() {
    std::vector<char> keys;
    char c;
    while (read(STDIN_FILENO, &c, 1) > 0) {
        keys.push_back(c);
    }
    return keys;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "keyboard_info_plus");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::String>("keyboard_info", 10);
    //std_msgs::String msg;
    ros::Rate loop_rate(1000);

    std::thread inputThread(ncursesInputThread);
    // 设置终端为非阻塞模式
    //set_terminal_mode(true);

    //ROS_INFO("Press keys (press x to quit)");

    while (ros::ok()) {
        std_msgs::String msg;
        msg.data = keyPressed; // 使用共享变量
        keyPressed.clear(); // 清空共享变量
        pub.publish(msg); // 发布按键消息
        ros::spinOnce();
        loop_rate.sleep();
    }

    // 恢复终端设置
    set_terminal_mode(false);
    keepRunning = false; // 告诉输入线程停止
    inputThread.join(); // 等待输入线程结束
    return 0;
}
