#include "Obtain_Illness_Switch.h"

Obtain_Illness_Switch::Obtain_Illness_Switch(Patient* patr, Illness*& ill) : pat_ptr(patr), ill_ptr(ill){

}

Obtain_Illness_Switch::~Obtain_Illness_Switch() {

}

void Obtain_Illness_Switch::set_switch_data(std::vector<std::string>& args) {
	if (args.size() == 1) {
		this->ill_ptr = nullptr;

		// 1. Extract name value of this illness
		// 2. Check if this patient has it
		// 3. If so, set the pointer to it's instacne in one of the illnesses vector fo this patient

		std::string name = args[0];
		for (auto& c : name)
			c = std::tolower(c);

		for (auto& ill : this->pat_ptr->access_cured_illnesses()) {
			std::string temp_ill_name;

			for (auto& c : temp_ill_name)
				c = std::tolower(c);

			if (temp_ill_name == name) {
				this->ill_ptr = &ill;
				return;
			}
		}

		// Check the on going illnesses

		for (auto& ill : this->pat_ptr->acces_on_going_illnesses()) {
			std::string temp_ill_name;

			for (auto& c : temp_ill_name)
				c = std::tolower(c);

			if (temp_ill_name == name) {
				this->ill_ptr = &ill;
				return;
			}
		}

		if (this->ill_ptr == nullptr)
			std::cerr << "Specified illness: " << name << " has not been found for the patient of pesel: " << this->pat_ptr->get_pesel() << "\n";
	}
	else {
		std::cerr << "Usage of switch -oil: -oil 'name of illness'\n";
	}
}