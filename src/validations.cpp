#include <string>
#include <regex>
#include <iostream>
#include "functions.hpp"
bool validateName(std::string *name){
    return std::regex_match(name->data(), std::regex("[a-zA-Z]([a-zA-Z0-9]|[-]|[_])*"));
}

bool validateChoice(){
    std::string choice;
    while (std::getline(std::cin, choice))
    {
        trimmer(&choice);
        if(std::regex_match(choice, std::regex("[yYnN]")))
            return (choice.compare("y") == 0 || choice.compare("Y") == 0);    
        else
            std::cout << "[Y/n]: ";   
    }
    abortApp();
    return false;
}

std::string getProjectName(){
    std::string pName;
    std::cout << "Enter the name of your new package: ";
    while (std::getline(std::cin, pName))
    {
        trimmer(&pName);
        if(validateName(&pName))
            return pName;
        else
            std::cout << "Please enter a valid name: ";
    }
    abortApp();
    return "";
}

std::string getProjectLang(std::map<std::string, std::string> *langs){
    std::string pLang;
    
    std::cout << "Languages available: \n";
    for(auto lang: *langs){
        std::cout << " * " << lang.first << std::endl;
    }
    std::cout << "Select a lanaguage from the list: ";
    while (getline(std::cin, pLang))
    {
        trimmer(&pLang);
        if(langs->contains(pLang))
            return pLang;
        else
            std::cout << "Try again. \n";
    }
    abortApp();
    return "";
}