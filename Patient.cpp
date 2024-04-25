#include "Patient.h"

Patient::Patient(std::string n, std::string sn, int a) : name(n), second_name(sn) {
	this->set_age(a);
	this->directory_name = "";
}

Patient::Patient(std::string dir_name) : directory_name(dir_name) {
	this->directory_name = dir_name;
	this->read_personal_data_from_file(dir_name);
	this->read_patient_illnesses(dir_name);
}

Patient::Patient() {
	this->directory_name = "";
	this->name = "";
	this->second_name = "";
	this->age = 0;
	this->gender = "";
}

Patient::Patient(const Patient& p) {
	if (p.get_gender() == "")
		std::cerr << "When copying patient object with address: " << &p << " has no gender set\n";
	else
		this->set_gender(p.get_gender());

	if (p.get_name() == "")
		std::cerr << "When copying patient object with address: " << &p << " has no name set\n";
	else
		this->set_name(p.get_name());

	if (p.get_second_name() == "")
		std::cerr << "When copying patient object with address: " << &p << " has no second name set\n";
	else
		this->set_second_name(p.get_second_name());

	if (p.get_pesel() == "")
		std::cerr << "When copying patient object with address: " << &p << " has no pesel set\n";
	else
		this->set_pesel(p.get_pesel());

	if(p.get_age() == 0)
		std::cerr<< "When copying patient object with address: " << &p << " has age set to zero\n";
	else
		this->set_age(p.get_age());

	if (p.directory_name == "")
		std::cerr << "When copying patient object with address: " << &p << " no directory_name set\n";
	else
		this->directory_name = p.directory_name;


	// Copy the illnesses

	int cured_ill_amount = p.cured_illness.size();
	this->cured_illness.reserve(cured_ill_amount);

	int on_going_amount = p.on_going_illness.size();
	this->on_going_illness.reserve(on_going_amount);

	for (auto& ill : p.cured_illness)
		this->cured_illness.emplace_back(ill);
	
	for (auto& ill : p.on_going_illness)
		this->on_going_illness.emplace_back(ill);

}

Patient::~Patient() {

}

//Setters and getters

void Patient::print_patient_data() const {
	std::cout << "Name: " << this->get_name() << "\n";
	std::cout << "Second Name: " << this->get_second_name() << "\n";
	std::cout << "Age: " << this->get_age() << "\n";
	std::cout << "PESEL: " << this->get_pesel() << "\n";
	std::cout << "Gender: " << this->get_gender() << "\n\n";

}

void Patient::set_age(int a) {
	if (a >= 0)
		this->age = a;
	else
		throw std::exception("Age provided for patient must be >= 0\n");
}

void Patient::set_name(std::string n) {
	this->name = n;
}

void Patient::set_second_name(std::string sn) {
	this->second_name = sn;
}

void Patient::set_pesel(std::string p) {
	this->pesel = p;
}

void Patient::set_gender(std::string g) {
	this->gender = g;
}

void Patient::set_prop_based_on_string_name(std::string name, std::string value) {

	for (auto& e : name)
		e = std::tolower(e);

	if (name == "name")
		this->set_name(value);
	else if (name == "second name" || name == "second_name" || name == "secondname")
		this->set_second_name(value);
	else if (name == "age") {
		int a = std::atoi(value.c_str());
		this->set_age(a);
	}
	else if (name == "pesel")
		this->set_pesel(value);
	else if (name == "gender")
		this->set_gender(value);
	else
		std::cerr << "Could not set value of the patient property named: " << name << ". No such property exist.\n";

}

//Getters

std::string Patient::get_name() const {
	return this->name;
}

std::string Patient::get_second_name() const {
	return this->second_name;
}

u_int Patient::get_age() const {
	return this->age;
}

std::string Patient::get_pesel() const {
	return this->pesel;
}

std::string Patient::get_gender() const {
	return this->gender;
}

Illness Patient::get_illness_from_on_going(int i) const {
	return this->on_going_illness[i];
}

Illness Patient::get_illness_from_cured(int i) const {
	return this->cured_illness[i];
}

void Patient::print_all_data() const {
	this->print_patient_data();

	std::cout << "Ongoing ilnesses: \n";
	if (this->on_going_illness.size() == 0)
		std::cout << "None\n";
	else
		for (const auto& illness : this->on_going_illness) 
			illness.print_all_data();

	std::cout << "Cured illnesses: \n";
	if (this->cured_illness.size() == 0)
		std::cout << "None\n";
	else
		for (const auto& illness : this->cured_illness)
			illness.print_all_data();

	std::cout << "\n";
}

// Accessors

std::vector<Illness>& Patient::acces_on_going_illnesses() {
	return this->on_going_illness;
}

std::vector<Illness>& Patient::access_cured_illnesses() {
	return this->cured_illness;
}

