#ifndef REPORT
#define REPORT

#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

std::string str_to_lower(std::string s);
std::string merge_vector_strings(std::vector<std::string> vec, int start, int end, std::string sep = "");

class Report {
private:
	std::string file_location;

	std::string title;
	std::string author;

	std::string date_of_adding;

	std::string content;

public:

	Report(std::string fl);

	Report(const Report& rep);

	Report();

	~Report();

	bool assign_values(std::string type, std::string val);

	// Getters and setters

	std::string get_author() const;

	std::string get_title() const;

	std::string get_date() const;

	std::string get_report_content() const;

	// Setters

	// NOTE: Add control system for checking whether name is valid or not (simple regex so that name and second name must be at least 2 letters long and doesn;t contain numebrs etc.)
	// NOTE: More ways to add content (from separate txt file, directly in here from the console (mini text editor)).
	// NOTE: Mini text editor for modification of content (and then saving it).
	
	
	void set_author(std::string auth);

	void set_title(std::string t);

	void set_date(std::string d);

	void set_content(std::string c);

	void set_file_location(std::string l);

	void append_content(std::string s);

	// Printing data

	void print_all_data() const;

	// Downloading data from file

	void read_report_file(std::string file_path="");

	// Saving data to file

	void save_current_data();
};

#endif