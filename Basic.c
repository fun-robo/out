#include "Basic.h"

void Basic_init(Basic *this)
{
	this->run_time = 0;
	this->speed = 110;
	this->cur_phase = EM;
}

void Basic_run(Basic *this)
{
	const U32 SOUND[8] = {523,587,659,698,783,880,987,1046};
	LineTracer_trace(this->lineTracer, this->speed, -1);
		
	switch (this->cur_phase){
		case BASIC_START: //坂道頂点
			if(DistMeasure_getDistance(this->distMeasure) > 1940) {
				ecrobot_sound_tone(SOUND[0], 150, 100);
				this->cur_phase = SLOPE_UP;
				this->speed = 90;
			}
			break;
		case SLOPE_UP:
			if(DistMeasure_getDistance(this->distMeasure) > 2660) {
				ecrobot_sound_tone(SOUND[1], 150, 100);
				this->cur_phase = SLOPE_DOWN;
				this->speed = 110;
			}
			break;
		case SLOPE_DOWN: //第一チェックポイント
		if(DistMeasure_getDistance(this->distMeasure) > 5200) {
			ecrobot_sound_tone(SOUND[1], 150, 100);
			this->cur_phase = GATE_ONE;
			this->speed = 130;
		}
		break;
		case GATE_ONE: //第２チェックポイント
			if (DistMeasure_getDistance(this->distMeasure) > 9400) {
				ecrobot_sound_tone(SOUND[2],150,100);
				this->cur_phase = GATE_TWO;
				this->speed = 120;
			}
			break;
		case GATE_TWO_BEFORE: //第３チェックポイント
			if (DistMeasure_getDistance(this->distMeasure) > 13600) {
				ecrobot_sound_tone(SOUND[3],150,100);
				this->cur_phase = GATE_THREE;
				this->speed = 100;
			} 
			break;
		case GATE_TWO: //第３チェックポイント
			if (DistMeasure_getDistance(this->distMeasure) > 14500) {
				ecrobot_sound_tone(SOUND[3],150,100);
				this->cur_phase = GATE_THREE;
			} 
			break;
		case GATE_TWO_AFTER: //第３チェックポイント
			if (DistMeasure_getDistance(this->distMeasure) > 15500) {
				ecrobot_sound_tone(SOUND[3],150,100);
				this->cur_phase = GATE_THREE;
				this->speed = 140;
			} 
			break;
		case GATE_THREE: //第４チェックポイント
			if (DistMeasure_getDistance(this->distMeasure) > 19100) {
				ecrobot_sound_tone(SOUND[4],150,100);
				this->cur_phase = GATE_FOUR;
				this->speed = 110;
			} 
			break;
		case GATE_FOUR: //Basicゴール
			if (DistMeasure_getDistance(this->distMeasure) > 25201) {
				ecrobot_sound_tone(SOUND[5],250,100);
				LineTracer_changePID(this->lineTracer,0.66,0.08,0.124,
							LineTracer_getTarget(this->lineTracer)+15);
				this->speed = 60;
				this->cur_phase = BASIC_GOAL;
			}
			break;
		case BASIC_GOAL:
		 	break;
		 case EM:
		 	if (DistMeasure_getDistance(this->distMeasure) > 6101) {
				ecrobot_sound_tone(SOUND[5],250,100);
				LineTracer_changePID(this->lineTracer,0.66,0.08,0.124,
							LineTracer_getTarget(this->lineTracer)+15);
				this->speed = 5;
				this->cur_phase = BASIC_GOAL;
			}
		break;
		// case GATE_FOUR: //Basicゴール6101
		// 	if (DistMeasure_getDistance(this->distMeasure) > 6101){
		// 		ecrobot_sound_tone(SOUND[5],250,100);
		// 		LineTracer_changePID(this->lineTracer,0.66,0.08,0.124,
		// 					LineTracer_getTarget(this->lineTracer)+15);
		// 		this->speed = 60;
		// 		this->cur_phase = BASIC_GOAL;
		// 	}
		// 	break;
		// case BASIC_GOAL:
		//  	break;
	 }
}

int Basic_getCurPhase(Basic *this)
{
	return this->cur_phase;
}
