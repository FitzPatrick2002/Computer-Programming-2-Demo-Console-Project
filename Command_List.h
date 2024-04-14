#ifndef COMMAND_LIST
#define COMMAND_LIST

#include <algorithm>
#include <functional>
#include <map>

#include "Command_Base.h"
#include "Patient.h"
#include "Order_Switch.h"
#include "Prop_Switch.h"
#include "Individual_Patient_Switch.h"
#include "Report_Switch.h"

bool compare_patients(Patient& p1, Patient& p2, bool ascending = true, std::string prop = "age");

class Command_List : public Command_Base {
protected:

	std::vector<Patient>* patients;
	//Patient* individual_pat; //If we were to operate on a single instance of a patient we store it here

	std::map<std::string, std::vector<std::string>>& switches_with_args;

public:

	Command_List(std::map<std::string, std::vector<std::string>>& swa, std::vector<Patient>* pat_vec_ptr);

	~Command_List();

	virtual void perform();

	// NOTE: This could be actually a template
	void list_all() const;

	// It just sorts the given vector and then prints it
	void list_by_age(bool ascending = true) const;

	void list_sw_all(std::vector<patient_properites_enum> prop_print, bool order_pr = false, bool asc_pr = true, patient_properites_enum prop_sort = patient_properites_enum::age);

	void list_sw_i(std::vector<Report>& reports_to_print, std::vector<patient_properites_enum> prop_print, std::string pesel);

	// Used insid ->perform() for printing according to switches arguments
	// Used for -all switch 
	void print_data_sw_all(bool ordered_print, bool ascending_print, std::vector<patient_properites_enum>& props_to_print);
};

#endif