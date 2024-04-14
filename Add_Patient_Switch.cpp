#include "Add_Patient_Switch.h"

Add_Patient_Switch::Add_Patient_Switch(bool& no_a_p) : no_args_passed(no_a_p) {

}

Add_Patient_Switch::~Add_Patient_Switch() {

}

void Add_Patient_Switch::set_switch_data(std::vector<std::string>& arguments) {
	if (arguments.size() != 0)
		no_args_passed = false;
	else
		no_args_passed = true;
}