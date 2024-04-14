#ifndef COMMAND_ADD
#define COMMAND_ADD

#include <map>
#include <filesystem>
#include <fstream>

#include "Command_Base.h"
#include "Patient.h"

#include "Add_Patient_Switch.h"

class Command_Add : public Command_Base {
private:
	std::string patients_folder_directory;
	std::vector<Patient>* patients;

	std::map<std::string, std::vector<std::string>>& switches_with_args;

public:

	Command_Add(std::vector<Patient>* pat_vec, std::map<std::string, std::vector<std::string>>& sw_w_arg, std::string pat_folder);

	~Command_Add();

	void add_new_patient(bool correctly_done_sw);

	void perform();

};

#endif