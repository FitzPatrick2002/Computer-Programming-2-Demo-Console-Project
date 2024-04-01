#ifndef BASE_SWITCH
#define BASE_SWITCH

#include <iostream>
#include <vector>
#include <string>

// The idea of a switch class is as such that:
// Every Command before is perfomed has to establish how to perform itself
// This requires setting of some booleans and other values 
// We pass some of these values into a specific switch class by reference (in the constructor)
// Then instance of this switch modifies these variables with it's ->set_switch_data(...);

class Base_Switch {
private:

public:

	Base_Switch();

	~Base_Switch();

	// It takes the arguments passed to the switch
	virtual void set_switch_data(std::vector<std::string>& arguments) = 0;

};

#endif
