#include "Command_List.h"

Command_List::Command_List(std::map<std::string, std::vector<std::string>>& swa, std::vector<Patient>* pat_vec_ptr) : Command_Base(), switches_with_args(swa){
	patients = pat_vec_ptr;
}

Command_List::~Command_List() {

}

// TO DO: Overload the [] for Patient class so that we can patient["name"] -> returns patients name filed value

// It prints all the patients data accoridng to passed switches. Called for the switch -all with additional switches
// Function called at the end of Command_List::perform()

void Command_List::list_sw_all(std::vector<patient_properites_enum> prop_print, bool order_pr, bool asc_pr, patient_properites_enum prop_sort) {

	// Check if we have to print in a specific order
	//		If yes sort the this->patients vector with std::qsort or std::sort
	//		Function passed for comparison will be defined later
	// If not just go through the this->patients and print the specific properties

	//std::cout << "Amount of props to print: " << prop_print.size() << "\n";
	if (order_pr == true) {
		std::string prop_to_sort_by = convert_pat_prop_enum_to_str(prop_sort);
		auto comparison = std::bind(compare_patients, std::placeholders::_1, std::placeholders::_2, asc_pr, prop_to_sort_by);
		std::sort(this->patients->begin(), this->patients->end(), comparison);

	}

	// If user has not specified what is to be printed, print everything...
	if (prop_print.size() == 0) {
		this->list_all();
		return;
	}
	
	for (auto& pat : *this->patients) {
		for (auto& prop : prop_print) {
			std::string prop_name = convert_pat_prop_enum_to_str(prop);
			std::cout << prop_name << ": " << pat[prop_name.c_str()] << "\n";
		}
		std::cout << "\n";
	}

}

// Function for printing individual data after using list -i ...
// 1. Find that individual based on the pesel number
// 2. Print its personal properties if they are specified
// 3. Print all the specified reports

void Command_List::list_sw_i(std::vector<Report>& reports_to_print, std::vector<patient_properites_enum> prop_print, std::string pesel) {
	Patient* pat = nullptr;

	// 1. Find that patient that has been specified
	for (auto& p : *(this->patients)) {
		if (p.get_pesel() == pesel) {
			pat = &p;
			break;
		}
	}

	if (pat == nullptr) {
		std::cerr << "Patient with such pesel: " << pesel << " not found\n";
		return;
	}
	else {
		// 2. Print the personal data
		for (auto& prop : prop_print) {
			std::string prop_str = convert_pat_prop_enum_to_str(prop);
			std::cout << prop_str << ": " << (*pat)[prop_str.c_str()] << "\n";
		}
		std::cout << "\n";

		// 3. Print the specified reports
		std::cout << "Reports found: \n";
		for (Report& rep : reports_to_print) {
			rep.print_all_data();
			std::cout << "\n";
		}
		std::cout << "\n";
	}
}

// Prints a given vector
// Uses the stream << 
// NOTE: Vectors with elements of type that has no overloaded << operator will cause an error

template <class T>
void print_vector(std::vector<T>& vec) {
	for (auto& elem : vec)
		std::cout << elem << " ";
	std::cout << "\n\n";
}

// Command layout: <range> () (<order> () ()) (<property> ()) // (<property> -> property to print
// Available switches: 
// -all -> lists all availabale patients from the vector, doesn't take parameters
//		<> -order (asc/desc) (propert:property_name), only the ones from the Personal_Data.txt
//		<> <> -prop (name, age, illness name, etc...) () () ... -> there can be many specific proeprties to list, but only the ones from the Personal_Data.txt file

// -i -> list only some individuals stuff. By default prints everything, both personal, ilnnes data and all the reports
//		It takes one argument, the PESEL of the desired individual
//		It operates the same as -all with switches like: -prop, 
//		-r 
//			with no args it prints all the reports
//			with argumnets Title, Author, duration time (in a form of dd.mm.yyyy-dd.mm.yyyy) will print the constents of reports that meet all the desired args values
//			ex. list -i 02322651964 -r time: 12.03.1810-23.06.1811 author: Carl Sagan
// 
//		(<property>) () () ....
//		-r 'title: specific report title / date: specific date of adding' -> if no such found give one that has the closest date to the given one

