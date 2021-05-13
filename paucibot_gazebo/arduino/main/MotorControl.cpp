#include "Arduino.h"
#include "MotorControl.h"

MotorControl::MotorControl(int EN_1, int EN_2, int MO_1,int MO_2, int PWM, PIDController *pid){
	this->_en_1 = EN_1;
	this->_en_2 = EN_2;
	this->_mo_1 = MO_1;
	this->_mo_2 = MO_2;
	this->_pwm = PWM;
	this->_pos_pid = *pid;
}
MotorControl::MotorControl(int EN_1, int EN_2, int MO_1,int MO_2, int PWM){
	this->_en_1 = EN_1;
	this->_en_2 = EN_2;
	this->_mo_1 = MO_1;
	this->_mo_2 = MO_2;
	this->_pwm = PWM;
	PIDController pos_pid;
	pos_pid.begin();
	pos_pid.tune(100, 0 ,3000);
	pos_pid.limit(-255,255);
	this->_pos_pid = pos_pid;
}
MotorControl::MotorControl(PIDController *pid){
	this->_en_1 = 2;
	this->_en_2 = 4;
	this->_mo_1 = 9;
	this->_mo_2 = 10;
	this->_pwm = 8;
	this->_pos_pid = *pid;
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
		pos_pid.tune(100, 0 ,3000);
		pos_pid.limit(-255,255);
		this->_pos_pid = pos_pid;
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
		this->_pos_pid = pos_pid; 
	}
}
void MotorControl::begin(int *enc){
	pinMode(this->_en_1, INPUT);
	pinMode(this->_en_2, INPUT);
	pinMode(this->_mo_1, OUTPUT);
	pinMode(this->_mo_2, OUTPUT);
	pinMode(this->_pwm, OUTPUT);
	this->_curr_set = -1379;
  this->encoder_pos = enc;
}

int MotorControl::control(int set){
	if(this->_curr_set!=set){
	  _pos_pid.setpoint(set);
    this->_curr_set = set;  
	}
	
	
	int power = _pos_pid.compute(*encoder_pos);
	if(abs(power)>thresh){
		analogWrite(this->_pwm, abs(power));
		if(power>0){
			digitalWrite(this->_mo_1, LOW);
			digitalWrite(this->_mo_2, HIGH);
		} else {
			digitalWrite(this->_mo_2, LOW);
			digitalWrite(this->_mo_1, HIGH);
		}

	} else {
		power = 0;
		analogWrite(this->_pwm, 0);
		digitalWrite(this->_mo_1, LOW);
		digitalWrite(this->_mo_2, LOW);
	}
	return power;
}
