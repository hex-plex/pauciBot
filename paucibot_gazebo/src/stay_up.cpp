#include <ros/ros.h>
#include <ros/console.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/JointState.h>
#include <tf/tf.h>
#include <geometry_msgs/Quaternion.h>
#include <std_msgs/Float64.h>
double pry[3] = {0.0, 0.0, 0.0};
double setpoint[3] = {0.0, 0.0, 0.0};

double encoder[2] = {0.0, 0.0};

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////  Depreciated use Paucibot Controller Class  ///////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


void update_imu(const sensor_msgs::Imu::ConstPtr& msg){
    geometry_msgs::Quaternion de = msg->orientation;
    tf::Quaternion q(de.x, de.y, de.z, de.w);
    ROS_INFO("IMU : %f %f %f %f ", de.x, de.y, de.z, de.w);
    tf::Matrix3x3(q).getRPY(pry[0],pry[1],pry[2]);
}

void update_encoder(const sensor_msgs::JointState::ConstPtr& msg){
    encoder[0] = msg->position[0];
    encoder[1] = msg->position[1];
}

int main(int argc, char **argv){
    ros::init(argc, argv, "Controller_node");

    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("imuReading", 1000, update_imu);
    ros::Subscriber sub2 = nh.subscribe("/paucibot/joint_states", 1000, update_encoder);

    ros::Publisher left_wheel = nh.advertise<std_msgs::Float64>("/paucibot/wheel1_position_controller/command",1000);
    ros::Publisher right_wheel = nh.advertise<std_msgs::Float64>("/paucibot/wheel2_position_controller/command",1000);

    double err = 0.0;

    ros::Rate t(100);

    while(ros::ok()){
        err = pry[1] - setpoint[1];
        ROS_INFO("%f %f %f" , err, pry[1], setpoint[1]);
        std_msgs::Float64 val;
        val.data = encoder[0] + err;
        left_wheel.publish(val);
        val.data = encoder[1] + err;
        right_wheel.publish(val);
    
        t.sleep();
    }

    return 0;

}