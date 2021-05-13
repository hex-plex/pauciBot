#include "MPU9250.h"
#include <ros.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>
#include <std_msgs/String.h>
#include <Wire.h>
MPU9250* IMU;
int staus;

ros::NodeHandle nh;

sensor_msgs::Imu msg_dat;
sensor_msgs::MagneticField mag_dat;
ros::Publisher raw("imu/data_raw",&msg_dat);
ros::Publisher maga("imu/mag",&mag_dat);
void setup() {
  //Wire.begin();
  nh.initNode();
  IMU = new MPU9250(Wire,0x68);
  while(!nh.connected()){
      nh.spinOnce();
    }
  staus = IMU->begin();
  char buff[7];
  if(staus<0){
      nh.logfatal("IMU initialization unsuccessful");
      nh.logerror("Check IMU wiring or try cycling power");
      itoa(staus,buff,10);
      nh.loginfo(strcat("Status: ",buff));
      while(1){nh.spinOnce();delay(1000);}  // Nodemcu doesnt like empty while loops
    }
  nh.advertise(raw);
  nh.advertise(maga);
  msg_dat.header.frame_id="/case_link";
  mag_dat.header.frame_id="/chase_link";
  msg_dat.orientation.x=0;msg_dat.orientation.y=0;msg_dat.orientation.z=0;msg_dat.orientation.w=0;
  for(int i=0;i<9;i++)msg_dat.orientation_covariance[i] =-1;
  for(int i=0;i<9;i++)msg_dat.angular_velocity_covariance[i]=0;
  for(int i=0;i<9;i++)msg_dat.linear_acceleration_covariance[i] = 0;
  for(int i=0;i<9;i++)mag_dat.magnetic_field_covariance[i] = 0;
}

void loop() {
  if(true){
    IMU->readSensor(); 
    msg_dat.angular_velocity.x = IMU->getGyroX_rads();
    msg_dat.angular_velocity.y = IMU->getGyroY_rads();
    msg_dat.angular_velocity.z = IMU->getGyroZ_rads();
    msg_dat.linear_acceleration.x = IMU->getAccelX_mss();
    msg_dat.linear_acceleration.y = IMU->getAccelY_mss();
    msg_dat.linear_acceleration.z = IMU->getAccelZ_mss();
    //Serial.print(IMU->getMagX_uT());
    //Serial.print("\n");
    mag_dat.magnetic_field.x = IMU->getMagX_uT();
    mag_dat.magnetic_field.y = IMU->getMagY_uT();
    mag_dat.magnetic_field.z = IMU->getMagZ_uT();
    mag_dat.header.stamp = nh.now();
    msg_dat.header.stamp = nh.now();
    maga.publish(&mag_dat);
    raw.publish(&msg_dat);
  }else{
    //Serial.println(".");
    delay(100);  
  }
  nh.spinOnce();
  delay(50);
}
