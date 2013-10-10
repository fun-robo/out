#include "LineReturn.h"

//初期化する
void LineReturn_init(LineReturn* this)
{
	this->phase = 0;
	this->run_time = 0;
	this->angle = 0;
}

//ライン復帰をする
BOOL LineReturn_run(LineReturn *this)
{

	switch(this->phase){
	case 0:
		TailRunner_run(this->tailRunner, 0, 0);
		if(this->run_time > 3000){
			this->run_time = 0;
			this->phase = 1;
		}
		break;
	case 1:
		Straight_tail(this->straight , -30);
		if(GyroSensor_getAngularVelocity(this->gyroSensor) < 560){
			this->phase = 2;
			this->angle = Motor_getAngle(this->rightMotor);
		}
		break;

	case 2:
		Straight_tail(this->straight , 30);
		if((Motor_getAngle(this->rightMotor) - (this->angle)) > 1000){
			this->phase = 3;
		}
		break;
	case 3:
		Straight_tail(this->straight , 0);
		break;
	}
	this->run_time+=4;
	return false;
}