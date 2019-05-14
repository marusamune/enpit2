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
	//plain�̏���
	plain();
	dly_tsk(200);
	zumo.driveTank(0,0);
	//slope�̏���
	slope();
	dly_tsk(200);
	zumo.driveTank(0,0);
	//top�̏���
	top();
	dly_tsk(200);
	zumo.driveTank(0,0);
	/*
	//cliff�̏���
	cliff();

	//goal�܂ł̏���
	goal();
	 */

}

void plain(){
	while(1){
		//�O�i
		zumo.driveTank(70,70);
		//�⓹���o
		if(searchSlope()){
			//�⓹�����o������break
			break;
		}
	}
}

void slope(){
	//LED1��_��
	led_r = 1;
	while(1){
		//�O�i
		zumo.driveTank(70,70);
		//�g�b�v�G���A�����o
		if(searchTop()){
			//�g�b�v�G���A�����o������break
			break;
		}
	}
}

void top(){
	//��~
	zumo.driveTank(0,0);
	//LED1������
	led_r = 0;
	//LED2��3�b�ԓ_��
	led_blink(led_g);
}

void cliff(){
	while(1){
		//�O�i
		zumo.driveTank(70,70);
		//�������o
		if(searchBlack()){
			//���������o������LED3��_����break
			break;
		}
	}

	while(1){
		//�O�i
		zumo.driveTank(60,60);
		//�R���o
		if(searchCliff()){
			//�R�����o�������~����break
			zumo.driveTank(0,0);
			break;
		}
	}
}

void goal(){
	//��i
	//�������o(1�{��)
	//���������o������break

	//��i
	//�������o
	//���������o������break
}

bool searchSlope(){
	short x,y,z;

	//�����x���擾
	zumo.getAcceleration(&x,&y,&z);
	if(x > 5000){
		//x�����̉����x5000�ȏ�Ȃ�⓹����
		return true;
	}else{
		return false;
	}
}

bool searchTop(){
	short x,y,z;

	//�����x���擾
	zumo.getAcceleration(&x,&y,&z);
	if(x < 500){
		//x�����̉����x500�ȉ��Ȃ�g�b�v����
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

	//IR�Z���T�l���擾
	zumo.readAnalogIrValue(IR_values);
	if(IR_values[0] > BLACK){
		//�l��臒l�ȏ�Ȃ獕������
		return true;
	}else{
		return false;
	}
}

bool searchCliff(){
	unsigned int IR_values[6];

	//IR�Z���T�l���擾
	zumo.readAnalogIrValue(IR_values);
	if(IR_values[0] > CLIFF){
		//�l��臒l�ȏ�Ȃ�R����
		return true;
	}else{
		return false;
	}
}
