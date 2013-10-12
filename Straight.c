#include "Straight.h"

int Calc_angle(Straight *this){
	return ((Motor_getAngle(this->rightMotor) - this->r_motor_angle) - (Motor_getAngle(this->leftMotor) - this->l_motor_angle) + 0.5);
}

void Straight_init(Straight *this){
	this->finish_flag = FALSE;
	this->r_motor_angle = Motor_getAngle(this->rightMotor);
	this->l_motor_angle = Motor_getAngle(this->leftMotor);
}

void Straight_run(Straight *this, int forword){
	if(!this->finish_flag){
		Straight_init(this);
		this->finish_flag = TRUE;
	}

	BalanceRunner_run(this->balanceRunner, Calc_angle(this), forword);
}

void Straight_tail(Straight *this, int forword){
	if(!this->finish_flag){
		Straight_init(this);
		this->finish_flag = TRUE;
	}

	 TailRunner_run(this->tailRunner, forword, 0);
}

void Straight_finish(Straight*this){
	this->finish_flag = FALSE;
}
