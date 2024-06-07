#ifndef ADD_PATIENT_SWITCH
#define ADD_PATIENT_SWITCH

#include "Base_Switch.h"

// All this switch class does, it just checks if there are no arguments passed to the switch
// If there are, it returns an error

// Usage:
// add -p // Doesn;t take any arguments, user is later asked to provide them
// add -i 'pesel' -il name: Illness_Name time: dd.mm.yyyy-dd.mm.yyyy / dd.mm.yyyy-- -> ongoing / '.' can be replaced by '-' 

class Add_Patient_Switch : public Base_Switch{
private:
	bool& no_args_passed;

public:

	Add_Patient_Switch(bool & no_a_p);

	~Add_Patient_Switch();


	void set_switch_data(std::vector<std::string>& arguments);
};

#endif