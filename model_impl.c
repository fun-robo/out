///////////////////////////////////////////////////////////
//  model_impl.c
///////////////////////////////////////////////////////////

#include "LineTracer.h"
#include "UI.h"
#include "BalanceRunner.h"
#include "TouchSensor.h"
#include "LightSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Maimai.h"
#include "Basic.h"
#include "DistMeasure.h"
#include "SeeSaw.h"
#include "Straight.h"
#include "RunMode.h"
#include "TailRunner.h"
#include "LineReturn.h"

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

typedef enum Zone {
		BASIC_RUN, SEESAW_RUN, GARAGE_IN,
} Zone;

// オブジェクトを静的に確保する
LineTracer 	lineTracer;
UI 		ui;
ColorJudgement 	colorJudgement;
BalanceRunner 	balanceRunner;
TouchSensor 	touchSensor;
LightSensor 	lightSensor;
GyroSensor 	gyroSensor;
Motor 		leftMotor, rightMotor, tailMotor;
SonarSensor sonarSensor;
Maimai maimai;
Basic basic;
DistMeasure distMeasure;
SeeSaw seeSaw;
Straight straight;
RunMode runMode;
TailRunner tailRunner;
LineReturn lineReturn;

void ecrobot_device_initialize();
void ecrobot_device_terminate();
void user_1ms_isr_type2(void);
void ecrobot_link();
void ecrobot_init();


TASK(TaskMain)
{
	//定数宣言部
	//const U32 SOUND[8] = {523,587,659,698,783,880,987,1046}; //オクターブ4 ド..シ オクターブ5 ド
	//変数宣言部
	int run_time = 0;
	U16 bright = 0;
	//unsigned char bright_15_8 = 0;
	//unsigned char bright_7_0 = 0;
	int zone = BASIC_RUN; //コース中のどこのゾーン

	//デバイスの初期化
	ecrobot_device_initialize();
	//オブジェクト間のリンク構築
	ecrobot_link();
	//各オブジェクト初期化
	ecrobot_init();
	
	// UIに開始待ちを依頼する
	UI_waitStart(&ui);
	Motor_tailControl(&tailMotor, TAIL_ANGLE_DRIVE);

	// 4ms周期で、ライントレーサにトレース走行を依頼する
	 while(1)
	 {
	 	if(UI_isEmergency(&ui))	break;
	 	if(runMode.run_mode)	Maimai_store(&maimai, run_time);

		switch (zone) {
			case BASIC_RUN:
				Basic_run(&basic);
				if (Basic_getCurPhase(&basic) == BASIC_GOAL) {
				 	zone = SEESAW_RUN;
				 	RunMode_change(&runMode,MAIMAI);
				 	LineTracer_changePID(&lineTracer,0.6,0.08,0.124,LineTracer_getMaimaiTarget(&lineTracer));
				}
			break;
			case SEESAW_RUN:
				if(SeeSaw_run(&seeSaw)) zone = GARAGE_IN;
			break;
			case GARAGE_IN:
				LineReturn_run(&lineReturn);
				break;
		}

		//bright = LineTracer_getBright(&lineTracer);
		
		systick_wait_ms(4);
		run_time+=4;
		
		//ロギングする
		ecrobot_bt_data_logger(0, 0);
		//ecrobot_bt_data_logger( ,bright);
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
	lineTracer.runMode		=&runMode;

	ui.touchSensor		   = &touchSensor;
	ui.tailMotor		   = &tailMotor;
	ui.sonarSensor = &sonarSensor;
	ui.lightSensor = &lightSensor;
	ui.lineTracer = &lineTracer;
	ui.maimai	   = &maimai;
	ui.runMode	   = &runMode;

	balanceRunner.gyroSensor   = &gyroSensor;
	balanceRunner.leftMotor    = &leftMotor;
	balanceRunner.rightMotor   = &rightMotor;

	basic.lineTracer = &lineTracer;
	basic.distMeasure = &distMeasure;

	distMeasure.rightMotor = &rightMotor;

	maimai.lightSensor = &lightSensor;

	seeSaw.gyroSensor	 = &gyroSensor;
	seeSaw.lineTracer	 = &lineTracer;
	seeSaw.rightMotor	 = &rightMotor;
	seeSaw.leftMotor	 = &leftMotor;
	seeSaw.straight		 = &straight;
	seeSaw.tailRunner	 = &tailRunner;

	straight.balanceRunner = &balanceRunner;
	straight.rightMotor  = &rightMotor;
	straight.leftMotor   = &leftMotor;
	straight.gyroSensor  = &gyroSensor;
	straight.tailRunner	 = &tailRunner;

	tailRunner.tailMotor = &tailMotor;
	tailRunner.lineTracer = &lineTracer;
	tailRunner.gyroSensor = &gyroSensor;
	tailRunner.balanceRunner = &balanceRunner;

	lineReturn.tailRunner = &tailRunner;
	lineReturn.gyroSensor = &gyroSensor;
	lineReturn.straight   = &straight;
	lineReturn.rightMotor = &rightMotor;
}

void ecrobot_init(){
	// 各オブジェクトを初期化する
	RunMode_init(&runMode);
	UI_init(&ui);
	LineTracer_init(&lineTracer);
	BalanceRunner_init(&balanceRunner);
	TouchSensor_init(&touchSensor, NXT_PORT_S4);
	LightSensor_init(&lightSensor, NXT_PORT_S3);
	GyroSensor_init(&gyroSensor, NXT_PORT_S1);
	Motor_init(&leftMotor, NXT_PORT_C);
	Motor_init(&rightMotor, NXT_PORT_B);
	Motor_init(&tailMotor, NXT_PORT_A);
	SonarSensor_init(&sonarSensor, NXT_PORT_S2);
	Maimai_init(&maimai);
	DistMeasure_init(&distMeasure);
	Basic_init(&basic);
	SeeSaw_init(&seeSaw);
	Straight_init(&straight);
	TailRunner_init(&tailRunner);
	LineReturn_init(&lineReturn);
}
