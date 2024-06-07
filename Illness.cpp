#include "Illness.h"

Illness::Illness() {

}

Illness::Illness(std::string f_path) : folder_path(f_path) {
	this->description = "";
	this->name = "";
	this->status = true;
}

// Used for the first time when we were adding a new illness to already existing patient
Illness::Illness(std::string name, std::string desc, std::pair<std::string, std::string> d_h, std::string folder_p) {
	this->name = name;
	this->description = desc;
	this->duration_history.push_back(d_h);
	this->folder_path = folder_p;

}

/*
bool status;
	std::string folder_path;

	std::string name;
	std::string description;

	std::vector<std::pair<std::string, std::string>> duration_history;
	std::vector<Report> reports_history;
*/

Illness::Illness(const Illness& ill) {

	this->status = ill.status;
	this->name = ill.name;
	this->description = ill.description;
	this->folder_path = ill.folder_path;

	// Copy the duration history
	int dur_hist_len = ill.duration_history.size();
	this->duration_history.reserve(dur_hist_len);

	for (auto& span : ill.duration_history)
		this->duration_history.emplace_back(span);
		
	// Copy the reports history
	int reports_amount = ill.reports_history.size();
	this->reports_history.reserve(reports_amount);

	for (auto& rep : ill.reports_history)
		this->reports_history.emplace_back(rep);
}

Illness::~Illness() {

}

// Getters

std::string Illness::get_name() const {
	return this->name;
}

std::string Illness::get_descripton() const {
	return this->description;
}

bool Illness::get_status() const {
	return this->status;
}

Report Illness::get_report(int i) const {
	if (i >= 0 && i < this->reports_history.size())
		return this->reports_history[i];
}

std::string Illness::get_path() const {
	return this->folder_path;
}


// Setters

//NOTE: Add some control for the name for ex. such illness must exist and be in the data base or smth. Description must be at least 10 characters long. etc...

void Illness::set_name(std::string n) {
	this->name = n;
}

void Illness::set_description(std::string desc) {
	this->description = desc;
}

void Illness::set_status(bool st) {
	this->status = st;
}

void Illness::set_path(std::string p) {
	this->folder_path = p;
}

// Accessors

std::vector<Report>& Illness::access_report_history() {
	return this->reports_history;
}

std::vector<std::pair<std::string, std::string>>& Illness::access_duration_history() {
	return this->duration_history;
}


// Modifications of duration_hisotry and reports_history


// Pushes back a new elemnt of this vector
// NOTE: Add some conitrl with regex whether format f the date is correct
void Illness::add_duration(std::pair<std::string, std::string> p) {
	this->duration_history.push_back(p);
}

std::pair<std::string, std::string> Illness::get_single_duration(int i) {
	if (i >= 0 && i < this->duration_history.size())
		return this->duration_history[i];

	throw std::exception("Tried to access vector outside it's range. Illness.duration_history\n");
}

void Illness::print_duration_history() const {
	for (auto& elem : this->duration_history)
		std::cout << elem.first << " : " << elem.second << "\n";

	std::cout << "\n";
}

// Uses .reserve()
// NOTE: It doesn't clear the vector only reserves the space
void Illness::reserve_space_for_reports(int amount) {
	if (amount <= 0) {
		std::cerr << "Could not reserve negative or 0 amount of space in the reports vector for an illness\n";
		return;
	}
	this->reports_history.reserve(amount);
}

void Illness::emplace_report(Report& rep) {
	this->reports_history.emplace_back(rep);
}

// Printing and presenting data

void Illness::print_general_data() const {
	std::cout << "Name: " << this->name << "\n";
	std::cout << "Duration: ";

	this->print_duration_history();
}

void Illness::print_all_data() const {
	this->print_general_data();

	for (const auto& rep : this->reports_history) {
		std::cout << "printing report\n";
		rep.print_all_data();
	}

	std::cout << "\n";
}

void Illness::read_general_data(std::string path_to_illness_folder) {
	if (!path_to_illness_folder.empty()) {
		if (this->folder_path != path_to_illness_folder)
			this->folder_path = path_to_illness_folder;
	}
	else
		this->folder_path = path_to_illness_folder; // If there was no path set so just set it
		

	std::string general_path;
	if(!this->folder_path.empty())
		 general_path = this->folder_path + "\\General.txt";
	else {
		std::cerr << "There is no path set for reading general data of an illness\n";
		return;
	}

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
				this->set_name(full_name);
			}
			else if (str_to_lower(split_line[0]) == "status:") {
				std::string status = str_to_lower(split_line[1]);
				if (status == "ongoing")
					this->set_status(true);
				else if (status == "cured")
					this->set_status(false);
				else
					throw std::exception("Status for an illness has been wrongly set. Available words: ongoing / cured");
			}
			else if (str_to_lower(split_line[0]) == "duration:") {
				// Read the duration of the illness
				// Each line contains the next period of being ill

				// Do the matches with REGEX
				std::string both_dates = merge_vector_strings(split_line, 1, split_line.size());

				std::string string_format = "(\\d{2}[.-]\\d{2}[.-]\\d{4})\\s*[:~-]\\s*(\\d{2}[.-]\\d{2}[-.]\\d{4}|-)";
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
							this->add_duration(illness_period_duriation);
						}
						else
							std::cerr << "Could not load one of the illness durations. Regex found pattern too short.\n";
					}
					else
						std::cerr << "Regex didn't match anything in the General.txt for an illness. Problem with Duration:\n";

				} while (std::getline(general_data_file, line));
			}
		}
		//this->print_duration_history();
	}
}

void Illness::read_all_reports(std::string path_to_illness_folder) {
	if(!path_to_illness_folder.empty())
		if (this->folder_path != path_to_illness_folder)
			this->folder_path = path_to_illness_folder;
	
	std::string illness_reports_path;
	if(!this->folder_path.empty())
		illness_reports_path = this->folder_path + "\\Reports";
	else {
		std::cerr << "Cannot read reports fro the path has been not set\n";
		return;
	}

	// Count the reports and reserve space for them
	int reports_amount = 0;
	for (const auto& elem : fs::directory_iterator(illness_reports_path))
		reports_amount++;

	this->reports_history.reserve(reports_amount);

	// Read each report file
	for (const auto& elem : fs::directory_iterator(illness_reports_path)) {
		std::string report_path = elem.path().string();
		std::ifstream report_file(report_path);

		Report report(report_path);

		report.read_report_file();
		this->reports_history.emplace_back(report);
	}
}
