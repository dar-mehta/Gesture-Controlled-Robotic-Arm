#ifndef __ARM__LIFT__
#define __ARM__LIFT__
#include "motor.h"
#include "touch.h"

class ArmLift{
	private:
	    
	    Touch *elevatorTouch;
	    int i;
		bool unlocked;
		
	public :
		Motor *liftMotor;
		ArmLift(Motor*,Touch*);
		ArmLift();
		
		void initialize();
		void move(int);

		
		void unlockArm(bool);
		bool isUnlocked();
		long int encoder;
};
#endif
