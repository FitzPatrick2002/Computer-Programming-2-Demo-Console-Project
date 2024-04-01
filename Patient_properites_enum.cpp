#include "Patient_properites_enum.h"

patient_properites_enum convert_pat_prop_str_to_enum(std::string prop) {
	// Convert the string to lowercase
	for (auto& letter : prop)
		letter = std::tolower(letter);

	if (prop == "name")
		return patient_properites_enum::name;
	else if (prop == "second name" || prop == "second_name")
		return patient_properites_enum::second_name;
	else if (prop == "age")
		return patient_properites_enum::age;
	else if (prop == "pesel")
		return patient_properites_enum::PESEL;
	else if (prop == "gender")
		return patient_properites_enum::gender;
	else if (prop == "illness")
		return patient_properites_enum::illness;
	else if (prop == "personal")
		return patient_properites_enum::personal;
	else
		return patient_properites_enum::error;
}

std::string convert_pat_prop_enum_to_str(patient_properites_enum prop) {

	switch (prop) {
	case patient_properites_enum::name:
		return "name";
		break;
	case patient_properites_enum::age:
		return "age";
		break;
	case patient_properites_enum::second_name:
		return "second_name";
		break;
	case patient_properites_enum::gender:
		return "gender";
		break;
	case patient_properites_enum::PESEL:
		return "pesel";
		break;
	case patient_properites_enum::personal:
		return "personal";
		break;
	case patient_properites_enum::illness:
		return "illness";
		break;
	default:
		return "error";
		break;
	}

}