#include "drive.h"

Drive::Drive(){
	unlocked = 1;
} 

Drive::Drive(Motor *motorA, Motor *motorB){
	leftDrive = motorA;
	rightDrive = motorB;
	unlocked = 1;
}



void Drive::forward(int lSpeed, int rSpeed){
	if (unlocked){
		leftDrive->on(lSpeed);
		rightDrive->on(rSpeed);
	}
}

void Drive::unlockDrive(bool unLock){
	unlocked = unLock;
	leftDrive->stop();
	rightDrive->stop();
}

bool Drive::isUnlocked(){
	return unlocked;
}
