#include "Prop_Switch.h"


Prop_Switch::Prop_Switch(std::vector<patient_properites_enum> &ptp) : Base_Switch(), props_to_print(ptp) {

}

Prop_Switch::~Prop_Switch() {

}

void Prop_Switch::set_switch_data(std::vector<std::string>& arguments) {

	if (arguments.size() != 0) {
		props_to_print.reserve(arguments.size());

		for (auto prop : arguments)
			props_to_print.emplace_back(convert_pat_prop_str_to_enum(prop));
	}
	else {
		throw std::exception("Switch -prop for the command list expects at least one argument (name, second_name, age, pesel, gender)\n");
	}

	std::cout << "Props to print has been set: " << arguments.size() << "\n";
}