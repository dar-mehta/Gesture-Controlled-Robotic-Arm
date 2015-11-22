#include "claw.h"

Claw::Claw(Motor *motorA, Motor *motorC){
	clawMotor = motorA;
	rotateClawMotor = motorC;
	unlocked = true;
	isOpen = true;
	rotated = false;
}

void Claw::initialize(){
    clawMotor->reset_Encoder();
	rotateClawMotor->reset_Encoder();
	initClaw = clawMotor->get_Encoder();
	initRotation = rotateClawMotor->get_Encoder();
}

void Claw::open(){
	if (!isOpen && !rotated){
		clawMotor->on(-20);
		int currentPos = clawMotor->get_Encoder()-initClaw;
		int i = 0;
		while(currentPos < rotation+120){
			i++;
			if(i%2==0){
				currentPos = clawMotor->get_Encoder()-initClaw;
			}
		}
		clawMotor->off();
		isOpen = true;
	}	
}

void Claw::close(){
	if (isOpen){
		clawMotor->on(20);
		while(clawMotor->get_Encoder() > rotation){}
		clawMotor->off();
		isOpen = false;
	}
}

void Claw::rotate(int speed){
	if(!isOpen && !rotated)
	rotateClawMotor->on(speed);
}

void Claw::unlockClaw(bool unLock){
	unlocked = unLock;
	clawMotor->off();
	rotateClawMotor->off();
}

bool Claw::isUnlocked(){
	return unlocked;
}


