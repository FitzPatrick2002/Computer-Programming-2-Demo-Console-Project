#include "Order_Switch.h"

Order_Switch::Order_Switch(bool &ord_pr, bool &asc_pr, patient_properites_enum &pr_sort) : ordered_print(ord_pr), ascending_print(asc_pr), prop_to_sort_by(pr_sort) {

}

Order_Switch::~Order_Switch() {

}


void Order_Switch::set_switch_data(std::vector<std::string>& arguments) {
	this->ordered_print = true;

	// By default we have ascending order of printing
	// -order takes only zero, one or two arguments, that being (asc / desc), (propert) by which it is supposed to be sorted
	if (arguments.size() == 0)
		ordered_print = true;
	else {
		std::vector<std::string>::iterator it;

		//Check if one of the args is demandig descending printing
		if ((it = std::find(arguments.begin(), arguments.end(), "desc")) != arguments.end()) {
			ascending_print = false;
			arguments.erase(it);
		}
		else if ((it = std::find(arguments.begin(), arguments.end(), "asc")) != arguments.end())
			arguments.erase(it);

		arguments.shrink_to_fit();
		//std::cout << "Args -order size: " << arguments.size();

		// If it is specified then check by which property we are supposed to sort the patients
		if (arguments.size() > 0) {
			prop_to_sort_by = convert_pat_prop_str_to_enum(arguments[0]);

			if (prop_to_sort_by == patient_properites_enum::error)
				std::cerr << "Could not read the property by which the enum was supposed to be read: " << arguments[0] << "\n";
		}
	}
	//std::cout << "Order set: " << ordered_print << " " << ascending_print << (int)prop_to_sort_by << "\n";
}
