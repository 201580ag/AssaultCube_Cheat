#pragma once
#include <iostream>
class Debugger
{
private:
	FILE* f;
	bool debuggerStatus = false;
public:
	template<typename T>
	void LOG(T msg, bool endL = true) {
		if (this->debuggerStatus) {
			if (endL)
				std::cout << msg << std::endl;
			else
				std::cout << msg;
		}
	}
	bool getState();
	void Toggle();
	~Debugger();
};

