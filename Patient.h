#ifndef PATIENT
#define PATIENT

#include <iostream>
#include <string>
#include <vector>

#include "Illness.h"

using u_int = unsigned int;

class Patient {
private:
	//Personal_Data
	std::string name;
	std::string second_name;
	std::string pesel;
	std::string gender;

	u_int age;
	
	std::string directory_name;

	//Illnesses data
	std::vector <Illness> on_going_illness;
	std::vector <Illness> cured_illness;

public:

	Patient(std::string n, std::string sn, int a);

	Patient(std::string dir_name);

	Patient(const Patient& p);
	
	Patient();

	~Patient();

	//Setters and getters

	void print_patient_data() const;

	void set_age(int a);

	void set_name(std::string n);

	void set_second_name(std::string sn);

	void set_pesel(std::string p);

	void set_gender(std::string g);

	void set_prop_based_on_string_name(std::string name, std::string value);

	//Getters

	std::string get_name() const;

	std::string get_second_name() const;

	u_int get_age() const;

	std::string get_pesel() const;

	std::string get_gender() const;

	// Accessors

	std::vector<Illness>& acces_on_going_illnesses();

	std::vector<Illness>& access_cured_illnesses();

	std::vector<Illness> access_all_illnesses();

	// Printing data

	void print_all_data() const;

	// Modification of vectors

	void push_ongoing_illness(Illness& ill);

	void push_cured_illness(Illness& ill);

	// Reading personal data from files and illnesses from folders

	void read_personal_data_from_file(std::string patient_folder_dir = "");

	void read_patient_illnesses(std::string patient_folder_dir = "");

	// Saving succrent state of the Patient into file

	void save_personal_data_to_file() ;

	// Overloads

	std::string operator[] (const char* prop) const;

};
#endif
