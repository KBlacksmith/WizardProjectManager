#include <string>
#include <regex>
#include <iostream>
#include "functions.hpp"

bool validateName(std::string *name);

bool validateChoice();

std::string getProjectName();

std::string getProjectLang(std::map<std::string, std::string> *langs);