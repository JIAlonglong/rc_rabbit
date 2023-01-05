//
// Created by jialong on 23/1/3.
//
#include <ros/ros.h>
#include "rc_msgs/ChassisCmd.h"
#include "rc_msgs/IbusData.h"
#include <geometry_msgs/Twist.h>
#include "Communication_ROS.h"

int main(int argc, char** argv)
{
    float shooter_base_state=0.0;
    float gimal_state=0.0;
    float trans_state=0.0;
    float shoot=0.0;
    ros::init(argc, argv, "rabbit_middleware");
    ros::NodeHandle nh;
    ros::NodeHandle nh1;
    ros::Publisher chassis_config;
    chassis_config = nh.advertise<rc_msgs::ChassisCmd>("/controllers/chassis_controller/command", 50);
    ros::Publisher vel_config;
    vel_config = nh1.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    
    rc_msgs::ChassisCmd chassis_msg;
    chassis_msg.accel.linear.x=1.0;
    chassis_msg.accel.linear.y=1.0;
    chassis_msg.accel.angular.z=4.0;

    geometry_msgs::Twist vel_msg;
    rc_msgs::IbusData Ibus;
    switch (Ibus.sw_a)
    {
    case 1:
        vel_msg.linear.x=Ibus.ch_l_y;
        vel_msg.linear.y=Ibus.ch_l_x;
        vel_msg.angular.z=Ibus.ch_r_y;
        break;

    case 2://auto_move
        break;    
    }

    switch (Ibus.sw_b)
    {
    case 1://close
        shooter_base_state=0.0;
        break;
    
    case 2://open
        shooter_base_state=1.0;
        break;

    case 3://open
        shooter_base_state=1.0;
        break;    
    }

    switch (Ibus.sw_c)
    {
    case 1:
        gimal_state=0.0;
        break;
    
    case 2://type:II
        gimal_state=2.0;
        break;

    case 3:// type:I
        gimal_state=1.0;
        break;    
    }

    switch (Ibus.sw_d)
    {
    case 1://tran_stop
        trans_state=1.0;
        break;

    case 2://tran_start
        trans_state=2.0;
        break;    
    }

    if (Ibus.key_l)
    {
        /* code */
    }

    if (Ibus.key_r)
    {
        /* code */
    }
    
    
    ros::Rate loop_rate(10);
    while (ros::ok())
    {
        chassis_config.publish(chassis_msg);
        vel_config.publish(vel_msg);
        ROS_WRITE_TO_STM32(shooter_base_state,gimal_state,trans_state,1.0,1.0,1.0,1.0,1.0,1.0,1.0);
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
}
