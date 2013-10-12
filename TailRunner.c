#include "TailRunner.h"
#include "LineTracer.h"
#include "BalanceRunner.h"
#include "Motor.h"
#include "GyroSensor.c"

void TailRunner_init(TailRunner* this)
{
  this->runtime = 0;
  this->phase = 0;
  this->angle = 85;
}

void TailRunner_run(TailRunner* this, int forward, int turn)//10,1
{
  if(this->phase == 4){
    LineTracer_trace_nonbalance(this->lineTracer, forward, turn);
  }
  TailRunner_set_tailMotor(this);
}

void TailRunner_run_nonlinetrace(TailRunner* this, int turn, int forward)//10,1
{
  if(this->phase == 4){
    NonBalanceRunner_run(this->balanceRunner, turn, forward);
  }
  TailRunner_set_tailMotor(this);
}

void TailRunner_changeTailAngle(TailRunner* this, char angle)
{
  this->angle = angle;
}

void TailRunner_set_tailMotor(TailRunner* this)
{
  Motor_tailControl(this->tailMotor, this->angle);
  switch(this->phase){
    case 0:
      TailRunner_changeTailAngle(this, 85);
      LineTracer_trace(this->lineTracer, 0, 0);
      if(this->runtime > 2000){//少し進んだら
        ecrobot_sound_tone(349, 100, 100);
        this->phase = 1;
        this->runtime = 0;
      }
      this->runtime += 4;
      break;
    case 1: 
      GyroSensor_changeOffset(this->gyroSensor, GYRO_OFFSET-20);
      LineTracer_trace(this->lineTracer, 0, 0);
      TailRunner_changeTailAngle(this, 85);
      if(this->runtime > 100){//少し進んだら
        ecrobot_sound_tone(349, 100, 100);
        this->phase = 2;
        this->runtime = 0;
        GyroSensor_changeOffset(this->gyroSensor, GYRO_OFFSET);
      }
      this->runtime += 4;
      break;
    case 2:
      TailRunner_changeTailAngle(this, 85 - this->runtime/100);
      LineTracer_trace_nonbalance(this->lineTracer, 0, 0);
      if(this->runtime >= 1800){//少したったら
        ecrobot_sound_tone(349, 100, 100);
        this->phase = 3;
        this->runtime = 0;
        LineTracer_changePID(this->lineTracer, 0.7, 0.1, 0.17, get_TARGET_tail(this->lineTracer));
      }
      this->runtime+=4;
      break;
    case 3: 
      TailRunner_changeTailAngle(this, 67);
      LineTracer_trace_nonbalance(this->lineTracer, 0, 0);
      if(this->runtime > 1000){//少し進んだら
        ecrobot_sound_tone(349, 100, 100);
        this->phase = 4;
        this->runtime = 0;
      }
      this->runtime+=4;
      break;
    default: break;
  }
}
