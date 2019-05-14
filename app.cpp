#include <kernel.h>
#include "kernel_cfg.h"
#include "app.h"
#include "mbed.h"
#include "app_config.h"
#include "Zumo.h"
#include "Milkcocoa.h"
Serial pc(USBTX, USBRX);
Zumo zumo;
DigitalIn sw1(D12);
DigitalOut led_r(LED1);
DigitalOut led_g(LED2);
DigitalOut led_b(LED3);

void plain();
void slope();
void top();
void cliff();
void goal();
bool searchSlope();
bool searchTop();
void led_blink(DigitalOut);
bool searchBlack();
bool searchCliff();

static int BLACK = 600;
static int CLIFF = 1000;

void task_main(intptr_t exinf){
	int n;

	sw1.mode(PullUp);
	n = sw1;
	while(sw1 != 0){

	}

	dly_tsk(1000);
	//plainの処理
	plain();
	dly_tsk(200);
	zumo.driveTank(0,0);
	//slopeの処理
	slope();
	dly_tsk(200);
	zumo.driveTank(0,0);
	//topの処理
	top();
	dly_tsk(200);
	zumo.driveTank(0,0);
	/*
	//cliffの処理
	cliff();

	//goalまでの処理
	goal();
	 */

}

void plain(){
	while(1){
		//前進
		zumo.driveTank(70,70);
		//坂道検出
		if(searchSlope()){
			//坂道を検出したらbreak
			break;
		}
	}
}

void slope(){
	//LED1を点灯
	led_r = 1;
	while(1){
		//前進
		zumo.driveTank(70,70);
		//トップエリアを検出
		if(searchTop()){
			//トップエリアを検出したらbreak
			break;
		}
	}
}

void top(){
	//停止
	zumo.driveTank(0,0);
	//LED1を消灯
	led_r = 0;
	//LED2を3秒間点滅
	led_blink(led_g);
}

void cliff(){
	while(1){
		//前進
		zumo.driveTank(70,70);
		//黒線検出
		if(searchBlack()){
			//黒線を検出したらLED3を点灯しbreak
			break;
		}
	}

	while(1){
		//前進
		zumo.driveTank(60,60);
		//崖検出
		if(searchCliff()){
			//崖を検出したら停止してbreak
			zumo.driveTank(0,0);
			break;
		}
	}
}

void goal(){
	//後進
	//黒線検出(1本目)
	//黒線を検出したらbreak

	//後進
	//黒線検出
	//黒線を検出したらbreak
}

bool searchSlope(){
	short x,y,z;

	//加速度を取得
	zumo.getAcceleration(&x,&y,&z);
	if(x > 5000){
		//x方向の加速度5000以上なら坂道発見
		return true;
	}else{
		return false;
	}
}

bool searchTop(){
	short x,y,z;

	//加速度を取得
	zumo.getAcceleration(&x,&y,&z);
	if(x < 500){
		//x方向の加速度500以下ならトップ発見
		return true;
	}else{
		return false;
	}
}
void led_blink(DigitalOut led){
	int i;
	for(i = 0 ; i<=2 ; i++){
		led = 1;
		dly_tsk(500);
		led = 0;
		dly_tsk(500);
	}
}
bool searchBlack(){
	unsigned int IR_values[6];

	//IRセンサ値を取得
	zumo.readAnalogIrValue(IR_values);
	if(IR_values[0] > BLACK){
		//値が閾値以上なら黒線発見
		return true;
	}else{
		return false;
	}
}

bool searchCliff(){
	unsigned int IR_values[6];

	//IRセンサ値を取得
	zumo.readAnalogIrValue(IR_values);
	if(IR_values[0] > CLIFF){
		//値が閾値以上なら崖発見
		return true;
	}else{
		return false;
	}
}
