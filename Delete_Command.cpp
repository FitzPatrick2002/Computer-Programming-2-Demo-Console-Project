#include "Delete_Command.h"

Delete_Command::Delete_Command(std::vector<Patient>* all_patients, std::map<std::string, std::vector<std::string>>& swa) : Command_Base(), patients(all_patients), switches_with_args(swa) {

}

Delete_Command::~Delete_Command() {

}

void Delete_Command::delete_patient(Patient* p) {

	// 1. Change folders name of all patients that are 'after' this one
	// 2. Delte this patient from the all_patients vector
	// 3. Delete him from the filesystem

	// 1. Rename patients below this one
	//		Find the path relative to the main.cpp
	//		Iterate through the patients folders nad rename them
	//		Iterate through patients in the patient vector and change their directory names

	std::string patients_folder_dir = p->get_directory_name();
	int separator_pos = patients_folder_dir.find("Patient_");
	patients_folder_dir = patients_folder_dir.substr(0, separator_pos - 1);

	bool change_below = false;
	int j = 1;
	for (auto& elem : std::filesystem::directory_iterator(patients_folder_dir)) {
		std::string pat_dir = elem.path().string(); // Get the current name

		// Change stuff only below the patient that we want to delete
		if (pat_dir == p->get_directory_name()) {
			change_below = true;

			//3. Delete him from the filesystem
			std::filesystem::remove_all(p->get_directory_name());

			continue;
		}

		if (change_below) {
			std::string num = std::to_string(j); // Get the current num number for that pateint and decrease by one
			int underscore_pos = pat_dir.find_last_of("_"); // Find the "_" which lies before the bumber part

			std::string new_name = pat_dir.substr(0, underscore_pos) + "_0" + num; // Set new name
			std::filesystem::rename(elem.path(), std::filesystem::path(new_name)); // Change name of this path

			// Find patient with the original path in the all_patients vector and set a new path for him
			for (auto& pat : *this->patients) {
				if (pat.get_directory_name() == pat_dir)
					pat.set_directory_name(new_name);
			}
		}

		j++;
	}


	// 2. Delte this patient from the all_patients vector
	//this->patients->erase(std::remove(this->patients->begin(), this->patients->end(), *p), this->patients->end());

	std::vector<Patient>::iterator itr = this->patients->begin();
	for (itr; itr != this->patients->end(); itr++) {
		if (itr->get_pesel() == p->get_pesel())
			break;
	}
	this->patients->erase(itr);

}

void Delete_Command::perform() {

	if (this->switches_with_args.size() == 0) {
		std::cerr << "Not provided what is to be deleted\n";
	}
	else if (this->switches_with_args.size() == 1) {
		if (this->switches_with_args.find("-i") != this->switches_with_args.end()) {
			std::string dummy;
			Patient* pat_for_del = nullptr;

			Individual_Patient_Switch i_sw(dummy, *this->patients);
			i_sw.set_switch_data(this->switches_with_args["-i"], pat_for_del);
			
			this->delete_patient(pat_for_del);
		}
		else
			std::cerr << "Command 'delete' takes only one switch, that is -i\n";
	}
	else
		std::cerr << "Command 'delete' takes only one switch: -i\n";

}