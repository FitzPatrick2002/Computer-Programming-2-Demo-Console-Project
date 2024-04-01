#ifndef REPORT_SWITCH
#define REPORT_SWITCH

#include "Base_Switch.h"
#include "Patient.h"

#include <regex>

// Report switch decides which reports are to be printed
// It is given the pesel of the desired patient and list of properties (for ex. author) that it's reoprt are to fullfill in order to be printed

class Report_Switch : public Base_Switch {
private:

	std::string i_pesel;
	std::vector<Patient>& all_patients;
	std::vector<Report> &reports; //Number of the report in the patient.reports

public:

	Report_Switch(std::string i_p, std::vector<Patient> &pat, std::vector<Report> &repo);

	~Report_Switch();

	virtual void set_switch_data(std::vector<std::string>& arguments);
};

#endif