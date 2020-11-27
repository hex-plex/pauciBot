#include "Arduino.h"
#include <PIDController.h>

class MotorControl{
	public:
		MotorControl(int EN_1, int EN_2, int MO_1,int MO_2, int PWM, PIDController *pid);
		MotorControl(int EN_1, int EN_2, int MO_1,int MO_2, int PWM);
		MotorControl(PIDController *pid);
		MotorControl(bool a);
		int control(int setpoint);
		int encoder_pos=0;
		int thresh = 75;
		void begin(void enc(void));
		void encoder();
	private:
		int _en_1;
		int _en_2;
		int _mo_1;
		int _mo_2;
		int _pwm;
		int _curr_set=0;
		PIDController* _pos_pid;
};
