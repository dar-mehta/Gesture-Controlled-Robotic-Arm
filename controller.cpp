#include "controller.h"
const double latSpeed[9]={0.6,0.40,0.20,0,0,0,-0.20,-0.40,-0.6};
const int longSpeed[15]={75, 40, 25, 15, 10, 5, 0, 0, 0, -5, -10, -15, -25, -40, -75};

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
	
	bool test(){
		
		std::clock_t start;
    	double duration = 0;
    	start = std::clock();
    	drive.forward(50,50);
    	int encoderA = drive.leftDrive->get_Encoder();
    	int encoderB = drive.rightDrive->get_Encoder();
    	while (duration < 3)
    	{
    	//test motot functionality by running. They should only run for the 3 seconds that this loop should last
    	
    	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		}		
		drive.forward(0,0);
		if(!(drive.leftDrive->get_Encoder() > encoderA && drive.rightDrive->get_Encoder >encoderB)){
			std::cout<< "Test failed at drive forward"<<endl;
			return false;
		}
		
		
		duration = 0;
		encoderA = drive.leftDrive->get_Encoder();
    	encoderB = drive.rightDrive->get_Encoder();
		start = std::clock();
		drive.forward(-50,-50);		
		while (duration < 3)
    	{
    	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		}
		drive.forward(0,0);
		if(!(drive.leftDrive->get_Encoder() < encoderA && drive.rightDrive->get_Encoder < encoderB)){
			std::cout<< "Test failed at drive backwards"<<endl;
			return false;
		}
		
		
		duration = 0;
		encoderA = arm.liftMotor->get_Encoder();
		start = std::clock();
		arm.move(50);
		while (duration < 3)
    	{
    	//test motot functionality by running. They should only run for the 3 seconds that this loop should last
    	
    	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		}		
		arm.move(0);
		if(arm.liftMotor->get_Encoder() > encoderA){
			std::cout<< "Test failed at lift arm"<<endl;
			return false;
		}
		
		
		duration = 0;
		encoderA = arm.liftMotor->get_Encoder();
		start = std::clock();
		arm.move(-50);
		while (duration < 3)
    	{
    	//test motot functionality by running. They should only run for the 3 seconds that this loop should last
    	
    	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		}		
		arm.move(0);
		if(arm.liftMotor->get_Encoder() < encoderA){
			std::cout<< "Test failed at lower arm"<<endl;
			return false;
		}
		
		duration = 0;
		encoderA = claw.rotateClawMotor->get_Encoder();
		start = std::clock();
		claw.rotate(100);
		while (duration < 3)
    	{
    	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		}		
		if(claw.rotateClawMotor->get_Encoder() > encoderA){
			std::cout<< "Test failed at rotateClaw"<<endl;
			return false;
		}
 		//return claw
		duration = 0;
		encoderA = claw.rotateClawMotor->get_Encoder();
		start = std::clock();
		arm.move(0);
		while (duration < 2)
    	{
    	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
		}		
	
		if(arm.liftMotor->get_Encoder() < encoderA){
			std::cout<< "Test failed at untip Claw"<<endl;
			return false;
			
		
		}
		
				  
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
			//cout<<"Flag1";
	    	driveConnection->connect(driveCom);
	    	//cout<<"Flag2";
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
	        driveConnection->disconnect();
	        armConnection->disconnect();
    		}
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
		//cout << "TARGET: " << target << endl;
		if(claw.isUnlocked())
		if(target<=2){
			claw.rotate(0);
		} else if (target <= 8){
			//cout<<"ENCODER TARGET"<< ((target - 2) *12)<<endl;
			claw.rotate((target - 2) *12);
		}
		else if(target > 8 <=11){
			claw.rotate(108);
		}
	}
	
	void Controller::setPitch (int pitch){
		if (controllingDrive && drive.isUnlocked()){
			lMotorSpeed = rMotorSpeed = longSpeed[pitch];
			//cout<<"PITCH:"<<pitch<<" SPEED"<<longSpeed[pitch] << endl;
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
        lMotorSpeed+=-(100-fabs(lMotorSpeed))*(latSpeed[index]);
        rMotorSpeed+=(100-fabs(rMotorSpeed))*(latSpeed[index]);
        
        
        if(controllingDrive){
        	drive.forward(lMotorSpeed, rMotorSpeed);
		}
        
	}

	void Controller::setPose(int pose){
		if(controllingClaw){
			if(pose == 0){
					std::cout<<"\n Confirm Open Claw Request: "<<std::endl;
					claw.rotateClawMotor->off();
					switch (getch()){
						case 'y':
							claw.open();
							break;
						default:
							std::cout << "\nCancelling open claw request." << std::endl;
							break;	
					}
			} 
			else if(pose == 1){	
						std::cout<<"Closing Claw"<<endl;
						claw.close();
						std::cout<<"Claw Closed"<<endl;;
			}
		}
	}
   
   	void Controller::runRobot(){
   	char ch;
   	//cout << "FALG BEFORE" << endl;
   	DataCollector collector;
	collector.initialize(this);
   	//cout << "FALG AFTER" << endl;
    hub.addListener(&collector);
    //cout << "FLAGGG" << endl;
    while (1) {
        hub.runOnce(1000/15);
        collector.print();
        if (kbhit()){
        	ch = getch();
			switch (ch){
				case 32:
					drive.unlockDrive(!drive.isUnlocked());
					arm.unlockArm(!arm.isUnlocked());
					claw.unlockClaw(!claw.isUnlocked());
					std::cout << std::boolalpha << "Drive Control: " << drive.isUnlocked() << std::endl;
					std::cout << std::boolalpha << "Arm Control: " << arm.isUnlocked() << std::endl;
					std::cout << std::boolalpha << "Claw Control: " << claw.isUnlocked() << std::endl;
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
