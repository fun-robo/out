#include "Basic.h"

void Basic_init(Basic *this)
{
	this->run_time = 0;
	this->speed = 100;
	this->cur_phase = GATE_FOUR;//0;
}

void Basic_run(Basic *this)
{
	const U32 SOUND[8] = {523,587,659,698,783,880,987,1046};
	LineTracer_trace(this->lineTracer, this->speed, 1);
		
	switch (this->cur_phase){
	// 	case BASIC_START: //坂道頂点
	// 		if(DistMeasure_getDistance(this->distMeasure) > 2300) {
	// 			ecrobot_sound_tone(SOUND[0], 150, 100);
	// 			this->cur_phase = SLOPE_TOP;
	// 			this->speed = 100;
	// 		}
	// 		break;
	// 	case SLOPE_TOP: //第一チェックポイント
	// 		if(DistMeasure_getDistance(this->distMeasure) > 5200) {
	// 			ecrobot_sound_tone(SOUND[1], 150, 100);
	// 			this->cur_phase = GATE_ONE;
	// 			this->speed = 130;
	// 		}
	// 		break;
	// 	case GATE_ONE: //第２チェックポイント
	// 		if (DistMeasure_getDistance(this->distMeasure) > 9400) {
	// 			ecrobot_sound_tone(SOUND[2],150,100);
	// 			this->cur_phase = GATE_TWO;
	// 		}
	// 		break;
	// 	case GATE_TWO: //第３チェックポイント
	// 		if (DistMeasure_getDistance(this->distMeasure) > 14500) {
	// 			ecrobot_sound_tone(SOUND[3],150,100);
	// 			this->cur_phase = GATE_THREE;
	// 		} 
	// 		break;
	// 	case GATE_THREE: //第４チェックポイント
	// 		if (DistMeasure_getDistance(this->distMeasure) > 19100) {
	// 			ecrobot_sound_tone(SOUND[4],150,100);
	// 			this->cur_phase = GATE_FOUR;
	// 			this->speed = 90;
	// 		} 
	// 		break;
	// 	case GATE_FOUR: //Basicゴール
	// 		if (DistMeasure_getDistance(this->distMeasure) > 25201) {
	// 			ecrobot_sound_tone(SOUND[5],250,100);
	// 			LineTracer_changePID(this->lineTracer,0.66,0.08,0.124,
	// 						LineTracer_getTarget(this->lineTracer)+15);
	// 			this->speed = 60;
	// 			this->cur_phase = BASIC_GOAL;
	// 		}
	// 		break;
	// 	case BASIC_GOAL:
	// 	 	break;
		case GATE_FOUR: //Basicゴール
			if (DistMeasure_getDistance(this->distMeasure) > 6101) {
				ecrobot_sound_tone(SOUND[5],250,100);
				LineTracer_changePID(this->lineTracer,0.66,0.08,0.124,
							LineTracer_getTarget(this->lineTracer)+15);
				this->speed = 60;
				this->cur_phase = BASIC_GOAL;
			}
			break;
		case BASIC_GOAL:
		 	break;
	 }
}

int Basic_getCurPhase(Basic *this)
{
	return this->cur_phase;
}
