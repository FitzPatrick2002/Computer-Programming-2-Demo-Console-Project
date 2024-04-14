#include "Command_Add.h"

Command_Add::Command_Add(std::vector<Patient>* pat_vec, std::map<std::string, std::vector<std::string>>& sw_w_arg, std::string pat_folder) : patients(pat_vec), switches_with_args(sw_w_arg), patients_folder_directory(pat_folder){

}

Command_Add::~Command_Add() {

}

// Creates the patient both in the files system and in the this->patients vector as well

void Command_Add::add_new_patient(bool correctly_done_sw) {
	if (correctly_done_sw) {
		// Create the new patients folder, create and fill the Personal_Data.txt file and add the Illness folder
		// 1. Count the amount of patients
		// 2. Create name for the new folder
		// 3. Create the new folder
		// 4. Create .txt within it and a new directory as well within it

		int pat_amount = this->patients->size() + 1;
		std::string pat_amount_str;

		if(pat_amount < 10)
			pat_amount_str = "0" + std::to_string(pat_amount);
		else
			pat_amount_str = std::to_string(pat_amount);

		std::string new_folder_name = "Patient_" + pat_amount_str;
		std::string new_pat_dir = this->patients_folder_directory + new_folder_name;
		
		if (!std::filesystem::create_directory(new_pat_dir)) {
			std::cerr<<"Could not create a new Patient directory at: .\\" + this->patients_folder_directory + new_folder_name;
		}

		// 4. Create .txt within it and a new directory wit Illnesses as well within it
		
		std::filesystem::create_directory(this->patients_folder_directory + new_folder_name + "\\Illness");

		// Change '\\' into '/'
		for (auto& c : new_pat_dir)
			if (c == '\\')
				c = '/';
		
		std::ofstream pat_personal_data_file(new_pat_dir + "/Personal_Data.txt");
		if (pat_personal_data_file.is_open()) {
			// Get the personal data from the user
			std::string name;
			std::string second_name;
			std::string age;
			std::string pesel;
			std::string gender;

			std::cout << "Name: ";
			std::cin >> name;

			std::cout << "Second name: ";
			std::cin >> second_name;

			std::cout << "Age: ";
			std::cin >> age;

			std::cout << "PESEL: ";
			std::cin >> pesel;

			std::cout << "Gender: ";
			std::cin >> gender;

			name = "Name: " + name + "\n";
			second_name = "Second Name: " + second_name + "\n";
			age = "Age: " + age + "\n";
			pesel = "PESEL: " + pesel + "\n";
			gender = "Gender: " + gender + "\n";

			pat_personal_data_file << name;
			pat_personal_data_file << second_name;
			pat_personal_data_file << age;
			pat_personal_data_file << pesel;
			pat_personal_data_file << gender;

			pat_personal_data_file.close();

			// Add the new patient to the vector of patients
			Patient new_pat(new_pat_dir);
			this->patients->push_back(new_pat);
		}
		else {
			std::cerr << "An error occured, Personal_Data.txt for the new pateint has not been automatically created...\n";
		}


	}
	else {
		std::cerr << "Switch -p in the command 'add' does not take any arguemtns\n";
	}
}

void Command_Add::perform() {

	if (switches_with_args.size() == 0) {
		std::cerr << "Command 'list' cannot be called without any switches. Available switches: -p -i -ar";
	}
	else {
		// We are adding a new patient
		// Switch handles only checking if there are no args passed to switch -p (which doesn't take any arguments)
		if (this->switches_with_args.find("-p") != this->switches_with_args.end()) {
			bool add_patient_sw_correct = true;

			Add_Patient_Switch add_pat_sw(add_patient_sw_correct);
			add_pat_sw.set_switch_data(this->switches_with_args["-p"]);

			this->add_new_patient(add_patient_sw_correct);
		}
		else if (this->switches_with_args.find("-i") != this->switches_with_args.end()) {

		}
		else if (this->switches_with_args.find("-ar") != this->switches_with_args.end()) {

		}
		else {
			std::cerr << "Invalid switches / arguemnts provided to the command 'add'. Available switches: -p -i -ar\n";
		}
	}
}