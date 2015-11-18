#ifndef __ARM__LIFT__
#define __ARM__LIFT__
#include "motor.h"
//#include "connection.h"
/*enum motorPort{
	A = OUT_A;
	B = OUT_B;
};*/
class ArmLift{
	public :
		ArmLift(Motor*);
		
		void raise(int);
		void lower(int);
		
		void unlockArm();
		void lockArm();
		bool isUnlocked();
		
		Motor *liftArm;
		bool unlocked;
};
#endif
