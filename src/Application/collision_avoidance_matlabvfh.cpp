/***************************************************************************************************************************
*
* Author: bingo
* Email: 1554459957@qq.com
* Time: 2019.10.14
* Description: lidar collision v1.0
*  
***************************************************************************************************************************/

//ROS 头文件
#include <ros/ros.h>
#include <Eigen/Eigen>


//topic 头文件
#include <iostream>
#include <std_msgs/Bool.h>
#include <geometry_msgs/Pose.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/PositionTarget.h>
#include <std_msgs/Float64MultiArray.h>
using namespace std;
Eigen::Vector3d vel_sp; 
Eigen::Vector3d pos_drone;                                      //无人机当前位置 (来自fcu)    
Eigen::Vector3d desire_pos; //期望位置                                
void targetVel_cb(const std_msgs::Float64MultiArray::ConstPtr &msg)
{
  vel_sp[0] =  msg->data.at(0);
  vel_sp[1] =  msg->data.at(1);
  ROS_INFO("I heard: [%f],[%f]", msg->data.at(0),msg->data.at(1));
}
void pos_cb(const geometry_msgs::PoseStamped::ConstPtr &msg)
{
    // Read the Drone Position from the Mavros Package [Frame: ENU]
    Eigen::Vector3d pos_drone_fcu_enu(msg->pose.position.x,msg->pose.position.y,msg->pose.position.z);

    pos_drone = pos_drone_fcu_enu;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>主 函 数<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
int main(int argc, char **argv)
{
    ros::init(argc, argv, "collision_avoidance_matlabvfh");
    ros::NodeHandle nh("~");
    ros::Rate rate(20.0);
	desire_pos[0] = -2;
	desire_pos[1] = 0;
    desire_pos[2] = 1;
    //【订阅】Lidar数据
    ros::Subscriber lidar_sub = nh.subscribe<std_msgs::Float64MultiArray>("/cmd_vel", 100, targetVel_cb);
    ros::Subscriber position_sub = nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 100, pos_cb);

    ros::Publisher setpoint_raw_local_pub = nh.advertise<mavros_msgs::PositionTarget>("/mavros/setpoint_raw/local", 100);

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Main Loop<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    while(ros::ok())
    {

		ros::spinOnce();
        if(sqrt((pos_drone[0]-desire_pos[0])*(pos_drone[0]-desire_pos[0])+(pos_drone[1]-desire_pos[1])*(pos_drone[1]-desire_pos[1])) > 0.2)
		{
			mavros_msgs::PositionTarget pos_setpoint;
			pos_setpoint.type_mask = 1 + 2 + /*4 + 8 + 16 + 32 +*/ 64 + 128 + 256 + 512 + 1024 + 2048;
			pos_setpoint.coordinate_frame = 1;
			pos_setpoint.velocity.x = vel_sp[0];
			pos_setpoint.velocity.y = vel_sp[1];
			pos_setpoint.position.z = desire_pos[2];
			//  pos_setpoint.yaw = 1.5;
			setpoint_raw_local_pub.publish(pos_setpoint);

		}
		else
		{
			mavros_msgs::PositionTarget pos_setpoint;
			pos_setpoint.type_mask = /*1 + 2 + 4 +*/ 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048;
			pos_setpoint.coordinate_frame = 1;
			pos_setpoint.position.x = desire_pos[0];
			pos_setpoint.position.y = desire_pos[1];
			pos_setpoint.position.z = desire_pos[2];
			//  pos_setpoint.yaw = 1.5;
			setpoint_raw_local_pub.publish(pos_setpoint);

		}
		rate.sleep();
    }
    return 0;

}



