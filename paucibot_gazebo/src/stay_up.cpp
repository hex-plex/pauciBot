#include <ros/ros.h>
#include <ros/console.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/JointState.h>
#include <tf/tf.h>
#include <geometry_msgs/Quaternion.h>
#include <std_msgs/Float64.h>

#include <dynamic_reconfigure/server.h>
#include <paucibot_config/ControllerTuneConfig.h>


double PRY[3] = {0.0, 0.0, 0.0};
double SETPOINT[3] = {0.0, 0.0, 0.0};
double PID[3] = {75.0, 10.0, 100.0};
double ENCODER[2] = {0.0, 0.0};

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////  Depreciated use Paucibot Controller Class  ///////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////


void controllerTuner(paucibot_config::ControllerTuneConfig &config, uint32_t level){
    PID[0] = config.P;
    PID[1] = config.I;
    PID[2] = config.D;
}

void update_imu(const sensor_msgs::Imu::ConstPtr& msg){
    geometry_msgs::Quaternion de = msg->orientation;
    tf::Quaternion q(de.x, de.y, de.z, de.w);
    //ROS_INFO("IMU : %f %f %f %f ", de.x, de.y, de.z, de.w);
    tf::Matrix3x3(q).getRPY(PRY[0],PRY[1],PRY[2]);
}

void update_encoder(const sensor_msgs::JointState::ConstPtr& msg){
    ENCODER[0] = msg->position[0];
    ENCODER[1] = msg->position[1];
}

int main(int argc, char **argv){
    ros::init(argc, argv, "Controller_node");

    ros::NodeHandle nh;

    dynamic_reconfigure::Server<paucibot_config::ControllerTuneConfig> server;
    dynamic_reconfigure::Server<paucibot_config::ControllerTuneConfig>::CallbackType f;

    f = boost::bind(&controllerTuner, _1, _2);
    server.setCallback(f);

    ros::Subscriber sub = nh.subscribe("/imuReading", 1, update_imu);
    ros::Subscriber sub2 = nh.subscribe("/paucibot/joint_states", 1000, update_encoder);

    ros::Publisher left_wheel = nh.advertise<std_msgs::Float64>("/paucibot/wheel1_position_controller/command",1000);
    ros::Publisher right_wheel = nh.advertise<std_msgs::Float64>("/paucibot/wheel2_position_controller/command",1000);

    double err = 0.0;
    double prev_err = 0.0;
    double init_err = 0.0;

    ros::Rate t(200);

    while(ros::ok()){
        err = PRY[1] - SETPOINT[1];
        //ROS_INFO("%f %f %f" , err, PRY[1], SETPOINT[1]);
        std_msgs::Float64 val;

        if(err<0.05)init_err += err;
        else init_err = 0;

        double feedback = PID[0]*err + PID[1]*(init_err) + PID[2]*(err-prev_err)*200;

        val.data = ENCODER[0] + feedback;
        left_wheel.publish(val);
        val.data = ENCODER[1] + feedback;
        right_wheel.publish(val);
        ros::spinOnce();
        t.sleep();
    }

    return 0;

}