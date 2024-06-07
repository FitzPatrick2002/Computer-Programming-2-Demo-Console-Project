#ifndef DELETE_COMMAND
#define DELETE_COMMAND

#include <map>
#include <filesystem>

#include "Patient.h"
#include "Command_Base.h"
#include "Individual_Patient_Switch.h"

class Delete_Command : public Command_Base {
private:

	// Stuff from the 'main' program that will be modified / needed by this command

	std::vector<Patient>* patients;

	std::map<std::string, std::vector<std::string>>& switches_with_args;

public:

	Delete_Command(std::vector<Patient>* all_patients, std::map<std::string, std::vector<std::string>>& swa);

	~Delete_Command();

	void perform();

	void delete_patient(Patient* p);


};

#endif