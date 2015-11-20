#ifndef __DRIVE__
#define __DRIVE__
#include "motor.h"
//#include "connection.h"
/*enum motorPort{
	A = OUT_A;
	B = OUT_B;
};*/
class Drive{
	public :
		Drive(Motor*, Motor*);
		
		void forward(int,int);
		void reverse(int);
		
		void turnLeft(int);
		void rotateCCW(int);
		
		void turnRight(int);
		void rotateCW(int);
		
		void unlockDrive();
		void lockDrive();
		
		Motor *leftDrive;
		Motor *rightDrive;
		bool unlocked;
};
#endif
