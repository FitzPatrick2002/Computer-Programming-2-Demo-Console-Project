#ifndef ALL_SWITCH
#define ALL_SWITCH

#include "Base_Switch.h"
#include "Patient_properites_enum.h"

class All_Switch : public Base_Switch {
private:

public:

	All_Switch();

	~All_Switch();

	virtual void set_switch_data(std::vector<std::string>& arguments);

};

#endif