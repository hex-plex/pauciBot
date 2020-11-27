#include "MotorControl.h"
MotorControl* motors[2];
int setpoints[2]={0,0};
motors[0] = new MotorControl(2, 4, 9, 10, 8);
motors[1] = new MotorControl(3, 5, 12, 11, 13);
int turn = 0;

void setup(){
  Serial.begin(9600);
  for(int i=0;i<2;i++)
    motors[i]->begin(motors[i]->encoder());
}
void loop(){
  for(int i = 0;i<2;i++){
    motors[i]->control(setpoints[i]);
  }
}
