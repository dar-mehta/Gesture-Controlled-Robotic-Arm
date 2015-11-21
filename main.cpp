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

Connection *driveConnection = new Bluetooth();
Connection *armConnection = new Bluetooth();

Motor *motorLeftDrive = new Motor (OUT_C, driveConnection);
Motor *motorRightDrive = new Motor (OUT_A, driveConnection);

Motor *motorLiftArm = new Motor (OUT_B, driveConnection);
Motor *motorToggleClaw = new Motor (OUT_A, armConnection);
Motor *motorRotateClaw = new Motor (OUT_C, armConnection);

Drive drive(motorLeftDrive, motorRightDrive);
ArmLift arm (motorLiftArm);
Claw claw (motorToggleClaw, motorRotateClaw);

float motor_speed = 0;
const int longSpeed[15]={75, 40, 25, 15, 10, 5, 0, 0, 0, -5, -10, -15, -25, -40, -75};
const double latSpeed[9]={1,0.80,0.60,0,0,0,-0.60,-0.80,-1};

void selectPort(ifstream &in, int &drivePort, int &armPort){
	cout<<"\nSelect Connection Device"<<endl;
	cout<<"Port"<<setw(20)<<"Name"<<endl;
	
	string n0;
	int p0;
	
	while(in >> p0 >> n0){
		cout<<setw(4)<<p0<<setw(20)<<n0<< endl;
	}
	
	cout << "\nEnter the drive and arm com ports you would like to use " << endl;
	cout << "from the above list or any other port you would like to use." << endl;
	cin >> drivePort >> armPort;
}

class DataCollector : public myo::DeviceListener {
private:
		
		boolean armOn, driveOn, controllingDrive, controllingClaw, onInit;
		int yawInit, index, lMotorSpeed, rMotorSpeed;
		char ch;
			
public:
    DataCollector()
    : onArm(false), isUnlocked(false), roll_w(0), pitch_w(0), yaw_w(0), currentPose()
    {
    }
    
    void initialize(){
    	onInit = true;
    	armOn = false;
    	driveOn = false;
    	controllingDrive = false;
    	controllingClaw = false;
    	index=4;
    	yawInit=0;
    	lMotorSpeed=0;
    	rMotorSpeed = 0;
	}
	
    void onUnpair(myo::Myo* myo, uint64_t timestamp)
    {
        roll_w = 0;
        pitch_w = 0;
        index = 4;
        yaw_w = 0;
        onArm = false;
        isUnlocked = false;
    }
	
	//unimplimented
	void rollSpeed(float roll){
		roll_w = static_cast<int>((roll + (float)M_PI/2.0f)/M_PI * 18);
		if(!arm.isUnlocked() && !controllingDrive){
			cout<<"int-cast roll reading:"<<roll_w<<endl;
		}
	}
	
	void longitudinalSpeed (float pitch){
		int pitch_w = static_cast<int>((pitch + (float)M_PI/2.0f)/M_PI * 18);
		//if driving and drive is unlocked
		if (controllingDrive && drive.isUnlocked()){
			lMotorSpeed = rMotorSpeed = longSpeed[pitch_w];
		} else if (!controllingDrive && arm.isUnlocked()){
			arm.raise(longSpeed[pitch_w]);
		}
	}
	
	void lateralSpeed (float yaw){
		int yaw_w = static_cast<int>((yaw + (float)M_PI)/(M_PI * 2.0f) * 18);
		if(onInit){
			yawInit = yaw_w;
			onInit=false;
		}
		
		if (controllingDrive && drive.isUnlocked()){
	        index = yaw_w - yawInit + 4;
	        lMotorSpeed+=(100-fabs(lMotorSpeed))*(latSpeed[index]);
	        rMotorSpeed+=-(100-fabs(rMotorSpeed))*latSpeed[index];
		}
	}
	
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
    {
        using std::atan2;
        using std::asin;
        using std::sqrt;
        using std::max;
        using std::min;

        float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
                           1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
        float pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
        float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
                        1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
                     
		
		longitudinalSpeed(pitch);
		lateralSpeed(yaw);
		
		
		drive.forward(lMotorSpeed, rMotorSpeed);
        /*roll_w = static_cast<int>((roll + (float)M_PI)/(M_PI * 2.0f) * 18);
       	pitch_w = static_cast<int>((pitch + (float)M_PI/2.0f)/M_PI * 18);
        yaw_w = static_cast<int>((yaw + (float)M_PI)/(M_PI * 2.0f) * 18); 
		index = yaw_w-yawInit+4;*/  
    }

    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
    {
        currentPose = pose;

        if (pose != myo::Pose::unknown && pose != myo::Pose::rest) {
            myo->unlock(myo::Myo::unlockHold);
			
			if (!controllingDrive && !arm.isUnlocked()){   
				if(pose == myo::Pose::fingersSpread){
					cout<<"Confirm Open Claw Request: "<<endl;
					switch (getch()){
						case 'y':
							claw.open();
							break;
						case default:
							std::cout << "Cancelling open claw request." << endl;
							break;	
					}
				} else if(pose == myo::Pose::fist){	
					cout<<"Closing Claw";
					claw.close();
				}
			}
			
			if (pose == myo::Pose::fist && controllingClaw claw.isunlocked()){
				
			}
			
			
			if (pose == myo::Pose::doubleTap){
				myo->lock();
			}

            myo->notifyUserAction();
        } else {
            myo->unlock(myo::Myo::unlockHold);
        }
    }

