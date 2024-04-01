#ifndef ORDER_SWITCH
#define ORDER_SWITCH

#include "Base_Switch.h"
#include "Patient_properites_enum.h"

// It is a switch -order claaa (initially fro the List_All command / class)
// 

class Order_Switch : public Base_Switch {
private:
	bool &ordered_print;
	bool &ascending_print;
	patient_properites_enum &prop_to_sort_by;

public:

	Order_Switch(bool &ord_pr, bool &asc_pr, patient_properites_enum &pr_sort);

	~Order_Switch();

	virtual void set_switch_data(std::vector<std::string>& arguments);

};

#endif;