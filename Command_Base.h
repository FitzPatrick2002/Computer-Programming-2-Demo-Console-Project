#ifndef COMMAND_BASE
#define COMMAND_BASE

#include <vector>
#include <string>

class Command_Base {
private:

public:

	Command_Base();

	virtual ~Command_Base();

	// This function is invoked when using a command. 
	// In this case command doesn't take any arguments, only switches
	virtual void perform() = 0;

};

#endif // !Command_Base
