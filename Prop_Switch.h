#ifndef PROP_SWITCH
#define PROP_SWITCH

#include "Base_Switch.h"
#include "Patient_properites_enum.h"

class Prop_Switch : public Base_Switch {
private:
	std::vector<patient_properites_enum>& props_to_print;

public:

	Prop_Switch(std::vector<patient_properites_enum>& ptp);

	~Prop_Switch();

	virtual void set_switch_data(std::vector<std::string>& arguemnts);

};

#endif