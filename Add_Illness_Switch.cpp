#include "Add_Illness_Switch.h"

Add_Illness_Switch::Add_Illness_Switch(std::string& n, std::pair<std::string, std::string>& t_s, Patient* p)
	: name(n), time_span(t_s), patient(p)
{

}

Add_Illness_Switch::~Add_Illness_Switch() {

}

void Add_Illness_Switch::set_switch_data(std::vector<std::string>& arguments) {

	// 1. Check if number of arguments is even if not then something has been specified worng
	// 2. Extract the name and time span of the illness from the 'arguments' vector
	// 3. Check validity of the name and time span of the illness
	//		3.1. Check if this patient does not already have such an illness
	// NOTE. We do not check if such patient exist for we assume that it has already been done by the -i switch

	if (arguments.size() != 4) {
		std::cerr << "Arguments passed to the '-il' should be in a form name: abc_def, time: dd-mm-yyyy-dd-mm-yyyy\n";
		return;
	}

	// 2. For checking the name we could do something fancy like parse a database of known to humanity illnesses or something like that, but we are going simple this time
	//	  As for date we just check if it is specified in a known format dd-mm-yyyy-dd-mm-yyyy / dd.mm.yyyy-dd.mm.yyyy / 
	std::string span_string;

	for (int i = 0; i < arguments.size(); i += 2) {
		if (arguments[i] == "name:") {
			this->name = arguments[i + 1];
		}
		else if (arguments[i] == "time:") {
			span_string = arguments[i + 1];
		}

		else {
			std::cerr << "Ont of the aruments to switch -il has been misspelled. Available: name: , time:";
			this->name = "";
			this->time_span = std::pair<std::string, std::string>("", "");
			return;
		}
	}

	// Also if name of the illness was multi-element we change _ -> ' ', ex. name_of_illness -> name of illness

	for (auto& c : this->name)
		if (c == '_')
			c = ' ';

	// 3.1. Check if this patient does not posses already such an illness

	for(auto& ill : this->patient->access_cured_illnesses())
		if (ill.get_name() == this->name) {
			std::cerr << "Patient already have illness (cured) of such name: " << this->name << "\n";
			this->name = "";
			this->time_span = std::pair<std::string, std::string>("", "");
			return;
		}

	for (auto& ill : this->patient->acces_on_going_illnesses())
		if (ill.get_name() == this->name) {
			std::cerr << "Patient already have illness (on going) of such name: " << this->name << "\n";
			this->name = "";
			this->time_span = std::pair<std::string, std::string>("", "");
			return;
		}

	// 3. Check validity of the name and time span of the illness

	if (this->name.size() < 3) {
		std::cerr << "Name of the illness should be at least three characters long\n";
		this->name = "";
		this->time_span = std::pair<std::string, std::string>("", "");
		return;
	}

	// The Illness class has a field std::vector<std::pair<std::string, std::string>> duration_history;, so ew just split the stuff...
	// Why did I design it like this? WHy not a vector of strings and a function ::split_date(std::string) ?
	std::regex date_range_regex("(\\d{2}[-.]\\d{2}[-.]\\d{4})[-](-|\\d{2}[-.]\\d{2}[-.]\\d{4})");
	std::smatch m;

	// We should also check if the following date is greater than the preceding one...
	std::regex_match(span_string, m, date_range_regex);
	this->time_span = std::pair<std::string, std::string>(m[1], m[2]);
}