    void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
                   myo::WarmupState warmupState)
    {
        onArm = true;
        whichArm = arm;
    }

    void onArmUnsync(myo::Myo* myo, uint64_t timestamp)
    {
        onArm = false;
    }

    void onUnlock(myo::Myo* myo, uint64_t timestamp)
    {
        isUnlocked = true;
    }

    void onLock(myo::Myo* myo, uint64_t timestamp)
    {
        isUnlocked = false;
    }

    void print()
    {
        string controllingNow;
		std::cout << '\r';
        if (controllingDrive){
			controllingNow = "drive";
		} else {
			controllingNow = "arm"; 
		}
        std::cout   //<< '[' << std::string(roll_w, 'R') << std::string(18 - roll_w, ' ') << ']'
//                  	<< '[' << std::string(pitch_w, 'P') << std::string(18 - pitch_w, ' ') << ']'
//                  	<< '[' << std::string(yaw_w, 'Y') << std::string(18 - yaw_w, ' ') << ']'
                  	<< '[' << controllingNow << index << ": " << latSpeed[index] << std::string(18 - index, ' ') << ']'
                  	<< '[' << controllingNow << pitch_w << ": " << longSpeed [pitch_w] << std::string(18 - pitch_w, ' ') << ']';
//                  << '[' << motorSpeed[pitch_w] << std::string(18 - pitch_w, ' ') << ']'
//                  << '[' << motor_speed << std::string(18 - pitch_w, ' ') << ']';

        if (onArm) {
            std::string poseString = currentPose.toString();

            std::cout << '[' << (isUnlocked ? "unlocked" : "locked  ") << ']'
//                      << '[' << (whichArm == myo::armLeft ? "L" : "R") << ']'
                      << '[' << poseString << std::string(14 - poseString.size(), ' ') << ']';
        } else {
//            std::cout << '[' << std::string(8, ' ') << ']' << "[?]" << '[' << std::string(14, ' ') << ']';
        }

        std::cout << std::flush;
    }

    bool onArm;
    myo::Arm whichArm;

    bool isUnlocked;

    int roll_w, pitch_w, yaw_w;
    myo::Pose currentPose;
};



int main(int argc, char** argv)
{
	ifstream comIn ("comports.txt");
	int driveCom = 0, armCom = 0;
	selectPort(comIn, driveCom, armCom);
	
	try {
	
	cout << "Trying to connect" << endl;
    driveConnection->connect(driveCom);
    armConnection->connect(armCom);
    cout << "Connected" << endl;
    claw.initialize();
	
    myo::Hub hub("com.example.hello-myo");

    std::cout << "Attempting to find a Myo..." << std::endl;

    myo::Myo* myo = hub.waitForMyo(10000);

    if (!myo) {
        throw std::runtime_error("Unable to find a Myo!");
    }

    std::cout << "Connected to a Myo armband!" << std::endl << std::endl;

    DataCollector collector;
	collector.initialize();
    hub.addListener(&collector);

    while (1) {
        hub.run(1000/15);
        collector.print();
        if (kbhit()){
        	char ch = getch();
			switch (ch){
				case 32:
					if(driveOn==armOn){
						driveOn=!driveOn;
					}
					driveOn = !driveOn;f
					std::cout << std::boolalpha << "Drive Control: " << driveOn << std::endl;
					armOn = !armOn;
					std::cout << std::boolalpha << "Arm Control: " << armOn << std::endl;
				 	break;
				case 'a':
					controllingArm = true;
					controllingDrive = false;
					controllingClaw = true;
					std::cout << std::boolalpha << "Controlling Arm!" << std::endl;
					break;
				case 's':
					controllingArm = false;
					controllingDrive = true;
					controllingClaw = true;
					std::cout << std::boolalpha << "Controlling Claw!" << std::endl;
				 	break;
				case 'd':
					controllingDrive = true;
					controllingArm = false;
					controllingClaw = true;
					std::cout << std::boolalpha << "Controlling Drive!" << std::endl;
				 	break;
				case 'k':
					killRobot();
			}	
		}
    }
	//driveConnection->disconnect();
	//armConnection->disconnect();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Press enter to continue.";
        std::cin.ignore();
        drive.forward(0, 0);
        driveConnection->disconnect();
        armConnection->disconnect();
        return 1;
    }
}

