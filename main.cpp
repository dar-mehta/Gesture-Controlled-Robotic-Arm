// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Distributed under the Myo SDK license agreement. See LICENSE.txt for details.
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <algorithm>

#include <myo.hpp>
#include "controller.h"
//#include "datacollector.h"

#include <cstdlib>
#include <conio.h>
#include "nxt.h"
#include <windows.h>
#include <stdlib.h>
#include <fstream>
#include "drive.h"
#include "armlift.h"
#include "claw.h"
//#include "laptop.h"

//Connection *driveConnection = new Bluetooth();
//Connection *armConnection = new Bluetooth();
//
//Motor *motorLeftDrive = new Motor (OUT_C, driveConnection);
//Motor *motorRightDrive = new Motor (OUT_A, driveConnection);
//
//Motor *motorLiftArm = new Motor (OUT_B, driveConnection);
//Motor *motorToggleClaw = new Motor (OUT_A, armConnection);
//Motor *motorRotateClaw = new Motor (OUT_C, armConnection);
//
//Touch *elevatorTouch = new Touch(IN_1, driveConnection);
//
//Drive drive(motorLeftDrive, motorRightDrive);
//ArmLift arm (motorLiftArm, elevatorTouch);
//Claw claw (motorToggleClaw, motorRotateClaw);





int main(int argc, char** argv)
{

	Controller robotControl;
	robotControl.setConnections();
	/*if(!robotControl.setConnections()){
		cout << "FLAGGGG" << endl;
		return -1;
	}*/
	cout << "FALGG" << endl;
	robotControl.runRobot();
	return 0;
}

