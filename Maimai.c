#include "Maimai.h"
#include "LightSensor.h"

void Maimai_init(Maimai* this){
	int i;
	for(i = 0; i < NUM; i++){
		this->light_value[i] = 0;
	}
	this->index = 0;
}

void Maimai_store(Maimai* this, int run_time){
	//16msec周期でライトを明滅させる
	if(run_time % 16 == 0)	LightSensor_switch(this->lightSensor);

	//4msec周期で輝度値を取得し配列に格納
	this->light_value[this->index]  = LightSensor_getBrightness(this->lightSensor);
	this->index++;
	if(this->index == NUM)	this->index = 0;
}

//配列内の最大の輝度値と最小の輝度値の差分を出す
U16 Maimai_calc(Maimai* this){
	int min = 1000;
	int max = 0;
	int sub;
	int i;
	
	for(i = 0; i < NUM; i++){
		if(min > this->light_value[i])	min = this->light_value[i];
		if(max < this->light_value[i])	max = this->light_value[i];
	}
	sub = max - min;
	if (sub > 255) sub = 255; 
	return (U16) sub;
}
