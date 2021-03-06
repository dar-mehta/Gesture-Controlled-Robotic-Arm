#include "claw.h"
#include <cmath>

const int TOL = -20;

Claw::Claw(){
	unlocked=true;
	isOpen=true;
	rotated=false;
	
} 


Claw::Claw(Motor *motorA, Motor *motorC){
	clawMotor = motorA;
	rotateClawMotor = motorC;
	unlocked = true;
	isOpen = true;
	rotated = false;
}

void Claw::initialize(){
	clawMotor->off();
	rotateClawMotor->off();
	clawMotor->reset_Encoder();
	rotateClawMotor->reset_Encoder();
	rotationEncoder = initRotateEncoder = rotateClawMotor->get_Encoder();
	clawEncoder = initClawEncoder = clawMotor->get_Encoder();
	cout<<clawMotor->get_Encoder()<<endl;
	cout<<clawMotor->get_Encoder()<<endl;
	i=0;
	
}

void Claw::open(){
	if (!isOpen && !rotated && unlocked){
		j = 0;
		clawMotor->on(20);
		while(fabs(clawEncoder-initClawEncoder) - 120 < TOL && j< 250){
			j++;
			if (i%5 == 0){
				clawEncoder = clawMotor->get_Encoder();
				//cout << clawMotor->get_Encoder() << endl;
			}
			i++;
		}
		clawMotor->stop();
		initClawEncoder = clawMotor->get_Encoder();
		isOpen = true;
	}
}

void Claw::close(){
	if (isOpen && unlocked){
		
		clawMotor->on(-20);
		j = 0;
		while(fabs(clawEncoder - initClawEncoder) - 120 < TOL && j<250){
			j++;
			if (i%5 == 0){
				clawEncoder = clawMotor->get_Encoder();
				//cout << clawMotor->get_Encoder() << endl;
			}
			i++;
		}
		clawMotor->stop();
		initClawEncoder = clawMotor->get_Encoder();
		isOpen = false;
	}
}

void Claw::rotate(int target){
	if(unlocked){
	
		if (i%3 == 0){
			rotationEncoder = rotateClawMotor->get_Encoder();
			//cout << "ENCODER: " << rotationEncoder << endl;	
		}
		i++;
		if(rotationEncoder < target - 6){
			rotateClawMotor->on(20);
		}
		else if(rotationEncoder > target + 6){
			rotateClawMotor->on(-10);
		}
		else{
			rotateClawMotor->off();
		}
		
		//cout << "Claw Encoder: " << rotateClawMotor->get_Encoder() << endl;
		//rotated = true;
	}
	/*if (target == 0){
		rotated = false;
	}*/
}

void Claw::unlockClaw(bool unLock){
	unlocked = unLock;
	clawMotor->stop();
	rotateClawMotor->off();
}

bool Claw::isUnlocked(){
	return unlocked;
}


