///////////////////////////////////////////////////////////
//  model_impl.c
///////////////////////////////////////////////////////////

#include "LineTracer.h"
#include "UI.h"
#include "ColorJudgement.h"
#include "BalanceRunner.h"
#include "TouchSensor.h"
#include "LightSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "GarageIn.h"
#include "Basic.h"
#include "LookUpGate.h"
#include "LookUpRunner.h"
#include "LineReturn.h"
#include "Maimai.h"

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

// オブジェクトを静的に確保する
LineTracer 	lineTracer;
UI 		ui;
ColorJudgement 	colorJudgement;
BalanceRunner 	balanceRunner;
TouchSensor 	touchSensor;
LightSensor 	lightSensor;
GyroSensor 	gyroSensor;
Motor 		leftMotor, rightMotor, tailMotor;
GarageIn	garageIn;
Basic		basic;
LookUpGate lookUpGate;
LookUpRunner lookUpRunner;
SonarSensor sonarSensor;
Maimai maimai;
LineReturn	lineReturn;

void ecrobot_device_initialize();
void ecrobot_device_terminate();
void user_1ms_isr_type2(void);
void ecrobot_link();
void ecrobot_init();


TASK(TaskMain)
{
	//変数宣言部
	int run_time = 0;
	int gyro = 0;
	long motor_r = 0;
	int zone = 0;
	int forward = 100;
	//デバイスの初期化
	ecrobot_device_initialize();
	//オブジェクト間のリンク構築
	ecrobot_link();
	//各オブジェクト初期化
	ecrobot_init();
	
	// UIに開始待ちを依頼する
	UI_waitStart(&ui);

	// 4ms周期で、ライントレーサにトレース走行を依頼する
	while(1)
	{
		if(UI_isEmergency(&ui))	break;
		Maimai_store(&maimai, run_time);
		LineTracer_trace(&lineTracer, forward, run_time);

		gyro = GyroSensor_getAngularVelocity(&gyroSensor);
		motor_r = Motor_getAngle(&rightMotor);
		
		switch(zone) {
			case 0: //坂道頂点
				if(motor_r > 2000) {
					ecrobot_sound_tone(587, 250, 100);
					zone = 1;
					forward = 100;
				}
				break;
			case 1: //第一チェックポイント
				if(motor_r > 5000) {
					ecrobot_sound_tone(523, 250, 100);
					zone = 2;
					forward = 80;
				}
				break;
			case 2: //Basicゴール
				if (motor_r > 25201) {
					ecrobot_sound_tone(493,250,100);
					LineTracer_changePID(&lineTracer,0.66,0.07,0.07,LineTracer_getTarget(&lineTracer)+10);
					//Motor_tailControl(&tailMotor,60);
					zone = 3;
				} 
				break;
			case 3: //シーソー直前でスピード落とす
				if (motor_r > 26200) {
					ecrobot_sound_tone(440,250,100);
					zone = 4;
					forward = 40;
				}
				break;
			case 4:
				if (gyro < 500 && gyro > 700) {
					ecrobot_sound_tone(523,10,100);
				}
				break;
		}
		run_time+=4;
		systick_wait_ms(4);
		
		//ロギングする
		ecrobot_bt_data_logger(zone, 0);
	}

}

// デバイス初期化用フック関数
// この関数はnxtOSEK起動時に実行されます。
void ecrobot_device_initialize()
{
	// センサ、モータなどの各デバイスの初期化関数を
	// ここで実装することができます
	// ⇒　光センサ赤色LEDをONにする
	ecrobot_set_light_sensor_active(NXT_PORT_S3);
	if(ecrobot_get_bt_status() == BT_NO_INIT){
		ecrobot_set_bt_device_name("ET025");
	}
	ecrobot_init_bt_slave("LEJOS-OSEK");
}

// デバイス終了用フック関数
// この関数はSTOPまたはEXITボタンが押された時に実行されます。
void ecrobot_device_terminate()
{
	// センサ、モータなどの各デバイスの終了関数を
	// ここで実装することができます。
	// ⇒　光センサ赤色LEDをOFFにする
	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);
	ecrobot_term_sonar_sensor(NXT_PORT_S2);
	ecrobot_term_bt_connection();
}

// 1msec周期割り込み(ISRカテゴリ2)から起動されるユーザー用フック関数
void user_1ms_isr_type2(void)
{
	// 今回は何も行わない
}

void ecrobot_link(){
	// オブジェクト間のリンクを構築する
	lineTracer.colorJudgement  = &colorJudgement;
	lineTracer.balanceRunner   = &balanceRunner;
	lineTracer.lightSensor	     = &lightSensor;
	lineTracer.maimai            = &maimai;

	ui.touchSensor		   = &touchSensor;
	ui.tailMotor		   = &tailMotor;
	ui.sonarSensor = &sonarSensor;
	ui.lightSensor = &lightSensor;
	ui.lineTracer = &lineTracer;
	ui.sonarSensor = &sonarSensor;
	ui.maimai	   = &maimai;

	colorJudgement.lightSensor = &lightSensor;

	balanceRunner.gyroSensor   = &gyroSensor;
	balanceRunner.leftMotor    = &leftMotor;
	balanceRunner.rightMotor   = &rightMotor;

	garageIn.leftMotor		  = &leftMotor;
	garageIn.rightMotor		  = &rightMotor;
	garageIn.tailMotor       = &tailMotor;

	basic.leftMotor			  = &leftMotor;
	basic.rightMotor		  = &rightMotor;
	basic.gyroSensor		  = &gyroSensor;
	basic.lineTracer		  = &lineTracer;
	basic.lightSensor         = &lightSensor;
	basic.ui				  = &ui;

	lookUpGate.lineTracer = &lineTracer;
	lookUpGate.lookUpRunner = &lookUpRunner;
	lookUpGate.sonarSensor = &sonarSensor;
	lookUpGate.colorJudgement  = &colorJudgement;

	lookUpRunner.gyroSensor = &gyroSensor;
	lookUpRunner.leftMotor = &leftMotor;
	lookUpRunner.rightMotor = &rightMotor;
	lookUpRunner.tailMotor = &tailMotor;

	lineReturn.lightSensor = &lightSensor;
	lineReturn.balanceRunner = &balanceRunner;
	lineReturn.lineTracer = &lineTracer;

	maimai.lightSensor = &lightSensor;
}

void ecrobot_init(){
	// 各オブジェクトを初期化する
	LineTracer_init(&lineTracer);
	UI_init(&ui);
	ColorJudgement_init(&colorJudgement);
	BalanceRunner_init(&balanceRunner);
	TouchSensor_init(&touchSensor, NXT_PORT_S4);
	LightSensor_init(&lightSensor, NXT_PORT_S3);
	GyroSensor_init(&gyroSensor, NXT_PORT_S1);
	Motor_init(&leftMotor, NXT_PORT_C);
	Motor_init(&rightMotor, NXT_PORT_B);
	Motor_init(&tailMotor, NXT_PORT_A);
	Basic_init(&basic);
	SonarSensor_init(&sonarSensor, NXT_PORT_S2);
	LookUpGate_init(&lookUpGate);
	LookUpRunner_init(&lookUpRunner);
	LineReturn_init(&lineReturn);
	Maimai_init(&maimai);
}


