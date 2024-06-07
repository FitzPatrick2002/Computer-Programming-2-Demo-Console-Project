#include "All_Switch.h"

All_Switch::All_Switch() {

}

All_Switch::~All_Switch() {

}

void All_Switch::set_switch_data(std::vector<std::string>& arguments) {

	// If size == 1 then it means we have only one switch that being -a
		if (arguments.size() != 0) 
			throw std::exception("Switch -all for the commant list does not take any arguments\n");

}
