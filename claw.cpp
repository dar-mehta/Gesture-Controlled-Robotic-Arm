#include "claw.h"

Claw::Claw(Motor *motorA, Motor *motorC){
	clawMotor = motorA;
	rotateClawMotor = motorC;
	unlocked = true;
	isOpen = true;
	rotated = false;
}

void Claw::initialize(){
	clawMotor->off();
	clawMotor->reset_Encoder();
	initEncoder = clawMotor->get_Encoder();
	cout<<clawMotor->get_Encoder()<<endl;
}

void Claw::open(){
	if (!isOpen && !rotated){
		
		clawMotor->on(20);
		while(encoder-initEncoder < 120){
			if (i%5 == 0){
				encoder = clawMotor->get_Encoder();
				cout << clawMotor->get_Encoder() << endl;
			}
			i++;
		}
		clawMotor->stop();
		initEncoder = clawMotor->get_Encoder();
		isOpen = true;
		//while (encoder < 500){}
		//clawMotor->off();
		//isOpen = true;
		/*int currentPos = clawMotor->get_Encoder();
		int i = 0;
		while(currentPos > -120){
			i++;
			if(i%2==0){
				currentPos = clawMotor->get_Encoder();
			}
		}*/
	}
}

void Claw::close(){
	if (isOpen){
		
		clawMotor->on(-20);
		while(encoder - initEncoder > -120){
			if (i%5 == 0){
				encoder = clawMotor->get_Encoder();
				cout << clawMotor->get_Encoder() << endl;
			}
			i++;
		}
		clawMotor->stop();
		initEncoder = clawMotor->get_Encoder();
		isOpen = false;
		//while (encoder > 0){}
		//clawMotor->off();
		//isOpen = false;
	}
}

void Claw::rotate(int speed){
	if(!isOpen && !rotated);
	rotateClawMotor->on(speed);
}

void Claw::unlockClaw(bool unLock){
	unlocked = unLock;
	clawMotor->stop();
	rotateClawMotor->off();
}

bool Claw::isUnlocked(){
	return unlocked;
}


