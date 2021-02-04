#include "MotorControl.h"
#include "MPU9250.h"
#include <Wire.h>


MPU9250 IMU(Wire,0x68);
int status=-1;
MotorControl motors[2]={MotorControl(2, 4, 9, 10, 8), MotorControl(3, 5, 12, 11, 13) };
int encoder_pos[2] = {0,0};
int setpoints[2]={0,0};
float p_r_y[3] = {0,0,0};
float p_r_y_set[3] = {0,0,0};

void pry_callback(MPU9250* imu,float p_r_y[3]){
  imu->readSensor();
  float accelX = imu->getAccelX_mss();
  float accelY = imu->getAccelY_mss();
  float accelZ = imu->getAccelZ_mss();
  float gyroX = imu->getGyroX_rads();
  float gyroY = imu->getGyroY_rads();
  float gyroZ = imu->getGyroZ_rads();
  float magX = imu->getMagX_uT();;
  float magY = imu->getMagY_uT();
  float magZ = imu->getMagZ_uT();;
  p_r_y[0] = atan2( accelY, ( sqrt( (accelX*accelX) + (accelZ*accelZ) ) ) );
  p_r_y[1] = atan2( -accelX, ( sqrt( (accelY*accelY) + (accelZ*accelZ) ) ) );
  float Yh = ( magY * cos(p_r_y[1]) ) - ( magZ * sin(p_r_y[1]) );
  float Xh = ( magX * cos(p_r_y[0]) ) + ( magY * sin(p_r_y[1]) * sin(p_r_y[0]) ) + ( magZ * cos(p_r_y[1]) * sin(p_r_y[0]) );
  p_r_y[2] = atan2(Yh, Xh);
}

void diff_compute(float p_r_y_setpoint[3], float p_r_y_current[3],int motor_setpoint[2]){
  // Implementing simple roll
  float error = (p_r_y_setpoint[1]-p_r_y_current[1]);
  motor_setpoint[0] -= 50*error;
  motor_setpoint[1] += 50*error;
}

void serial_comm(){
  // This is specifiacally for the py-serial code to display ros
  // This is done over ros nodehandle as ros nodehandle gives a lot of bugs
  // and errors working with chinese boards and interfere with the SPI communication.
  // Serial communication is rather very slow and slows down the whole process of commutation as it waits for the handshake.
  Serial.println("");
}

void enc_0(){
  if(digitalRead(motors[0]._en_2)==HIGH){
    encoder_pos[0]++;  
  } else {
    encoder_pos[0]--;  
  }
}
void enc_1(){
  if(digitalRead(motors[1]._en_2)==HIGH){
    encoder_pos[1]++;  
  } else {
    encoder_pos[1]--;  
  }
}

int turn = 0;

void setup(){
  Serial.begin(9600);
  //IMU = new MPU9250(Wire,0x68);
  status = IMU.begin();
  if(status<0){
    Serial.print("Failed");
  }
  
  attachInterrupt(digitalPinToInterrupt(motors[0]._en_1),enc_0,RISING);
  attachInterrupt(digitalPinToInterrupt(motors[1]._en_1),enc_1,RISING);
  for(int i=0;i<2;i++){
    motors[i].begin(&encoder_pos[i]);
  }
}
void loop(){
  
  //for(int i = 0;i<2;i++){
    //Serial.print(motors[i].control(setpoints[i]));
    //Serial.print("-");
    //Serial.print(*motors[i].encoder_pos);
    //Serial.print(' ');
  //}
  // pry_callback(IMU,p_r_y);
  /*IMU.readSensor();
  float accelX = IMU.getAccelX_mss();
  float accelY = IMU.getAccelY_mss();
  float accelZ = IMU.getAccelZ_mss();
  float gyroX = IMU.getGyroX_rads();
  float gyroY = IMU.getGyroY_rads();
  float gyroZ = IMU.getGyroZ_rads();
  float magX = IMU.getMagX_uT();
  float magY = IMU.getMagY_uT();
  float magZ = IMU.getMagZ_uT();
  p_r_y[0] = atan2( accelY, ( sqrt( (accelX*accelX) + (accelZ*accelZ) ) ) );
  p_r_y[1] = atan2( -accelX, ( sqrt( (accelY*accelY) + (accelZ*accelZ) ) ) );
  float Yh = ( magY * cos(p_r_y[1]) ) - ( magZ * sin(p_r_y[1]) );
  float Xh = ( magX * cos(p_r_y[0]) ) + ( magY * sin(p_r_y[1]) * sin(p_r_y[0]) ) + ( magZ * cos(p_r_y[1]) * sin(p_r_y[0]) );
  p_r_y[2] = atan2(Yh, Xh);
  */
  pry_callback(&IMU,p_r_y);
  diff_compute(p_r_y_set, p_r_y, setpoints);
  for(int i = 0;i<2;i++){
    motors[i].control(setpoints[i]);
    //Serial.print("-");
    //Serial.print(*motors[i].encoder_pos);
    //Serial.print(' ');
  }
  /*Serial.print(" p:");
  Serial.print(p_r_y[0]);
  Serial.print(" r:");
  Serial.print(p_r_y[1]);
  Serial.print(" y:");
  Serial.print(p_r_y[2]);
  delay(100);
  Serial.print('\n');*/
  delay(25);
  
}
