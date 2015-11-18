#include "armlift.h"

ArmLift::ArmLift(Motor *motorA){
	liftArm = motorA;
	unlocked = true;
}

void ArmLift::raise(int speed){
	if (unlocked){
		liftArm->on(speed);
	}
}

void ArmLift::lower(int speed){
	if (unlocked){
		liftArm->on(-speed);
	}
}

void ArmLift::unlockArm(){
	unlocked = true;
}

void ArmLift::lockArm(){
	unlocked = false;
	liftArm->off();
}

bool ArmLift::isUnlocked(){
	return unlocked;
}
