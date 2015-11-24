#ifndef __CONTROLLER__
#define __CONTROLLER__

#include <fstream>
#include "drive.h"
#include "armlift.h"
#include "claw.h"
#include "nxt.h"
#include "datacollector.h"
#include <iomanip>
#include <conio.h>
#include "motor.h"

class Controller{
	private:
	    Connection *driveConnection;
	    Connection *armConnection;
			
	    Motor *motorLeftDrive;
		Motor *motorRightDrive;
		Motor *motorLiftArm;
		
		Motor *motorToggleClaw;
		Motor *motorRotateClaw;
		
		Touch *elevatorTouch;
		
		Drive drive;
		ArmLift arm;
		Claw claw;
		int rollInit,yawInit, target, lMotorSpeed,rMotorSpeed;
		bool controllingDrive, controllingArm, controllingClaw,
  			 onRollInit, onYawInit, systemStatus;
			 
		   myo::Hub hub;
	public:
		
		int index;
		
		
		Controller();
	
		void selectPort(ifstream  &, int &, int &);
	
	
		void setConnections();
	    
	    int getControlSystem();
	   
	    void setRoll(int);
		
		void setPitch (int);
		
		void setYaw (int);
	
		void setPose(int);
	   
	   	void runRobot();
		   
		bool getSystemStatus();	

	
};

#endif
