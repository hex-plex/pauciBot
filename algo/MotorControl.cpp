#include "Arduino.h"
#include "MotorControl.h"

MotorControl::MotorControl(int EN_1, int EN_2, int MO_1,int MO_2, int PWM, PIDController *pid){
	this->_en_1 = EN_1;
	this->_en_2 = EN_2;
	this->_mo_1 = MO_1;
	this->_mo_2 = MO_2;
	this->_pwm = PWM;
	this->_pos_pid = pid;
}
MotorControl::MotorControl(int EN_1, int EN_2, int MO_1,int MO_2, int PWM){
	this->_en_1 = EN_1;
	this->_en_2 = EN_2;
	this->_mo_1 = MO_1;
	this->_mo_2 = MO_2;
	this->_pwm = PWM;
	PIDController pos_pid;
	pos_pid.begin();
	pos_pid.tune(15, 0 ,2000);
	pos_pid.limit(-255,255);
	this->_pos_pid = &pos_pid;
}
MotorControl::MotorControl(PIDController *pid){
	this->_en_1 = 2;
	this->_en_2 = 4;
	this->_mo_1 = 9;
	this->_mo_2 = 10;
	this->_pwm = 8;
	this->_pos_pid = pid;
}
MotorControl::MotorControl(bool a){
	if (a){
		this->_en_1 = 2;
		this->_en_2 = 4;
		this->_mo_1 = 9;
		this->_mo_2 = 10;
		this->_pwm = 8;
		PIDController pos_pid;
		pos_pid.begin();
		pos_pid.tune(15, 0 ,2000);
		pos_pid.limit(-255,255);
		this->_pos_pid = &pos_pid;
	} else {
		this->_en_1 = -1;
		this->_en_2 = -1;
		this->_mo_1 = -1;
		this->_mo_2 = -1;
		this->_pwm = -1;
		PIDController pos_pid;
		pos_pid.begin();
		pos_pid.tune(0,0,0);
		pos_pid.limit(-255,255);
		this->_pos_pid = &pos_pid; 
	}
}
void MotorControl::begin(void enc(void) ){
	pinMode(this->_en_1, INPUT);
	pinMode(this->_en_2, INPUT);
	pinMode(this->_mo_1, OUTPUT);
	pinMode(this->_mo_2, OUTPUT);
	pinMode(this->_pwm, OUTPUT);
	attachInterrupt(digitalPinToInterrupt(this->_en_1),enc,RISING);
	this->_curr_set = -1379;
}
void MotorControl::encoder(){
	if(digitalRead(this->_en_2) == HIGH){
		this->encoder_pos++;
	} else {
		this->encoder_pos--;
	}
}
int MotorControl::control(int setpoint){
	if(this->_curr_set!=setpoint){
		this->_pos_pid->setpoint(setpoint);
		this->_curr_set = setpoint;
	}
	
	int power = this->_pos_pid->compute(this->encoder_pos);
	if(abs(power)>this->thresh){
		analogWrite(this->_pwm, abs(power));
		if(power>0){
			digitalWrite(this->_mo_1, HIGH);
			digitalWrite(this->_mo_2, LOW);
		} else {
			digitalWrite(this->_mo_2, HIGH);
			digitalWrite(this->_mo_1, LOW);
		}

	} else {
		power = 0;
		analogWrite(this->_pwm, 0);
		digitalWrite(this->_mo_1, LOW);
		digitalWrite(this->_mo_2, LOW);
	}
	return power;
}
