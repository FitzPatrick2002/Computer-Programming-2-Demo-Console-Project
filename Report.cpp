#include "Report.h"

Report::Report(std::string fl) : file_location(fl) {
	content = "";
	title = "";
	author = "";
	date_of_adding = "";
}

Report::Report(const Report& rep) {
	this->content = rep.content;
	this->title = rep.title;
	this->author = rep.author;
	this->date_of_adding = rep.date_of_adding;
	this->file_location = rep.file_location;
}

Report::Report() {

}

Report::~Report() {

}

bool Report::assign_values(std::string type, std::string val) {
	if (type == "Title: ")
		this->title = val;
	else if (type == "Author: ")
		this->author = val;
	else if (type == "Date: ")
		this->date_of_adding = val;
	else
		throw std::exception("During reading a report file parameter wasn;t equalt ot Author: / Title: / Date: \n");
}

// Getters and setters

std::string Report::get_author() const {
	return this->author;
}

std::string Report::get_title() const {
	return this->title;
}

std::string Report::get_date() const {
	return this->date_of_adding;
}

std::string Report::get_report_content() const {
	return this->content;
}

// Setters

// NOTE: Add control system for checking whether name is valid or not (simple regex so that name and second name must be at least 2 letters long and doesn;t contain numebrs etc.)
// NOTE: More ways to add content (from separate txt file, directly in here from the console (mini text editor)).
// NOTE: Mini text editor for modification of content (and then saving it).

void Report::set_author(std::string auth) {
	this->author = auth;
}

void Report::set_title(std::string t) {
	this->title = t;
}

void Report::set_date(std::string d) {
	this->date_of_adding = d;
}

void Report::set_content(std::string c) {
	this->content = c;
}

void Report::set_file_location(std::string l) {
	this->file_location = l;
}


void Report::append_content(std::string s) {
	this->content += s;
}

void Report::print_all_data() const {
	std::cout << "Title: " << this->title << "\n";
	std::cout << "Author: " << this->author << "\n";
	std::cout << "Date: " << this->date_of_adding << "\n";

	std::cout << "Content: " << this->content << "\n";
	std::cout << "\n";
}

// Reading the files

// Read the file with the report
// If path is not given then use the one saved in the object
// If it is given than change the one saved and read from this file
void Report::read_report_file(std::string file_path){
	if (not file_path.empty()) 
		this->file_location = file_path;

	std::ifstream report_file(this->file_location);

	if (report_file.is_open()) {
		std::string line;

		while (std::getline(report_file, line)) {
			if (line.empty())
				continue;

			std::stringstream ss(line);

			std::vector<std::string> split_line;
			std::string temp;
			while (ss >> temp)
				split_line.push_back(temp);

			if (str_to_lower(split_line[0]) == "author:") {
				std::string auth = merge_vector_strings(split_line, 1, split_line.size(), " ");
				this->set_author(auth);
			}
			else if (str_to_lower(split_line[0]) == "date:") {
				std::string date = split_line[1];
				this->set_date(date);
			}
			else if (str_to_lower(split_line[0]) == "title:") {
				std::string title = merge_vector_strings(split_line, 1, split_line.size(), " ");
				this->set_title(title);
			}
			else {
				this->append_content(line);

				//Read the main reports content
				while (std::getline(report_file, line))
					this->append_content(line);
			}
		}
	}
	else
		std::cerr << "Could not open the reports path for an illness. Path: " << file_path << "\n";
}

// Saving data to file

void Report::save_current_data() {

	if (this->file_location != "") {
		std::ofstream file;

		file.open(this->file_location, std::ios::out | std::ios::trunc);

		if (file.is_open()) {
			std::string input = "";

			input = "Title: " + this->get_title() + "\n";
			file << input;

			input = "Author: " + this->get_author() + "\n";
			file << input;

			input = "Date: " + this->get_date() + "\n";
			file << input;

			input = this->get_report_content() + "\n";
			file << "\n" << input;

			file.close();
		}
		else
			std::cerr << "Could not open the file: " << this->file_location << " in order to save report of title: "<< this->get_title() << "\n";

	}
	else
		std::cerr << "File for saving the report of title: " << this->get_title() << " is not set\n";

}
