#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <regex>
#include <map>

#include "Command_Base.h"
//#include "Patient.h"
#include "Command_List.h"
#include "Command_Add.h"
#include "Change_Command.h"

using u_int = unsigned int;
namespace fs = std::filesystem;

// Function for comparison of two strings alphabetically.
// It checks which string would be first if we were to list them by the first letters alphabetically
// It uses the standard ASCII char for comparison
// return true if the first one would go first nd false otherwise

bool check_alphabetical_order_of_strings(std::string s1, std::string s2) {
	if (s1 == s2)
		return true;

	int i = 0;
	while (s1[i] == s2[i]) {
		std::cout << "Comparing: " << s1[i] << s2[i] << "\n";
		i++;

	}

	char c1 = s1[i];
	char c2 = s2[i];

	return c1 < c2;
}

// Function for comparison of patients (eg. Age_1 > Age_2) (listing, etc...)
bool compare_patients(Patient& p1, Patient& p2, bool ascending, std::string prop) {
	bool outcome = true;

	//Transform the given property string to lowercase
	std::for_each(prop.begin(), prop.end(), [&](char& c) {
		c = std::tolower(c);
		});

	if (prop == "age") {
		outcome = p1.get_age() < p2.get_age();
	}
	else if (prop == "name") {
		outcome = check_alphabetical_order_of_strings(p1.get_name(), p2.get_name());
	}
	else if (prop == "second name" || prop == "second_name" || prop == "secondname") {
		outcome = check_alphabetical_order_of_strings(p1.get_second_name(), p2.get_second_name());
	}
	else if (prop == "pesel") {
		outcome = check_alphabetical_order_of_strings(p1.get_pesel(), p2.get_pesel());
	}
	else if (prop == "gender") {
		if (p1.get_gender() == "male")
			outcome = false;
	}
	else {
		throw std::exception("Unknown prop provided to function compare_patients()\n");
	}


	if (ascending)
		return outcome;
	else
		return !outcome;
}

// Depreciated (old version with only very basic functionality that doesn't support anything)
void download_patients_data(std::string directory, std::vector<Patient>& destination) {

	// 1. Download all patients data into a single vector
	// 2. Enter the program loop

	//1. Download the patients
	//		1.1 Count how many there is and create vector for storing them
	//		1.2 Open the patient file and save the data

	u_int patients_amount = 0;
	for (const auto& entry : fs::directory_iterator(directory)) {
		patients_amount++;
	}
	destination.clear();
	destination.shrink_to_fit();
	destination.reserve(patients_amount);

	for (const auto& entry : fs::directory_iterator(directory)) {
		//patient_file_names.emplace_back(entry.path().filename().string());
		std::string file_dir = entry.path().string();

		std::ifstream patient_file(file_dir);
		Patient p;

		// Read the patients data
		if (patient_file.is_open()) {
			std::string line;

			std::getline(patient_file, line);
			p.set_name(line);

			std::getline(patient_file, line);
			p.set_second_name(line);

			std::getline(patient_file, line);
			int age = std::atoi(line.c_str());
			p.set_age(age);

			destination.emplace_back(p);

			patient_file.close();
		}
	}
}

// Returns given string just with lowercase letters
std::string str_to_lower(std::string s) {
	std::for_each(s.begin(), s.end(), [&](char& c) {
		c = std::tolower(c);
		});

	return s;
}

// It takes vector of strings and merges given positions into a single string
std::string merge_vector_strings(std::vector<std::string> vec, int start, int end, std::string sep) {
	std::string outcome = "";
	for (int i = start; i < end - 1; i++) {
		outcome += vec[i];
		outcome += sep;
	}

	outcome += vec[end - 1];

	return outcome;
}

