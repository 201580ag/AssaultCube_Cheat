#include "debugger.h"
#include <Windows.h>

bool Debugger::getState() {
	return this->debuggerStatus;
}
void Debugger::Toggle() {
	if (!this->debuggerStatus) {
		this->debuggerStatus = true;
		AllocConsole();
		freopen_s(&f, "CONOUT$", "w", stdout);
		LOG("***CONSOLE DEBUGGER BY R1PERXNX***");
	}
	else {
		this->debuggerStatus = false;
		if (this->f != NULL) {
			fclose(f);
		}
		FreeConsole();
	}
}
Debugger::~Debugger() {
	if (this->debuggerStatus = true) {
		if (this->f != NULL) {
			fclose(this->f);
		}
		FreeConsole();
	}
}