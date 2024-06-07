#ifndef REPORT_SWITCH
#define REPORT_SWITCH

#include "Base_Switch.h"
#include "Patient.h"

#include <regex>

// Report switch decides which reports are to be printed
// It is given the pesel of the desired patient and list of properties (for ex. author) that it's report are to fullfill in order to be printed
// This switch is solely for finding stuff, not modyfying or changing or adding...

class Report_Switch : public Base_Switch {
private:

	std::string i_pesel;
	std::vector<Patient> &all_patients;
	std::vector<Report> &reports; //Number of the report in the patient.reports // Here we store the reports that are to be used

	// If we want to obtain reports so that we can modify them directly we can get pointers to them.
	// Yes I know, pointerse to memebers of a vector is a really bad idea... Nontheless...
	// Czemu mnie to spotyka?

	std::vector<Report*> &reports_ptr;

public:

	Report_Switch(std::string i_p, std::vector<Patient> &pat, std::vector<Report> &repo, std::vector<Report*>& ptr_repo);

	~Report_Switch();

	virtual void set_switch_data(std::vector<std::string>& arguments);

	void set_switch_data_as_ptr(std::vector<std::string>& arguments, bool why);

};

#endif