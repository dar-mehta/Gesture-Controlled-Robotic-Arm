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

void Drive::unlockDrive(bool unLock){
	unlocked = unLock;
	leftDrive->off();
	rightDrive->off();
}

bool Drive::isUnlocked(){
	return unlocked;
}
