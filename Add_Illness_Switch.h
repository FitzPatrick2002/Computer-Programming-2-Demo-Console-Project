#ifndef ADD_ILLNESS_SWITCH
#define ADD_ILLNESS_SWITCH
#include "Base_Switch.h"
#include "Patient.h"

#include <regex>
#include <tuple>

// Purpose of this switch is to validate the name and time span of the illness as well as check whether this specific patient does not already have this illness
// This switch is used as such: ... -il name: ------ time: ------ .....
// Where ----- are specified name and time

// NOTE. This switch does not handle the descriton of illness (which goes into the General.txt file for each illness)
// NOTE. We do not check if such patient exist for we assume that it has already been done by the -i switch

class Add_Illness_Switch : public Base_Switch {
private:
	std::string& name;
	std::pair<std::string, std::string>& time_span;

	Patient* patient;

	std::string pesel;

public:

	Add_Illness_Switch(std::string& n, std::pair<std::string, std::string>& t_s, Patient* p = nullptr);

	~Add_Illness_Switch();

	void set_switch_data(std::vector<std::string>& arguments);

	

};

#endif // !ADD_ILLNESS_SWITCH
