#ifndef __CLAW__
#define __CLAW__
#include "motor.h"
//#include "connection.h"
/*enum motorPort{
	A = OUT_A;
	B = OUT_B;
};*/
class Claw{
	public :
		Claw(Motor*, Motor*);
		
		void open(int);
		void close(int);
		bool isOpen();
		
		void rotateClaw(int);
		
		void unlockClaw();
		void lockClaw();
		bool isUnlocked();
		
		Motor *clawMotor;
		Motor *rotateClawMotor;
		bool unlocked, clawIsOpen;
};
#endif
