#ifndef CHANGE_COMMAND
#define CHANGE_COMMAND

#include "Command_Base.h"
#include "Patient.h"
#include "Individual_Patient_Switch.h"
#include "Prop_Switch.h"
#include "Report_Switch.h"

#include <map>

class Change_Command : public Command_Base {
private:

	std::vector<Patient>* patients;

	std::map<std::string, std::vector<std::string>>& switches_with_args;

public:

	Change_Command(std::map<std::string, std::vector<std::string>>& swa, std::vector<Patient>* pat_vec_ptr);

	~Change_Command();

	void change_specific_personal_data(std::vector<patient_properites_enum>& props_to_change, std::string pesel);

	void change_reports(std::vector<Report*> report, std::string pesel);

	void perform();
};

#endif