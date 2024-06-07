#include "Add_Report_Switch.h"

Add_Reports_Switch::Add_Reports_Switch(std::string& t, std::string& au, std::string& d, Illness *& ill_ptr, Patient*& pat_ptr) : patient_ptr(pat_ptr), title(t), author(au), date(d), illness_ptr(ill_ptr){

}

Add_Reports_Switch::~Add_Reports_Switch() {

}

void Add_Reports_Switch::basic_error() {
	std::cerr << "Arguments are to be passed to switch -ar title:---- author:---- date:dd.mm.yyyy\n";
	this->author = "";
	this->date = "";
	this->title = "";
}

// Check the validity of arguments: author, title, date, illness check if such an illness even exist in the given patient
void Add_Reports_Switch::set_switch_data(std::vector<std::string>& arguments) {
	if (arguments.size() != 8) {
		basic_error();
		return;
	}
	else {
		
		std::string illness_name = "";
		// Extract arguments
		for (int i = 0; i < arguments.size(); i += 2) {
			std::cout << "arg provded: " << arguments[i] << arguments[i + 1] << "\n";

			if (arguments[i] == "title:") {
				this->title = arguments[i + 1];
			}
			else if (arguments[i] == "date:") {
				this->date = arguments[i + 1];
			}
			else if (arguments[i] == "author:") {
				this->author = arguments[i + 1];
			}
			else if (arguments[i] == "illness:") {
				illness_name = arguments[i + 1];
			}
			else {
				basic_error();
				return;
			}
		}

		// Check if this patient even has such an illness or is it necessary to first create it?

		// Check the cured illnesses
		for (auto& c : illness_name) //This for actually cuts off the ':' element ?
			c = std::tolower(c);

		for (auto& ill : patient_ptr->access_cured_illnesses()) {
			std::string temp_name = ill.get_name();
			for (auto& c : temp_name)
				c = std::tolower(c);

			// This is the illness we seek
			if (temp_name == illness_name) {
				this->illness_ptr = &ill;
				break;
			}
		}

		// Check the ongoing illnesses
		for (auto& ill : patient_ptr->acces_on_going_illnesses()) {
			std::string temp_name = ill.get_name();

			for (auto& c : temp_name)
				c = std::tolower(c);

			// This is the illness we seek
			if (temp_name == illness_name) {
				this->illness_ptr = &ill;
				break;
			}
		}

		// If an illnes has not been specified then end the program
		if (illness_ptr == nullptr) {
			std::cerr << "No such illness: " << illness_name << " has been found\n";
			basic_error();
			return;
		}

		// Check if all arguments has been specified

		if (this->title == "" || this->date == "" || this->author == "") {
			std::cout << "One of the args title, illness, author or date is not set\n";
			basic_error();
		}
		else {
			// Check validity of these strings

			if (this->title.size() < 3) {
				std::cerr << "Title must be at least 3 characters long\n";
				basic_error();
				return;
			}

			if (this->author.size() < 2) {
				std::cerr << "Even chinese have longer names than that...\n";
				basic_error();
				return;
			}

			std::regex date_reg("(\\d{2}[.-]\\d{2}[.-]\\d{4})");
			std::smatch m;

			if (!std::regex_match(this->date, m, date_reg)) {
				std::cerr << "Date has been specified wrongly. Should be: dd.mm.yyyy (only numbers)\n";
				basic_error();
			}
		}
	}
}