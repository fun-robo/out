#if !defined(INCLUDED_BASIC_H_)
#define INCLUDED_BASIC_H_

#include "LineTracer.h"
#include "DistMeasure.h"

typedef enum Basic_gate {
		BASIC_START, SLOPE_UP, SLOPE_DOWN, GATE_ONE, GATE_TWO_BEFORE, GATE_TWO, GATE_TWO_AFTER, GATE_THREE, GATE_FOUR, BASIC_GOAL,EM,EM1
} Basic_gate;

typedef struct Basic
{
	LineTracer *lineTracer;
	DistMeasure *distMeasure;
	int run_time;
	int speed;
	int cur_phase;
	
} Basic;

void Basic_init(Basic *this);
void Basic_run(Basic *this);
int Basic_getCurPhase(Basic *this);
#endif
