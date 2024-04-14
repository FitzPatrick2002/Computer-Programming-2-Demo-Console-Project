#ifndef ILLNESS
#define ILLNESS

#include <tuple>
#include <regex>
#include <filesystem>

#include "Report.h"

namespace fs = std::filesystem;

class Illness {
private:
	bool status; 
	std::string folder_path;

	std::string name;
	std::string description;

	std::vector<std::pair<std::string, std::string>> duration_history;
	std::vector<Report> reports_history;

public:

	Illness();

	Illness(std::string f_path);

	Illness(const Illness &ill);

	~Illness();

	// Getters

	std::string get_name() const;

	std::string get_descripton() const;

	bool get_status() const;

	Report get_report(int i) const;

	// Setters

	//NOTE: Add some control for the name for ex. such illness must exist and be in the data base or smth. Description must be at least 10 characters long. etc...

	void set_name(std::string n);

	void set_description(std::string desc);

	void set_status(bool st);

	// Accessors

	std::vector<Report>& access_report_history();

	// Modifications of duration_hisotry and reports_history


	// Pushes back a new elemnt of this vector
	// NOTE: Add some conitrl with regex whether format f the date is correct
	void add_duration(std::pair<std::string, std::string> p);

	std::pair<std::string, std::string> get_single_duration(int i);

	void print_duration_history() const;

	// Uses .reserve()
	// NOTE: It doesn't clear the vector only reserves the space
	void reserve_space_for_reports(int amount);

	void emplace_report(Report& rep);

	// Printing and presenting data

	void print_general_data() const;

	void print_all_data() const;

	// Reading data from text files and folders

	void read_general_data(std::string path_to_illness_folder = "");

	void read_all_reports(std::string path_to_illness_folder = "");

};

#endif