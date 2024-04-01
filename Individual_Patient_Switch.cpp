#include "Individual_Patient_Switch.h"

Individual_Patient_Switch::Individual_Patient_Switch(std::string& i_p, std::vector<Patient>& vec) : Base_Switch(), i_pesel(i_p), all_patients(vec){

}

Individual_Patient_Switch::~Individual_Patient_Switch() {

}

// Check if given string is ccomposed of numbers and is 11 characters in length
bool check_if_pesel_correct(std::string pesel) {
	std::regex pattern("(\\d{11})");

	if (std::regex_search(pesel, pattern))
		return true;

	return false;
}

// Checks if the amount of given args is correct and if the pesel provided is in it's correct form.
// Check if such patient even exist

void Individual_Patient_Switch::set_switch_data(std::vector<std::string>& arguments) {

	if (arguments.size() != 1) {
		std::cerr << "Switch -i takes always exactly one argument, that is the pesel of the patient\n";
	}
	else {
		// Check if PESEL is in correct form
		std::string given_pesel = arguments[0];
		if (check_if_pesel_correct(given_pesel)) {
			
			// Check if patient with such pesel exist
			// If so end the function
			for(const auto & pat : all_patients)
				if (pat.get_pesel() == given_pesel) {
					this->i_pesel = given_pesel;
					return;
				}
			std::cerr << "None patient posses given pesel: " << given_pesel << "\n";
		}
		else
			std::cerr << "Given Pesel is not valid: " << given_pesel << " Error when using -i switch\n";
	}
}