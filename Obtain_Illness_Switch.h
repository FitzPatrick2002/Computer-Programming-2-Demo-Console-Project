#ifndef OBTAIN_ILLNESS_SWITCH
#define OBTAIN_ILLNESS_SWITCH

#include "Base_Switch.h"
#include "Patient.h"
#include "Illness.h"

// This switch is supposed to set the pointer to an illness of specified name 
// Usage: -oil 'name of illness'

class Obtain_Illness_Switch : public Base_Switch {
private:

	Patient* pat_ptr; // We need the pointer to patient to check if specified illness exist
	Illness*& ill_ptr; // This is what we need to set to acces illness from outside

public:

	Obtain_Illness_Switch(Patient* patr, Illness *& ill);

	~Obtain_Illness_Switch();

	void set_switch_data(std::vector<std::string>& args);

};

#endif