// IMPORTNAT: This comment contains old functions that were used to read data from files in order to create an instance of Patient
/*
// Depraciated and superseded by function from the Patient class
// 1. Open the file
// 2. Read line and covert it into a stringstream
// 3. Split stringstream into a vector (space as a separator)
// 4. Elements [0] (and sometimes [1]) of the vector specify which property of Patient class has been read
//		4.1. Sometimes names can be multi element (like the chinese ones Xiao Li ?) so we merge the following elements of the vector into a single string
void read_personal_patient_info(Patient& destination, std::string personal_data_dir) {
	std::string personal_dir = personal_data_dir + "\\Personal_Data.txt";
	std::ifstream personal_data_file(personal_dir);

	if (personal_data_file.is_open()) {
		std::string line;

		while (std::getline(personal_data_file, line)) {
			std::stringstream ss(line);
			std::vector <std::string> split_line;

			std::string temp;
			while (ss >> temp)
				split_line.push_back(temp);

			if (str_to_lower(split_line[0]) == "name:") {
				std::string name = merge_vector_strings(split_line, 1, split_line.size());
				destination.set_name(name);
			}
			else if (str_to_lower(split_line[0]) == "age:") {
				int age = std::atoi(split_line[1].c_str());
				destination.set_age(age);
			}
			else if (str_to_lower(split_line[0]) == "gender:") {
				std::string gender = split_line[1];
				destination.set_gender(gender);
			}
			else if (str_to_lower(split_line[0]) == "pesel:") {
				std::string pesel = merge_vector_strings(split_line, 1, split_line.size());
				destination.set_pesel(pesel);
			}
			else if (str_to_lower(merge_vector_strings(split_line, 0, 2)) == "secondname:") {
				std::string second_name = merge_vector_strings(split_line, 1, split_line.size());
				destination.set_second_name(second_name);
			}
			else {
				throw std::exception("Wrongly set property in one of patients files\n");
			}
		}
	}

}

//Completly replaced by the methode in Illness.h
// 1. Open the General.txt file
// 2. Keep on reading the lines until you encounter one that begins with 'duration:'
//		2.1. Split each line into separate pieces (space as separator), zero-th element decides what property of illness is being stored in the rest of the line
// 3. When we are reading duration time of illness it is set in a format of eg. dd-mm-yyyy : dd-mm-yyyy
//		Whether this format is applied is checked using regex
//		If it is applied we add given duration period to the Illness object and proceed to the next line
//		If not we log that we couldn;t read that data and proceed to the next line
void read_illness_general_data(Illness &destination, std::string illenss_path) {
	std::string general_path = illenss_path + "\\General.txt";

	std::ifstream general_data_file(general_path);

	if (general_data_file.is_open()) {
		std::string line;

		while (std::getline(general_data_file, line)) {
			std::stringstream ss(line);
			std::vector<std::string> split_line;

			std::string temp = "";
			while (ss >> temp)
				split_line.push_back(temp);

			if (str_to_lower(split_line[0]) == "name:") {
				std::string full_name = merge_vector_strings(split_line, 1, split_line.size());
				destination.set_name(full_name);
			}
			else if (str_to_lower(split_line[0]) == "status:") {
				std::string status = str_to_lower(split_line[1]);
				if (status == "ongoing")
					destination.set_status(true);
				else if (status == "cured")
					destination.set_status(false);
				else 
					throw std::exception("Status for an illness has been wrongly set. Available words: ongoing / cured");
			}
			else if (str_to_lower(split_line[0]) == "duration:") {
				// Read the duration of the illness
				// Each line contains the next period of being ill
				
				// Do the matches with REGEX
				std::string both_dates = merge_vector_strings(split_line, 1, split_line.size());

				std::string string_format = "(\\d\\d-\\d\\d-\\d{4})\\s*[:-~]\\s*(\\d\\d-\\d\\d-\\d{4}|-)";
				std::regex date_reg(string_format);
				std::smatch match;
				
				std::string start_date, end_date;

				line = merge_vector_strings(split_line, 1, split_line.size());
				do {
					if (std::regex_search(line, match, date_reg)) {
						if (match.size() > 2) {
							start_date = match[1];
							end_date = match[2];

							std::pair<std::string, std::string> illness_period_duriation = std::make_pair(start_date, end_date);
							destination.add_duration(illness_period_duriation);
						}
						else
							std::cerr << "Could not load one of the illness durations. Regex found pattern too short.\n";
					}
					else
						std::cerr << "Regex didn't match anything in the General.txt for an illness. Problem with Duration:\n";

				} while (std::getline(general_data_file, line));
			}
		}
		destination.print_duration_history();
	}
}

void read_illness_reports(Illness& destination, std::string illness_path) {
	std::string illness_reports_path = illness_path + "\\Reports";

	// Count the reports and reserve space for them
	int reports_amount = 0;
	for (const auto& elem : fs::directory_iterator(illness_reports_path))
		reports_amount++;

	destination.reserve_space_for_reports(reports_amount);

	// Read each report file
	for (const auto& elem : fs::directory_iterator(illness_reports_path)) {
		std::string report_path = elem.path().string();
		std::ifstream report_file(report_path);

		Report report(report_path);

		report.read_report_file();
		destination.emplace_report(report);
	}
}

// 1. Count all the ilnesses of the patient and reserve space (have a separate vector for them)
// 2. Enter the particular illness folder
//		2.1. Read the General.txt
//		2.2. Enter the Reports folder
//			2.2.1. Read these repoerts into a vector of repeorts
void read_patients_illnesses(Patient& destination, std::string personal_data_dir) {
	std::string ilnesses_dir = personal_data_dir + "\\Illness";
	
	int ilnesses_count = 0;
	//Count these illnesses
	for (auto& elem : fs::directory_iterator(ilnesses_dir)) 
		ilnesses_count++;
	
	std::vector<Illness> all_illnesses(ilnesses_count);

	int i = 0;
	for (auto& elem : fs::directory_iterator(ilnesses_dir)) {
		std::string general_illness_data_dir = elem.path().string();

		all_illnesses[i].read_general_data(general_illness_data_dir);
		all_illnesses[i].read_all_reports(general_illness_data_dir);
		i++;
	}

	//Assign illnesses as ongoing or already cured

	for (auto& illness : all_illnesses) {
		if (illness.get_status())
			destination.push_ongoing_illness(illness);
		else
			destination.push_cured_illness(illness);
	}
}
*/

