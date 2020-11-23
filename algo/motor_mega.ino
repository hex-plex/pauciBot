#include <PIDController.h>
#define EN_1 2
#define EN_2 3
#define PWM_1 8
#define MO_1 9
#define MO_2 10
volatile long int encoder_pos = 0;
PIDController pos_pid;
int motor_value = 255;
unsigned int integerValue=0;
char incomingByte;

void setup() {
  Serial.begin(9600);
  pinMode(EN_1,INPUT);
  pinMode(EN_2,INPUT);
  pinMode(MO_1,OUTPUT);
  pinMode(MO_2,OUTPUT);
  pinMode(PWM_1,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(EN_1),encoder,RISING);

  pos_pid.begin();
  pos_pid.tune(15,0,2000);
  pos_pid.limit(-255,255);
}

void loop() {
  if(Serial.available() > 0){
    integerValue = 0;
    while(1){
      incomingByte = Serial.read();
      int temp = ((int)incomingByte - 48);
      if(temp<=9 && temp>=0) {
        integerValue *= 10;
        integerValue += temp;
      }
      if(incomingByte == '\n')break;
      if(incomingByte == -1 ) continue;
        
    }  
    pos_pid.setpoint(integerValue);
  }
  motor_value = pos_pid.compute(encoder_pos);
  if(motor_value > 0){
    MotorCounterClockwise(motor_value);
  }else{
    MotorClockwise(abs(motor_value));
  }
  Serial.print(integerValue);
  Serial.print(' ');
  Serial.println(encoder_pos);
  delay(10);
}

void encoder(){
  if(digitalRead(EN_2) == HIGH){
    encoder_pos++;  
  }else{
    encoder_pos--;  
  }
}
void MotorClockwise(int power){
  if(power>75){
    analogWrite(PWM_1,power);
    digitalWrite(MO_1,HIGH);
    digitalWrite(MO_2,LOW);  
  }else{
    analogWrite(PWM_1,0);
    digitalWrite(MO_1, LOW);
    digitalWrite(MO_2, LOW);
  }  
}
void MotorCounterClockwise(int power){
  if(power>75){
    analogWrite(PWM_1,power);
    digitalWrite(MO_2,HIGH);
    digitalWrite(MO_1,LOW);  
  }else{
    analogWrite(PWM_1,0);
    digitalWrite(MO_1, LOW);
    digitalWrite(MO_2, LOW);
  }  
}
