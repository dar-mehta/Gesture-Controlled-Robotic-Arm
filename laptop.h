#ifndef __LAPTOP__
#define __LAPTOP__

#include <iostream>
using namespace std;
class Laptop{
	public:
		
	Laptop();

	Laptop(string n0, int p0);
	
	int getPort() const;
	
	void setPort(int p0);
	
	string getName() const;
	
	void setName(string n0);
	
	private:
		int port;
		string name;
	};
#endif

