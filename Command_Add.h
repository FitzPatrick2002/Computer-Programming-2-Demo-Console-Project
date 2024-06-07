#ifndef COMMAND_ADD
#define COMMAND_ADD

#include <map>
#include <filesystem>
#include <fstream>
#include <tuple>

#include "Command_Base.h"
#include "Patient.h"
#include "Report.h"

#include "Add_Patient_Switch.h"
#include "Individual_Patient_Switch.h"
#include "Add_Illness_Switch.h"
#include "Add_Report_Switch.h"
#include "Obtain_Illness_Switch.h"


class Command_Add : public Command_Base {
private:
	std::string patients_folder_directory; // This is where the Patients folder is located, realtive to our main.cpp file
	std::vector<Patient>* patients;

	std::map<std::string, std::vector<std::string>>& switches_with_args;

public:

	Command_Add(std::vector<Patient>* pat_vec, std::map<std::string, std::vector<std::string>>& sw_w_arg, std::string pat_folder);

	~Command_Add();

	void add_new_patient(bool correctly_done_sw);

	void add_new_illness(std::string name, std::pair<std::string, std::string> duration, Patient* pat_ptr);

	void add_new_report(std::string title, std::string author, std::string date, Illness* ill_ptr, Patient* pat_ptr);

	void add_new_illness_duration_period(Illness* ill_ptr);

	void perform();

};

#endif