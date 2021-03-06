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
		Claw();
		Claw(Motor*, Motor*);
		
		void initialize();
		
		void open();
		void close();
		
		void rotate(int);
		
		void unlockClaw(bool);
		bool isUnlocked();
		
		
		
		Motor *clawMotor;
		Motor *rotateClawMotor;
		bool unlocked, isOpen, rotated;
		int initClaw, initRotation, i,j;
		long rotation;
		long int clawEncoder, rotationEncoder, initClawEncoder, initRotateEncoder;
};
#endif
