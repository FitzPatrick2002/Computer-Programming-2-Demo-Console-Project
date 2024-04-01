#include "Report_Switch.h"


Report_Switch::Report_Switch(std::string i_p, std::vector<Patient>& pat, std::vector<Report>& repo) : Base_Switch(), i_pesel(i_p), all_patients(pat), reports(repo) {

}

Report_Switch::~Report_Switch() {

}

// Checks if the first date happened earlier than the second one
// Also checks if dates are in the form of: dd.mm.yyyy

bool date_earlier_than(std::string& date_1, std::string date_2) {
	std::regex date_reg("(\\d{2})\\(d{2})\\(d{4}))");
	std::smatch m1, m2;
	
	if (std::regex_match(date_1, m1, date_reg) && std::regex_match(date_2, m2, date_reg)) {
		int year_1 = std::atoi(m1[3].str().c_str());
		int year_2 = std::atoi(m2[3].str().c_str());

		if (year_1 < year_2) {
			return true;
		}
		else if (year_1 == year_2) {
			int month_1 = std::atoi(m1[2].str().c_str());
			int month_2 = std::atoi(m2[2].str().c_str());

			if (month_1 < month_2)
				return true;
			else if (month_1 == month_2) {
				int day_1 = std::atoi(m1[1].str().c_str());
				int day_2 = std::atoi(m2[1].str().c_str());

				if (day_1 <= day_2)
					return true;
			}
		}
	}
	else
		std::cerr << "Given two strings are not dates. Can't say which happened earlier";
	
	return false;
}

// Passed arguments are the in the form ex. Title: Another_boring_report, Date: 12.04.3021-23.05.3022, Author: Daidus
// Title, Author, Date -> specific or a range 
// Illness

// The algorithm:
// 0. Check if this patient exist (by pesel), if not just end the function and throw cerr
// 1. extract the properties of the report from the passed arguments (Title: this_report_is_stupid)
//		BTW. it changes all the '_' into ' '
// 2. If date was specified use regex to check if it is in an appropriate format
// 3. Loop through all the reports of this patient and check if they meet requirements, if so add the to the this->reports vector

void Report_Switch::set_switch_data(std::vector<std::string>& arguments) {
	//Check if this patient even exist

	int patient_index = -1;
	for (int i = 0; i < this->all_patients.size(); i++) {
		if (this->all_patients[i].get_pesel() == this->i_pesel) {
			patient_index = i;
			break;
		}
	}

	if (patient_index == -1) {
		std::cerr << "For the switch -r pesel: " << this->i_pesel << " has not been found among the patients\nFunction not executed\n";
		return;
	}

	std::string author;
	std::string title;
	std::string illness;
	
	std::string date_range;
	std::string start_date;
	std::string end_date;

	// Parse the arguments
	
	std::regex argument_reg("([\\w\\D]):[\\s]*([\\w])");
	std::smatch m;

	// 1. Check which property user demands report has
	// 2. Save it

	for (auto& arg : arguments) {

		if (std::regex_search(arg, m, argument_reg)) {
			std::string prop_name = str_to_lower(m[1].str());
			std::string prop_value = m[2].str();

			//Change the prop_value '_' to ' '
			for (auto& letter : prop_value)
				if (letter == '_')
					letter = ' ';

			if (prop_name == "title") {
				title = prop_value;
			}
			else if (prop_name == "author") {
				author = prop_value;
			}
			else if (prop_name == "date") {
				date_range = prop_value;
			}
			else if (prop_name == "illness") {
				illness = prop_value;
			}
			else 
				std::cerr << "Couldn't read property passed to switch -r\n";
		}
		else 
			std::cerr << "Given argument: " << arg << " to switch -i was invalid\n";
	}

	// If date has been specified check if it is a specific date or a range of dates

	if (!date_range.empty()) {
		std::regex date_range_reg("(\\d{2}.\\d{2}.\\d{4})-(\\d{2}.\\d{2}.\\d{4})");
		std::regex date_specific_reg("(\\d{2}.\\d{2}.\\d{4})");
		std::smatch m;

		if (std::regex_match(date_range, m, date_range_reg)) {
			start_date = m[1].str();
			end_date = m[2].str();

			date_range = "";
		}
		else if (std::regex_match(date_range, m, date_specific_reg)) {
			start_date = end_date = m[1].str();
		}
		else
			std::cerr << "Given range of dates: " << date_range << " for the -r is in an inappropriate form\n";
	}

	//Now decide which reports of this patient match the requiremets

	Patient& pat = this->all_patients[patient_index];
	std::vector<Illness>& illnesses = pat.acces_on_going_illnesses();

	for (auto& ill : illnesses) {
		// Check if we want reports for this kind of illness
		if (ill.get_name() == illness) {
			for (auto& rep : ill.access_report_history()) {

				bool title_ok = true;
				bool author_ok = true;
				bool date_ok = true;

				// Check of this report has the the properties required by the user to be printed
				if (title != "") {
					if (rep.get_title() != title)
						title_ok = false;
				}

				if (author != "") {
					if (rep.get_author() != author)
						author_ok = false;
				}

				if (date_range != "") {
					if (start_date != end_date) {
						// Check if the date when the report has been added is lesser than the end_date and greater then the start_date

						std::string report_date = rep.get_date();
						if (date_earlier_than(report_date, start_date) || !date_earlier_than(report_date, end_date))
							date_ok = false;
					}
					else {
						if (rep.get_date() != start_date)
							date_ok = false;
					}
				}

				// If this report fullfills the requirements add it to the vector of repoerts to be printed
				this->reports.push_back(rep);
			}
		}
	}

	
}