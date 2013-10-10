#if !defined(LineReturn__INCLUDED_)
#define LineReturn__INCLUDED_

#include "TailRunner.h"
#include "GyroSensor.h"
#include "Straight.h"
#include "Motor.h"

typedef struct LineReturn
{
	TailRunner *tailRunner;
	GyroSensor *gyroSensor;
	Straight   *straight;
	Motor	   *rightMotor;
	int phase;
	int run_time;
	long angle;
	
} LineReturn;

void LineReturn_init(LineReturn *this);
BOOL LineReturn_run(LineReturn *this);

#endif /*!defined(EA_0311591B_68F8_4d9d_A572_FDE0A82543E8__INCLUDED_)*/
