#include "MotorControl.h"

MotorControl motors[2]={MotorControl(2, 4, 9, 10, 8), MotorControl(3, 5, 12, 11, 13) };
int encoder_pos[2] = {0,0};
int setpoints[2]={100,100};

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
  //motors[0] = (new MotorControl(2, 4, 9, 10, 8));
  //motors[1] = (new MotorControl(3, 5, 12, 11, 13));
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(motors[0]._en_1),enc_0,RISING);
  attachInterrupt(digitalPinToInterrupt(motors[1]._en_1),enc_1,RISING);
  for(int i=0;i<2;i++){
    motors[i].begin(&encoder_pos[i]);
  }
}
void loop(){
  for(int i = 0;i<2;i++){
    Serial.print(motors[i].control(setpoints[i]));
    Serial.print("-");
    Serial.print(*motors[i].encoder_pos);
    Serial.print(' ');
  }
  delay(100);
  Serial.print('\n');
  
}
