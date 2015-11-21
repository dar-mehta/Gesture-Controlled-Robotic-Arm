#include "armlift.h"
const int MAX_HEIGHT = 1000;

ArmLift::ArmLift(Motor *motorA,Touch *touch){
	liftMotor = motorA;
	elevatorTouch = touch;
	unlocked = false;
}

ArmLift::ArmLift(){
	unlocked=false;
}

void ArmLift::zeroEncoder() {
	elevatorTouch->init();
	if(elevatorTouch->read()!= 1){
		unlocked = true;
		liftMotor->on(-100);
		cout<<"Lowering to bottom "<<endl;
		while(elevatorTouch->read()!=1){
		}
		liftMotor->off();
		liftMotor->reset_rotation();
	}
}


//Void Function, raises arm at specified velocity. 
//Note: velocity must be positive.
void ArmLift::raise(int velocity){
	//If the function is passed a negative velocity, make it positive
	if(velocity<0){
		velocity*=-1;
	}
	//Powers the motor, if the elevator is below max height
	//cout<<"Trying to go up"<<endl;
	//cout<<"Encoder Reading:"<< liftMotor->get_rotation()<<endl;
	if(liftMotor->get_rotation()+5<1000){
				liftMotor->on(velocity);
	} 
	//If the lift is beyond max height, stop the elevator.
	else if(liftMotor->is_running()){
		liftMotor->off();
		cout<<"Maximum Height reached, elevator turning off"<<endl;
	}

}

//Boolean Function, lowers the arm at a set speed
//Note: velocity must be negative
void ArmLift::lower(int velocity){
	
	//if passed a positive velocity, convert to negative
	if(velocity>0){
		velocity*=-1;
	}
	//if the elevator is above min height, turn on the motor
	if(liftMotor->get_rotation()>5){
		liftMotor->on(velocity);
	}
	//If above, turn off the motor and output a flag to console.
	else if(liftMotor ->is_running()){
		cout<<"Minimum Height reached, elevator turning off to avoid robot damage"<<endl;
		liftMotor->off();
	}
}

void ArmLift::stop(){
	liftMotor->off();
}


void ArmLift::unlockArm(){
	unlocked = true;
}

void ArmLift::lockArm(){
	unlocked = false;
	liftMotor->off();
}

bool ArmLift::isUnlocked(){
	return unlocked;
}
