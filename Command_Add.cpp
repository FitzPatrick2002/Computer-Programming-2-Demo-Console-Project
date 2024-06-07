#include "Command_Add.h"

Command_Add::Command_Add(std::vector<Patient>* pat_vec, std::map<std::string, std::vector<std::string>>& sw_w_arg, std::string pat_folder) : patients(pat_vec), switches_with_args(sw_w_arg), patients_folder_directory(pat_folder){

}

Command_Add::~Command_Add() {

}

// Creates the patient both in the files system and in the this->patients vector as well

void Command_Add::add_new_illness(std::string name, std::pair<std::string, std::string> duration, Patient* pat_ptr) {

	// 1. Prepare a new illness folder with the Reports sub-folder and the General.txt file
	//		1.1. Set the path for the illness folder
	// 2. Fill the General.txt with name, status and duration
	//		2.1. Depending on duration add this illness to appropriate patient vectro for stroing illnesses
	
	// 1. Create a new folder for this illness

	std::string new_illness_folder_dir = pat_ptr->get_directory_name();
	new_illness_folder_dir +=  "\\Illness\\" + name;  // "//" + name 
	std::filesystem::path new_illness_path(new_illness_folder_dir);

	new_illness_path.make_preferred();

	if (!std::filesystem::create_directory(new_illness_path)) {
		std::cerr << "Could not create a new illness folder at " + new_illness_folder_dir << "\n";
		return;
	}

	// Now prepare the folder, add the Reports folder inside it and the General.txt file

	std::string new_illness_reports_dir = new_illness_folder_dir + "\\Reports";

	if (!std::filesystem::create_directory(new_illness_reports_dir)) {
		std::cerr << "Could not create a Reports folder in this directory .\\" + new_illness_folder_dir << "\n";
		return;
	}


	// 2. Create a new General.txt file and save the provided data

	std::string general_file_dir = pat_ptr->get_directory_name() + "/Illness/" + name + "/General.txt";
	//Change '\\' -> '/'

	for (auto& c : general_file_dir)
		if (c == '/')
			c = '\\';
	std::ofstream general_file(general_file_dir);

	if (general_file.is_open()) {
		Illness ill;

		general_file << "Name: " << name << "\n";
		general_file << "Status: ";

		if (duration.second == "-") {
			general_file << "cured\n";
			ill.set_status(false);
		}
		else {
			general_file << "ongoing\n";
			ill.set_status(true);
		}

		general_file << "Duration: ";
		general_file << duration.first << "-" << duration.second << "\n";

		ill.add_duration(duration);

		general_file.close();

		// Now add the illness as a memeber of illness vector of the patient
		// And set it's path

		ill.set_name(name);
		std::string new_illness_path = pat_ptr->get_directory_name() + "/Illness/" + name;
		std::filesystem::path p(new_illness_path);
		p.make_preferred();

		ill.set_path(p.string());
		
		if (duration.second == "-")
			pat_ptr->push_ongoing_illness(ill);
		else
			pat_ptr->push_cured_illness(ill);

	}
	else
		std::cerr << "Could not create the file: " << general_file_dir << "\n";

	//Illness new_illness(name, illness_desc, duration, )

}

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


void Command_Add::add_new_report(std::string title, std::string author, std::string date, Illness* ill_ptr, Patient* pat_ptr){
	
	// 1. Get the desired content of the report
	// 2. Create new instance of the class Report and set all the data
	//		2.1. Set the file_location of the report
	// 3. Reserve space for new report in repoert vector of this illness
	//		3.1. Emplace the new report
	//4. Save report as a file
	
	std::string report_content;
	std::string line = "";
	std::cout << "Now provide the report content. To stop type in 'end' without '\n";
	while (line != "end") {
		report_content += line;
		report_content += "\n";
		std::getline(std::cin, line);
	}
	
	Report new_report;

	new_report.set_author(author);
	new_report.set_title(title);
	new_report.set_date(date);
	new_report.set_content(report_content);

	std::string path_to_new_report = ill_ptr->get_path() + "/Reports/" + title + ".txt";
	std::filesystem::path p(path_to_new_report);
	p.make_preferred();

	new_report.set_file_location(p.string());

	ill_ptr->reserve_space_for_reports(1);
	ill_ptr->emplace_report(new_report);

	new_report.save_current_data(); // We can use this instacne not the one that is in the vector, for they now have the same content and the same path
	
}

void Command_Add::add_new_illness_duration_period(Illness* ill_ptr) {

	// 1. Get new time period
	// 2. Check if it is in a correct form (regex)
	// 3. Check if the previous period has already ended
	// 4. If so add the new period to file and save

	std::cout << "Provide the new duration period in the form of dd.mm.yyyy-dd.mm.yyyy if it has ended or dd.mm.yyyy-- if it is still ongoing\n";

	std::string new_time_period;
	std::cin >> new_time_period;

	// 2. 

	std::regex time_reg("(\\d{2}.\\d{2}.\\d{4})[-](\\d{2}.\\d{2}.\\d{4}|-)");
	std::smatch m;

	// Check the added time period
	if (std::regex_match(new_time_period, m, time_reg)) {
		// Time period is weel-specified
		// Now check the last time period of that illness

		std::string t;
		std::vector <std::pair<std::string, std::string>> duration_history = ill_ptr->access_duration_history();
		t = duration_history[duration_history.size() - 1].second;


		if (t == "-") {
			// Illness is still ongoing and it doesn;t make sense to add a new period now
		}
		else {
			duration_history.push_back(std::pair<std::string, std::string>(m[1], m[2]));
		}
		
	}
	else {
		std::cerr << "Time period in a wrong format\n";
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
			// To a specific patient we can add an illness or a new report to a specific illness
			// Add illness -> -il
			// Add report -> -ar
			std::string dummy; //We won't be needing a specific value of pesel of this patient
			Patient* pat_ptr;

			// Switch validates whther pateitn with such pesel exist
			Individual_Patient_Switch individual_sw(dummy, *this->patients);
			individual_sw.set_switch_data(this->switches_with_args["-i"], pat_ptr); //We obtain a pointer to this specific patient

			if (this->switches_with_args.find("-il") != this->switches_with_args.end()) {
				std::string name;
				std::pair<std::string, std::string> duration;

				// Switch will validate whther given span of dates is okay or not okay, same for name of the illness
				Add_Illness_Switch illness_sw(name, duration, pat_ptr);
				illness_sw.set_switch_data(this->switches_with_args["-il"]);
				this->add_new_illness(name, duration, pat_ptr);
			}
			else if (this->switches_with_args.find("-ar") != this->switches_with_args.end()) {
				std::string title = "";
				std::string author = "";
				std::string date = "";
				Illness* illness_ptr = nullptr;
				
				Add_Reports_Switch add_report_sw(title, author, date, illness_ptr, pat_ptr);
				add_report_sw.set_switch_data(this->switches_with_args["-ar"]);
				this->add_new_report(title, author, date, illness_ptr, pat_ptr);
			}
			else if (this->switches_with_args.find("-oil") != this->switches_with_args.end()) {
				// Here we want to add another duration period to the illness
				Illness* ill_ptr;
				Obtain_Illness_Switch obtain_ill_sw(pat_ptr, ill_ptr);
				obtain_ill_sw.set_switch_data(this->switches_with_args["-oil"]); // Is that a mistake? iol -> oil

			}

		}
		else {
			std::cerr << "Invalid switches / arguemnts provided to the command 'add'. Available switches: -p -i -ar\n";
		}
	}
}