void Command_List::perform() {
	//std::cout << "Entering ->perform()\n";

	// If there is no switches just list the patients
	if (this->switches_with_args.size() == 0)
		this->list_all();
	else {
		std::vector<std::string> arguments;

		if (this->switches_with_args.find("-a") != switches_with_args.end()) {
			bool print_no_rules = true;

			// If size == 1 then it means we have only one switch that being -all
			if (this->switches_with_args.size() == 1) {
				//std::cout << "There is only one switch\n";
				arguments = switches_with_args["-a"];

				if (arguments.size() == 0) {
					this->list_all();
					return;
				}
				else
					throw std::exception("Switch -a for the commant list does not take any arguments\n");
				arguments.clear();
			}
			
			bool ordered_print = false;
			bool ascending_print = true;
			patient_properites_enum prop_to_sort_by = patient_properites_enum::age;

			//Check if these elements are to be sorted in some kind of an order
			if (this->switches_with_args.find("-order") != switches_with_args.end()) {

				Order_Switch order_sw(ordered_print, ascending_print, prop_to_sort_by);
				order_sw.set_switch_data(switches_with_args["-order"]);
			}
			arguments.clear();
			
			std::vector<patient_properites_enum> props_to_print;
			// Check if only a specific proeprty (or a list of properties is supposed to be displayed
			if (switches_with_args.find("-prop") != switches_with_args.end()) {

				Prop_Switch prop_sw(props_to_print);
				prop_sw.set_switch_data(switches_with_args["-prop"]);
			}

			// Print the data according to the switches and arguments
			this->list_sw_all(props_to_print, ordered_print, ascending_print, prop_to_sort_by);
		}

		if (this->switches_with_args.find("-i") != switches_with_args.end()) {
			std::string i_pesel;

			// Handle the choosing of the patient
			// Find the patient with this pesel
			// By performing the Individual_Patient_Command

			Individual_Patient_Switch i_sw(i_pesel, *this->patients);
			i_sw.set_switch_data(this->switches_with_args["-i"]);

			// Check if we want to print only specific properties (personal data) of the patient
			std::vector<patient_properites_enum> props_to_print;
			if (this->switches_with_args.find("-prop") != this->switches_with_args.end()) {
				Prop_Switch prop_sw(props_to_print);
				prop_sw.set_switch_data(switches_with_args["-prop"]);
			}

			std::vector<Report> reports_to_print;
			std::vector<Report*> dummy;
			// Check if the -r switch has been used (to print the reports)
			if (this->switches_with_args.find("-r") != this->switches_with_args.end()) {
				Report_Switch r_sw(i_pesel, *this->patients, reports_to_print, dummy);
				r_sw.set_switch_data(switches_with_args["-r"]);
			}

			/*
			std::cout << "Pesel: " << i_pesel << "\n";
			for (auto& x : props_to_print) {
				std::cout << convert_pat_prop_enum_to_str(x) << "\n";
			}
			*/
			this->list_sw_i(reports_to_print, props_to_print, i_pesel);
		}
	}
}

// NOTE: This could be actually a template
void Command_List::list_all() const {

	for (auto& p : *this->patients)
		p.print_all_data();
}

// It just sorts the given vector and then prints it
void Command_List::list_by_age(bool ascending) const {
	auto comparison = std::bind(compare_patients, std::placeholders::_1, std::placeholders::_2, ascending, "age");

	std::sort(this->patients->begin(), this->patients->end(), comparison);
	this->list_all();

}

void print_props(Patient &p, std::vector<patient_properites_enum>& props_to_print) {
	if (std::find(props_to_print.begin(), props_to_print.end(), patient_properites_enum::age) != props_to_print.end())
		std::cout << "Age: " << p.get_age() << "\n";

	if (std::find(props_to_print.begin(), props_to_print.end(), patient_properites_enum::name) != props_to_print.end())
		std::cout << "Name: " << p.get_name() << "\n";

	if (std::find(props_to_print.begin(), props_to_print.end(), patient_properites_enum::second_name) != props_to_print.end())
		std::cout << "Second Name: " << p.get_second_name() << "\n";

	if (std::find(props_to_print.begin(), props_to_print.end(), patient_properites_enum::PESEL) != props_to_print.end())
		std::cout << "PESEL: " << p.get_pesel() << "\n";

	if (std::find(props_to_print.begin(), props_to_print.end(), patient_properites_enum::gender) != props_to_print.end())
		std::cout << "Gender: " << p.get_gender() << "\n";
}

void Command_List::print_data_sw_all(bool ordered_print, bool ascending_print, std::vector<patient_properites_enum> &props_to_print) {



}