#include "controller.h"


	Controller::Controller(){
		
		
		driveConnection = new Bluetooth();
		armConnection = new Bluetooth();

		motorLeftDrive = new Motor (OUT_C, driveConnection);
		
		motorRightDrive = new Motor (OUT_A, driveConnection);
		
		motorLiftArm = new Motor (OUT_B, driveConnection);
		motorToggleClaw = new Motor (OUT_A, armConnection);
		motorRotateClaw = new Motor (OUT_C, armConnection);

		elevatorTouch = new Touch(IN_1, driveConnection);
		
		drive= Drive(motorLeftDrive, motorRightDrive);
		arm = ArmLift(motorLiftArm, elevatorTouch);
		claw = Claw(motorToggleClaw, motorRotateClaw);
		

		double latSpeed[9]={1,0.80,0.60,0,0,0,-0.60,-0.80,-1};
		int longSpeed[15]={75, 40, 25, 15, 10, 5, 0, 0, 0, -5, -10, -15, -25, -40, -75};
		
		target = 0;
		yawInit = 0;
		rollInit = 0;
		index = 0;
		lMotorSpeed = 0;
		rMotorSpeed = 0;
		controllingDrive = true;
    	controllingArm = false;
    	controllingClaw = false;
    	onRollInit = true;
    	onYawInit = true;

	}
	
	void Controller::selectPort(ifstream &in, int &drivePort, int &armPort){
	std::cout<<"\nSelect Connection Device"<<std::endl;
	std::cout<<"Port"<<setw(20)<<"Name"<<std::endl;
	
	string n0;
	int p0;
	
	while(in >> p0 >> n0){
		std::cout<<setw(4)<<p0<<setw(20)<<n0<< std::endl;
	}
	
	std::cout << "\nEnter the drive and arm com ports you would like to use " << std::endl;
	std::cout << "from the above list or any other port you would like to use." << std::endl;
	std::cin >> drivePort >> armPort;
	}
	
	void Controller::setConnections (){
		ifstream comIn ("comports.txt");
		int driveCom = 0, armCom = 0;
		selectPort(comIn, driveCom, armCom);
		try{
	
			std::cout << "Trying to connect" << std::endl;
			cout<<"Flag1";
	    	driveConnection->connect(driveCom);
	    	cout<<"Flag2";
	    	armConnection->connect(armCom);
	    	std::cout << "Connected" <<std::endl;
	    	
	    	arm.initialize();
			claw.initialize();	
	    	
	        myo::Hub hub("com.example.hello-myo");
	
	        std::cout << "Attempting to find a Myo..." << std::endl;
	
	        myo::Myo* myo = hub.waitForMyo(10000);
	
	    	if (!myo) {
	           throw std::runtime_error("Unable to find a Myo!");
	    	   }
	
	   	    std::cout << "Connected to a Myo armband!" << std::endl << std::endl;
	
		}
		catch (const std::exception& e) {
	        std::cerr << "Error: " << e.what() << std::endl;
	        std::cerr << "Press enter to continue.";
	        std::cin.ignore();
	        drive.unlockDrive(false);
	        arm.unlockArm(false);
	        driveConnection->disconnect();
	        armConnection->disconnect();
	        //return false;
    		}
 	    //return true;
    }
    
    int Controller::getControlSystem(){
    	
    	if(controllingDrive){
    		return 0;
		}
		if(controllingArm){
			return 1;
		}
		return 2;
							
	}
	
	bool Controller::getSystemStatus(){
		if(controllingDrive){
			systemStatus = drive.isUnlocked();
		}
		else if (controllingArm){
			systemStatus = arm.isUnlocked();
		}
		else
			systemStatus = claw.isUnlocked();
			
		return systemStatus;
	}
   
    void Controller::setRoll(int roll){
		if(onRollInit){
			rollInit = roll;
			onRollInit = false;
		}
		target = roll - rollInit;
		if(claw.isUnlocked())
		if(target<=2){
			claw.rotate(0);
		} else if (target <= 11){
			claw.rotate((target - 2) *12);
		}
	}
	
	void Controller::setPitch (int pitch){
		if (controllingDrive && drive.isUnlocked()){
			lMotorSpeed = rMotorSpeed = longSpeed[pitch];
		} else if (controllingArm && arm.isUnlocked()){
			arm.move(longSpeed[pitch]);
		}
	}
	
	void Controller::setYaw (int yaw){
		if(onYawInit){
			yawInit = yaw;
			onYawInit=false;
		}
        index = yaw - yawInit + 4;
        lMotorSpeed+=(100-fabs(lMotorSpeed))*(latSpeed[index]);
        rMotorSpeed+=-(100-fabs(rMotorSpeed))*latSpeed[index];
        
        if(controllingDrive){
        	drive.forward(lMotorSpeed, rMotorSpeed);
		}
        
	}

	void Controller::setPose(int pose){
		if(controllingClaw){
			if(pose == 0){
					std::cout<<"Confirm Open Claw Request: "<<std::endl;
					switch (getch()){
						case 'y':
							claw.open();
							break;
						default:
							std::cout << "Cancelling open claw request." << std::endl;
							break;	
					}
			} 
			else if(pose == 1){	
						std::cout<<"Closing Claw";
						claw.close();
			}
		}
	}
   
   	void Controller::runRobot(){
   	char ch;
   	cout << "FALG BEFORE" << endl;
   	DataCollector *collector = new DataCollector();
	collector->initialize(this);
   	cout << "FALG AFTER" << endl;
    hub.addListener(collector);
    while (1) {
        hub.run(1000/15);
        collector->print();
        if (kbhit()){
        	ch = getch();
			switch (ch){
				case 32:
					drive.unlockDrive(!drive.isUnlocked());
					arm.unlockArm(!arm.isUnlocked());
					claw.unlockClaw(!claw.isUnlocked());
					std::cout << std::boolalpha << "Drive Control: " << drive.isUnlocked() << std::endl;
					std::cout << std::boolalpha << "Arm Control: " << arm.isUnlocked() << std::endl;
				 	break;
				case 'a':
					controllingArm = true;
					controllingDrive = false;
					controllingClaw = false;
					std::cout << std::boolalpha << "Controlling Arm!" << std::endl;
					break;
				case 's':
					controllingArm = false;
					controllingDrive = false;
					controllingClaw = true;
					std::cout << std::boolalpha << "Controlling Claw!" << std::endl;
				 	break;
				case 'd':
					controllingArm = false;
					controllingDrive = true;
					controllingClaw = false;
					std::cout << std::boolalpha << "Controlling Drive!" << std::endl;
				 	break;
			}
   	
   }	

}
};