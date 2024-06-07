#ifndef ADD_REPORT_SWITCH
#define ADD_REPORT_SWITCH

#include "Base_Switch.h"
#include "Illness.h"
#include "Patient.h"

#include <regex>

// Used like this: -ar title: ---- author: ---- date: ---- illness: -----

class Add_Reports_Switch : public Base_Switch {
private:

	std::string& title;
	std::string& author;
	std::string& date;

	Illness*& illness_ptr;
	Patient*& patient_ptr;

public:

	Add_Reports_Switch(std::string &t, std::string &au, std::string& d, Illness *& ill_ptr, Patient *& pat_ptr);

	~Add_Reports_Switch();

	void set_switch_data(std::vector<std::string>& arguments);

	void basic_error();


};

#endif 