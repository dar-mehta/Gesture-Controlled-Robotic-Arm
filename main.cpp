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
//#include "laptop.h"

using namespace std;

Connection *connection = new Bluetooth();

Motor *motorA = new Motor (OUT_A, connection);
Motor *motorC = new Motor (OUT_C, connection);
Drive drive(motorA, motorC);

int selectPort(ifstream &in, int &port){
	cout<<"\nSelect Connection Device"<<endl;
	cout<<"Port"<<setw(20)<<"Name"<<endl;
	
	string n0;
	int p0;
	
	while(in >> p0 >> n0){
		cout<<setw(4)<<p0<<setw(20)<<n0<< endl;
	}
	
	cout << "\nEnter the com port you would like to use from the " << endl;
	cout << "above list or any other port you would like to use." << endl;
	cin>>port;
	return port;
}

class DataCollector : public myo::DeviceListener {
public:
    DataCollector()
    : onArm(false), isUnlocked(false), roll_w(0), pitch_w(0), yaw_w(0), currentPose()
    {
    }

    void onUnpair(myo::Myo* myo, uint64_t timestamp)
    {
        roll_w = 0;
        pitch_w = 0;
        yaw_w = 0;
        onArm = false;
        isUnlocked = false;
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

        roll_w = static_cast<int>((roll + (float)M_PI)/(M_PI * 2.0f) * 18);
        pitch_w = static_cast<int>((pitch + (float)M_PI/2.0f)/M_PI * 18);
        yaw_w = static_cast<int>((yaw + (float)M_PI)/(M_PI * 2.0f) * 18);
    }

    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
    {
        currentPose = pose;

        if (pose != myo::Pose::unknown && pose != myo::Pose::rest) {
            myo->unlock(myo::Myo::unlockHold);
            if (pose == myo::Pose::fist){
            	drive.forward(-100);
            	cout << "FIST DRIVE FORWARD -100" << endl;
			} else if (pose == myo::Pose::fingersSpread){
				drive.forward(100);
				cout << "FINGERSSPREAD DRIVE FORWARD 100" << endl;
			} else if (pose == myo::Pose::waveIn){
				drive.forward(0);
				cout << "REST DRIVE FORWARD 0" << endl;
			}

            myo->notifyUserAction();
        } else {
            myo->unlock(myo::Myo::unlockTimed);
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
        std::cout << '\r';

        std::cout << '[' << std::string(roll_w, '*') << std::string(18 - roll_w, ' ') << ']'
                  << '[' << std::string(pitch_w, '*') << std::string(18 - pitch_w, ' ') << ']'
                  << '[' << std::string(yaw_w, '*') << std::string(18 - yaw_w, ' ') << ']';

        if (onArm) {
            std::string poseString = currentPose.toString();

            std::cout << '[' << (isUnlocked ? "unlocked" : "locked  ") << ']'
                      << '[' << (whichArm == myo::armLeft ? "L" : "R") << ']'
                      << '[' << poseString << std::string(14 - poseString.size(), ' ') << ']';
        } else {
            std::cout << '[' << std::string(8, ' ') << ']' << "[?]" << '[' << std::string(14, ' ') << ']';
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
	int comPort = 0;
	comPort = selectPort(comIn, comPort);
	
	try {
	
	cout << "Trying to connect" << endl;
    connection->connect(comPort);
    cout << "Connected" << endl;
	
    myo::Hub hub("com.example.hello-myo");

    std::cout << "Attempting to find a Myo..." << std::endl;

    myo::Myo* myo = hub.waitForMyo(10000);

    if (!myo) {
        throw std::runtime_error("Unable to find a Myo!");
    }

    std::cout << "Connected to a Myo armband!" << std::endl << std::endl;

    DataCollector collector;

    hub.addListener(&collector);

    while (1) {
        hub.run(1000/20);
        collector.print();
    }
	connection->disconnect();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Press enter to continue.";
        std::cin.ignore();
        connection->disconnect();
        return 1;
    }
}

