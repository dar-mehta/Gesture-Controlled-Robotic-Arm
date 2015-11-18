#include "claw.h"

Claw::Claw(Motor *motorA, Motor *motorC){
	clawMotor = motorA;
	rotateClawMotor = motorC;
	unlocked = true;
	clawIsOpen = true;
}

void Claw::open(int speed){
	if (!clawIsOpen){
		clawMotor->on(speed);
		clawIsOpen = true;
	}	
}

void Claw::close(int speed){
	if (clawIsOpen){
		clawMotor->off();
		clawIsOpen = false;
	}
}

bool Claw::isOpen(){
	return clawIsOpen;
}

void Claw::rotateClaw(int speed){
	rotateClawMotor->on(speed);
}

void Claw::unlockClaw(){
	unlocked = true;
}

void Claw::lockClaw(){
	unlocked = false;
	clawMotor->off();
	rotateClawMotor->off();
}

bool Claw::isUnlocked(){
	return unlocked;
}
