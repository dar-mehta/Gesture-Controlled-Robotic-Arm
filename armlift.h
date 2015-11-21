#ifndef __ARM__LIFT__
#define __ARM__LIFT__
#include "motor.h"
#include "touch.h"
//#include "connection.h"

class ArmLift{
	private:
	    Motor *liftMotor;
	    Touch *elevatorTouch;
		bool unlocked;
		
	public :
		ArmLift(Motor*,Touch*);
		ArmLift();
		
		void zeroEncoder();
		void raise(int);
		void lower(int);
		void stop();

		
		void unlockArm();
		void lockArm();
		bool isUnlocked();
		
};
#endif