std::vector<Illness> Patient::access_all_illnesses() {
	int size_1 = this->on_going_illness.size();
	int size_2 = this->cured_illness.size();

	std::vector<Illness> all;
	all.reserve(size_1 + size_2);

	for (int i = 0; i < size_1; i++)
		all.emplace_back(this->on_going_illness[i]);

	for (int i = 0; i < size_2; i++)
		all.emplace_back(this->cured_illness[i]);
		
	return all;
}

Illness& Patient::access_illness_from_on_going(int i) {
	return this->on_going_illness[i];
}

Illness& Patient::access_Illness_from_cured(int i) {
	return this->cured_illness[i];
}

// Misc

void Patient::push_ongoing_illness(Illness& ill) {
	this->on_going_illness.push_back(ill);
}

void Patient::push_cured_illness(Illness& ill) {
	this->cured_illness.push_back(ill);
}

// Reading from file

void Patient::read_personal_data_from_file(std::string patient_folder_dir) {
	if (!patient_folder_dir.empty())
		if (this->directory_name != patient_folder_dir)
			this->directory_name = patient_folder_dir;

	std::string personal_dir;
	if(!this->directory_name.empty())
		personal_dir = this->directory_name + "\\Personal_Data.txt";
	else {
		std::cerr << "Cannot read personal data for the patient for there is no directory provided\n";
		return;
	}

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
				std::string n = merge_vector_strings(split_line, 1, split_line.size());
				this->set_name(n);
			}
			else if (str_to_lower(split_line[0]) == "age:") {
				int a = std::atoi(split_line[1].c_str());
				this->set_age(a);
			}
			else if (str_to_lower(split_line[0]) == "gender:") {
				std::string g = split_line[1];
				this->set_gender(g);
			}
			else if (str_to_lower(split_line[0]) == "pesel:") {
				std::string p = merge_vector_strings(split_line, 1, split_line.size());
				this->set_pesel(p);
			}
			else if (str_to_lower(merge_vector_strings(split_line, 0, 2)) == "secondname:") {
				std::string s_n = merge_vector_strings(split_line, 2, split_line.size());
				this->set_second_name(s_n);
			}
			else {
				throw std::exception("Wrongly set property in one of patients files\n");
			}
		}
	}
	else
		std::cerr << "Could not open the file Personal_Data.txt" << " in provided directory: " << patient_folder_dir << "\n";
}

void Patient::read_patient_illnesses(std::string patient_folder_dir) {
	if (!patient_folder_dir.empty())
		if (this->directory_name != patient_folder_dir)
			this->directory_name = patient_folder_dir;

	std::string ilnesses_dir;
	if (!this->directory_name.empty())
		ilnesses_dir = this->directory_name + "\\Illness";
	else {
		std::cerr << "Cannot read personal data for the patient for there is no directory provided\n";
		return;
	}

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
			this->on_going_illness.push_back(illness);
		else
			this->cured_illness.push_back(illness);
	}
}

// Saving current state into file

void Patient::save_personal_data_to_file() {

	//Change the '\\' into '/'
	for (auto& c : this->directory_name)
		if (c == '\\')
			c = '/';

	std::ofstream personal_file(this->directory_name + "/Personal_Data.txt");

	if (personal_file.is_open()) {
		std::string new_name = "Name: " + this->name + "\n";;
		std::string new_second_name = "Second Name: " + this->second_name + "\n";
		std::string new_age = "Age: " + std::to_string(this->age) + "\n";
		std::string new_pesel = "PESEL: " + this->pesel + "\n";
		std::string new_gender = "Gender: " + this->gender + "\n";

		personal_file << new_name.c_str();
		personal_file << new_second_name.c_str();
		personal_file << new_age.c_str();
		personal_file << new_pesel.c_str();
		personal_file << new_gender.c_str();

		personal_file.close();
	}
	else {
		std::cerr << "Could not open the file: " << this->directory_name + "Personal_Data.txt, in order to save new Patient data\n";
	}

}

// Overloads

std::string Patient::operator[] (const char* prop) const {
	std::string str(prop);
	str = str_to_lower(str);
	//prop = str.c_str();

	if (str == "name")
		return this->get_name();
	else if (str == "second_name" || str == "second name" || str == "secondname")
		return this->get_second_name();
	else if (str == "pesel")
		return this->get_pesel();
	else if (str == "gender")
		return this->get_gender();
	else if (str == "age") {

		auto int_to_str = [&] {
			std::string outcome = "";
			u_int val = this->get_age();

			while (val > 0) {
				int rest = val % 10;
				char letter = (char)(48 + rest);
				outcome = letter + outcome;

				val = val / 10;
			}

			return outcome;
			};

		std::string current_age = int_to_str();
		return current_age;
	}
	else {
		std::string mess = "Tried to acces non-existing property: " + str + " in the Patient class\n";
		throw std::exception(mess.c_str());
	}

}