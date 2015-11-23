#include "armlift.h"
const int MAX_HEIGHT = -11000;
const int MIN_HEIGHT = -100;

ArmLift::ArmLift(Motor *motorA,Touch *touch){
	liftMotor = motorA;
	elevatorTouch = touch;
	unlocked = false;
}

ArmLift::ArmLift(){
	unlocked=false;
}

void ArmLift::initialize() {
	elevatorTouch->init();
	liftMotor->reset_Encoder();
	if(elevatorTouch->read()!= 1){
		unlocked = true;
		liftMotor->on(100);
		cout<<"Lowering to bottom "<<endl;
		while(elevatorTouch->read()!=1){}
	}
	liftMotor->off();
	liftMotor->reset_Encoder();
	cout << liftMotor->get_Encoder();
	i=0;
}

//Void Function, raises arm at specified velocity. 
//Note: velocity must be positive.
void ArmLift::move(int velocity){
	i++;
	if(i%3 == 0){
		encoder = liftMotor->get_Encoder();
		//cout << encoder << endl;
	}
	
	if (velocity < 0 && encoder > MAX_HEIGHT){
		liftMotor->on(velocity);
	} else if (velocity > 0 && encoder < MIN_HEIGHT){
		liftMotor->on(velocity);
	} else {
		liftMotor->on(0);
	}
	
	/*if (elevatorTouch->read() == 1){
		liftMotor->reset_Encoder();
	}*/
	
}

void ArmLift::unlockArm(bool unLock){
	unlocked = unLock;
	liftMotor->stop();
}

bool ArmLift::isUnlocked(){
	return unlocked;
}

