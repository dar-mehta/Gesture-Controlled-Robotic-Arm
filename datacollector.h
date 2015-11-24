#ifndef __DATA__COLLECTOR__
#define __DATA__COLLECTOR__

#include <myo.hpp>
#include "controller.h"

class Controller;
class DataCollector : public myo::DeviceListener {
private:
		
		Controller *controller;
		int controlSystem;
		bool systemUnlocked;
			
public:
	DataCollector();
    
    void initialize(Controller *control);
	
    void onUnpair(myo::Myo* myo, uint64_t timestamp);
	
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat);
    
    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose);

    void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
                   myo::WarmupState warmupState);

    void onArmUnsync(myo::Myo* myo, uint64_t timestamp);

    void onUnlock(myo::Myo* myo, uint64_t timestamp);

    void onLock(myo::Myo* myo, uint64_t timestamp);

    void print();

    bool onArm;
    myo::Arm whichArm;

    bool isUnlocked;

    int roll_w, pitch_w, yaw_w;
    float roll, pitch, yaw;
    myo::Pose currentPose;
};

#endif
