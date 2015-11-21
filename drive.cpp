#include "drive.h"

Drive::Drive(Motor *motorA, Motor *motorB){
	leftDrive = motorA;
	rightDrive = motorB;
	unlocked = true;
}

void Drive::forward(int lSpeed, int rSpeed){
	if (unlocked){
		leftDrive->on(lSpeed);
		rightDrive->on(rSpeed);
	}
}

void Drive::unlockDrive(){
	unlocked = true;
}

void Drive::lockDrive(){
	unlocked = false;
	leftDrive->off();
	rightDrive->off();
}

bool Drive::isUnlocked(){
	return unlocked;
}
