#include <string>
#include <regex>
#include <iostream>
void trimmer(std::string *arg){
    auto start = 0;
    auto end = arg->length();
    if(arg->compare("")){
        while (arg->substr(start, end-start).starts_with(" "))
        {
            start++;
        }
        while (arg->substr(start, end-start).ends_with(" "))
        {
            end--;
        }
        arg->assign(arg->substr(start, end-start));
    }
}

void abortApp(){
    std::cout << "Abort. \n";
}

