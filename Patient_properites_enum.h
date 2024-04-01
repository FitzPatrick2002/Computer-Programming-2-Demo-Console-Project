#ifndef PATIENT_PROPERTIES_ENUM
#define PATIENT_PROPERTIES_ENUM

#include <string>


enum class patient_properites_enum {
	name,
	second_name,
	age,
	PESEL,
	gender,
	illness,
	personal,
	error
};

patient_properites_enum convert_pat_prop_str_to_enum(std::string prop);

std::string convert_pat_prop_enum_to_str(patient_properites_enum prop);


#endif