// Count all the elements found in a given folder, in this case we assume they were just patients. I would like to believe...
int count_pat(std::string patients_directory) {
	int count = 0;

	for (const auto& elem : fs::directory_iterator(patients_directory))
		count++;

	return count;
}

// Downloads all the patients data into a single vector
void download_all_patients(std::string patients_directory, std::vector<Patient>& all_patients) {
	
	u_int patients_amount = 0;
	for (const auto& patient_folder : fs::directory_iterator(patients_directory))
		patients_amount++;

	all_patients.clear();
	all_patients.shrink_to_fit();
	all_patients.reserve(patients_amount);

	int i = 0;
	// Now we are operating on folders that are in the patients_directory
	for (const auto& patient_folder : fs::directory_iterator(patients_directory)) {

		// 1. Read the Personal_Data.txt
		// 2. Read Hospital_Data.txt ---------------->>>>> UNIMPLEMENTED
		// 3. Go into the Illness subdirectory
		//		3.1. Read the General.txt
		//		3.2. Go inside the Reports subdirectory
		//			3.2.1. Read each file and save it

		fs::path patient_folder_dir = patient_folder.path(); //It includes name of this folder
		
		all_patients.emplace_back();
		all_patients[i].read_personal_data_from_file(patient_folder_dir.string());
		all_patients[i].read_patient_illnesses(patient_folder_dir.string());
		//read_personal_patient_info(all_patients[i], patient_folder_dir.string());
		//read_patients_illnesses(all_patients[i], patient_folder_dir.string());
		i++;
	}
}

void create_command(std::string command_name, Command_Base* comm, std::map<std::string, std::vector<std::string>>& switches) {

	if (command_name == "list") {
		delete comm;
		//comm = new Command_List(switches);
	}

}

int main() {

	try {
		//NOTE: This could be actually taken as an argument during opening of the .exe after all that program is just a way of manging some files and that is all...
		std::string all_patients_directory = "Patients\\";
		std::vector<Patient> all_patients;

		int amount_of_patients = count_pat(all_patients_directory);
		all_patients.reserve(amount_of_patients);

		//download_patients_data(all_patients_directory, all_patients);
		download_all_patients(all_patients_directory, all_patients);

		// List of available commands
		std::vector<std::string> available_command_names = { "list", "quit program", "add", "change", "delete"};

		Command_Base* command = nullptr;
		std::string user_input;
		while (user_input != "quit program") {
			std::cout << "User input: ";
			std::getline(std::cin, user_input);

			// If there was no input just pass
			if (user_input == "")
				continue;

			std::stringstream ss(user_input);
			std::string temp;
			std::string sw;
			std::string command_name;

			std::map<std::string, std::vector<std::string>> switches_with_args;

			ss >> command_name;
			while (ss >> temp) {
				// If the argument is a switch save it and use as a key in the map
				if (temp[0] == '-') {
					sw = temp;
					switches_with_args[sw]; // In case switch doesn't take any args -> we create a key in the map so as to mark that such switch was passed
					continue;
				}
				//std::cout << "sw: " << sw << ": \n";
				//std::cout << "arg: " << temp << "\n";

				switches_with_args[sw].push_back(temp);
			}

			// Usage of a command
			// 1. Create a command instance, give it switches and arguments to swithces
			// 2. Provide it with (for now) all available data (all_patients vector)
			// 3. ->perform() the command. Logic inside perform() does everything for us

			command = nullptr;
			if (std::find(available_command_names.begin(), available_command_names.end(), command_name) != available_command_names.end()) {
				
				if (command_name == "list") {
					command = new Command_List(switches_with_args, &all_patients);
					command->perform();

					delete command;
				}
				else if (command_name == "add") {
					command = new Command_Add(&all_patients, switches_with_args, all_patients_directory);
					command->perform();

					delete command;
				}
				else if (command_name == "change") {
					command = new Change_Command(switches_with_args, &all_patients);
					command->perform();

					delete command;
				}
				else if (command_name == "delete") {
					std::cerr << "Not implemented yet\n";
				}
				
				
			}
			else {
				std::cerr << "Unknown command used: " << command_name << "\n";
				std::cerr << "List of available commands: \n";
				for (const auto& c : available_command_names)
					std::cerr << c << "\n";

				std::cerr << "\n";
			}

		}
	}
	catch (std::exception& e) {
		std::cout << "Exception occured: \n";
		std::cout << e.what() << "\n";
	}

	return 0;
}