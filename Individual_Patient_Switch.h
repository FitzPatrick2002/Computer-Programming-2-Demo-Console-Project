#ifndef INDIVIDUAL_PATIENT_SWITCH
#define INDIVIDUAL_PATIENT_SWITCH

#include "Base_Switch.h"
#include "Patient.h"

#include <regex>

// Responsible for setting the following kopitons in the for ex. Command_List for printing:
// Setting the pesel of the desired patient -> also a reference to the object representing the patient
// Also checks if such patient exists at all

class Individual_Patient_Switch : public Base_Switch {
private:
	std::string& i_pesel;
	std::vector<Patient>& all_patients;
	
public:

	Individual_Patient_Switch(std::string& i_p, std::vector<Patient>& vec);

	~Individual_Patient_Switch();

	//It just checks whether pesel is okay and patient with such pesel exist
	virtual void set_switch_data(std::vector<std::string>& arguments);

	// Does the same but also sets a pointer to this particualr patient
	virtual void set_switch_data(std::vector<std::string>& arguemnts, Patient*& pat);

};

#endif