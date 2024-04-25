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

void Change_Command::change_reports(std::vector<Report*> reports, std::string pesel) {
	std::cout << "Reports found num : " << reports.size() << "\n";

	// 1. Iterate through the reports that has been found
	//		1.1 Find to which illness this report belongs. Get a refernce / pointer to this illness -->> ALready done by the Report_Switch
	//		1.2 Display the reports contents and ask the user if he wants to change it
	//			1.2.1 Yes -> Keep the report displayed and ask for the new report data title, author , content ... 
	//				  We are changing directly the report which is stoerd in a vector in the illness class instance
	//			1.1.2 No -> Clear the console. Go to 
	//		1.3 If changes has been done, save the new report in the illness reports vector
	//			Save this report into a .txt file (which already exists)
	//		1.4. Go to (1.)

	// 0. Get access to the patient with specified pesel
	// Is this 0. part even neededd anymore? We already havea pointer to the report after all...

	/*
	int patient_num = -1;
	for (int i = 0; i < this->patients->size(); i++) {
		Patient pat = (*(this->patients))[i];
		if (pat.get_pesel() == pesel) {
			patient_num = i;
			break;
		}
	}

	if (patient_num == -1) {
		std::cerr << "Somehow when calling 'change' -i for pesel: " << pesel << " patient could not be found\n";
		return;
	}

	Patient& pat = (*(this->patients))[patient_num];
	*/

	// We have pointer to the specified report. Now we just display original stuff and ask how do we wish it to be modified
	for (auto& rep_ptr : reports) {

		// 1. Display report
		std::cout << "Original content: \n";
		rep_ptr->print_all_data();
		std::cout << "If you don't want to modify the field just type in -\n\n";

		std::vector<std::string> rep_fields = { "Title: ", "Author: ", "Date: ", "Content: " };
		std::vector<std::string> fields_vals(4);

		// Get the new values of the report property
		for (int i = 0; i < rep_fields.size()-1; i++) {
			std::string val = "";

			std::cout << rep_fields[i];
			std::getline(std::cin, val);
			fields_vals[i] = val;
		}

		std::stringstream ss;
		while (true) {
			std::string val = "-";
			std::getline(std::cin, val);
			
			if (val == "-")
				break;

			ss << val;
		}

		fields_vals[3] = ss.str();

		// Now check which properties are to be changed and saved

		if (fields_vals[0] != "-")
			rep_ptr->set_title(fields_vals[0]);

		if (fields_vals[1] != "-")
			rep_ptr->set_author(fields_vals[1]);

		if (fields_vals[2] != "-")
			rep_ptr->set_date(fields_vals[2]);

		if (fields_vals[3] != "-")
			rep_ptr->set_content(fields_vals[3]);

		// Save the changes to file
		rep_ptr->save_current_data();
	}

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
					std::vector<Report> dummy;

					std::vector<Report*> report_to_change;

					Report_Switch report_sw(patient_pesel, *(this->patients), dummy, report_to_change);
					report_sw.set_switch_data_as_ptr(this->switches_with_args["-r"], true);

					this->change_reports(report_to_change, patient_pesel);
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
