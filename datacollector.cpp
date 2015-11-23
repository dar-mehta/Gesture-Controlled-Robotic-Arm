#include "datacollector.h"
#include <myo.hpp>
#include "controller.h"
#include "armlift.h"
#include "claw.h"
#include "drive.h"

DataCollector::DataCollector()
: onArm(false), isUnlocked(false), roll_w(0), pitch_w(0), yaw_w(0), currentPose()
{
}

void DataCollector::initialize(Controller *control){
	 controller = control;
	 controlSystem = 0;
	 systemUnlocked = false;
	 
}

void DataCollector::onUnpair(myo::Myo* myo, uint64_t timestamp)
{
    roll_w = 0;
    pitch_w = 0;
    yaw_w = 0;
    onArm = false;
    isUnlocked = false;
}

void DataCollector::onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
{
    using std::atan2;
    using std::asin;
    using std::sqrt;
    using std::max;
    using std::min;
	controlSystem = controller->getControlSystem();
	systemUnlocked = controller->getSystemStatus();
	if (controlSystem==2 && systemUnlocked){
		float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
                       1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
		roll_w = static_cast<int>((roll + (float)M_PI/2.0f)/M_PI * 18);		   
		controller->setRoll(roll_w);	
	} else {
		float pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
		pitch_w = static_cast<int>((pitch + (float)M_PI/2.0f)/M_PI * 18);
		controller->setPitch(pitch_w);
	}
    if (controlSystem == 0 && systemUnlocked){
		float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
                    1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
		yaw_w = static_cast<int>((yaw + (float)M_PI)/(M_PI * 2.0f) * 18);
		controller->setYaw(yaw_w);
		}
}

void DataCollector::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
{
    currentPose = pose;

    if (pose != myo::Pose::unknown && pose != myo::Pose::rest) {
        myo->unlock(myo::Myo::unlockHold);
		
		if (controlSystem == 2&& systemUnlocked ){
		    if(pose == myo::Pose::fingersSpread){
		    	controller->setPose(0);
			}   
			else if(pose == myo::Pose::fist){
				controller->setPose(1);
			}
		}

        myo->notifyUserAction();
    } else {
        myo->unlock(myo::Myo::unlockHold);
    }
}

void DataCollector::onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
               myo::WarmupState warmupState)
{
    onArm = true;
    whichArm = arm;
}

void DataCollector::onArmUnsync(myo::Myo* myo, uint64_t timestamp)
{
    onArm = false;
}

void DataCollector::onUnlock(myo::Myo* myo, uint64_t timestamp)
{
    isUnlocked = true;
}

void DataCollector::onLock(myo::Myo* myo, uint64_t timestamp)
{
    isUnlocked = false;
}

void DataCollector::print()
{
    string controllingNow;
	std::cout << '\r';
    if (controlSystem == 0){
		controllingNow = "drive";
	} else {
		controllingNow = "arm"; 
	}
    std::cout 
              	<< '[' << controllingNow << controller->index << ": " << controller->latSpeed[controller->index] << std::string(18 - controller->index, ' ') << ']'
              	<< '[' << controllingNow << pitch_w << ": " << controller->longSpeed [pitch_w] << std::string(18 - pitch_w, ' ') << ']';

    if (onArm) {
        std::string poseString = currentPose.toString();

        std::cout << '[' << (isUnlocked ? "unlocked" : "locked  ") << ']'
                  << '[' << poseString << std::string(14 - poseString.size(), ' ') << ']';
    }

    std::cout << std::flush;
}



