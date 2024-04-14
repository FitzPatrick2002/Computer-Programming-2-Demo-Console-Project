#include "Change_Command.h"

Change_Command::Change_Command(std::map<std::string, std::vector<std::string>>& swa, std::vector<Patient>* pat_vec_ptr) : switches_with_args(swa), patients(pat_vec_ptr) {

}

Change_Command::~Change_Command() {

}

void Change_Command::change_specific_personal_data(std::vector<patient_properites_enum>& props_to_change, std::string pesel) {

	// 1. Find the patient with specified pesel
	// 2. Input new data values and change them in the Patient structure in the program
	// 3. Save the new personal patients data into file

	int patient_num = 0;
	for (int i = 0; i < this->patients->size(); i++) {
		if ((*(this->patients))[i].get_pesel() == pesel) {
			patient_num = i;
			break;
		}
	}

	// 2. Inut new values
	for (auto& type : props_to_change) {
		std::string type_string = convert_pat_prop_enum_to_str(type);
		std::string val;

		std::cout << type_string << ": ";
		std::cin >> val;

		(*(this->patients))[patient_num].set_prop_based_on_string_name(type_string, val);
	}
	
	// 3. Save new data into file
	(*(this->patients))[patient_num].save_personal_data_to_file();
}

void Change_Command::change_specific_report(std::vector<Report> report, std::string pesel) {

}

void Change_Command::perform() {

	if (switches_with_args.size() == 0) {
		std::cerr << "Command 'change' requires switch -i combined with -r or -prop\n";
	}
	else {
		if (this->switches_with_args.find("-i") == this->switches_with_args.end()) {
			std::cerr << "Command 'change' requires the use of switch -i\n";
			return;
		}
		else {
			// Choose the patient on which we are operating (by providing a unique pesel numberpesel)
			std::string patient_pesel = "";

			Individual_Patient_Switch i_sw(patient_pesel, *(this->patients));
			i_sw.set_switch_data(this->switches_with_args["-i"]);

			if (patient_pesel != "") {
				if (this->switches_with_args.find("-prop") != this->switches_with_args.end()) {
					std::vector<patient_properites_enum> props_to_change;

					Prop_Switch prop_sw(props_to_change);
					prop_sw.set_switch_data(this->switches_with_args["-prop"]);

					this->change_specific_personal_data(props_to_change, patient_pesel);
				}
				else if (this->switches_with_args.find("-r") != this->switches_with_args.end()) {
					std::vector<Report> report_to_change;

					Report_Switch report_sw(patient_pesel, *(this->patients), report_to_change);
					report_sw.set_switch_data(this->switches_with_args["-r"]);

				}
				else {
					std::cerr << "Command 'change' requires switch -i combined with -r or -prop\n";
				}
			}
			else {
				std::cerr << "Pesel number has not been found by the -i switch\n";
			}
		}
	}

}
