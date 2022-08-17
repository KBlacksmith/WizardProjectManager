#include <iostream>
#include <string>
#include <map>
#include <filesystem>
#include <fstream>
#include <regex>

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
bool validateName(std::string *name){
    return std::regex_match(name->data(), std::regex("[a-zA-Z]([a-zA-Z0-9]|[-]|[_])*"));
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

void createProject(std::string lang, std::string name, std::string extension){
    std::map<std::string, std::string> temp{
        {"python", "print(\"Hello World!\")\n"}, 
        {"cpp", "#include <iostream>\n\nint main(int argc, char * argv[]){\n\tstd::cout << \"Hello, world!\\n\";\n\treturn 0;\n}\n"},
        {"java", "public class "+name+"{\n\tpublic static void main(String[] args){\n\t\tSystem.out.println(\"Hello, world!\");\n\t}\n}\n"},
        {"javascript", "//Run with 'deno run "+name+".js\n\nconsole.log(\"Hello World!\")"},
        {"typescript", "//Run with 'deno run "+name+".ts\n\nconsole.log(\"Hello, world!\")"}, 
        {"rust", "fn main() {\n\tprintln!(\"Hello, world!\");\n}"},
        {"sql", "//SQL Project"},
        {"ruby", ""},
        {"c", "#include <stdio.h>\n\nint main(int argc, char * argv[]){\n\tprintf(\"Hello, world!\\n\");\n\treturn 0;\n}"},
        {"dart", "//Compile with dart compile exe "+name+".dart\nvoid main(List<String> args) {\n\tprint(\"Hello, world!\");\n}"},
    };
    std::map<std::string, std::string> bash{
        {"c", "gcc "+name+".c -o "+name},
        {"cpp", "g++ -std=c++2a "+name+".cpp -o "+name},
        {"java", "javac -d ../build/"+name+".java\n#jar -cfe "+name+".jar "+name+" *.class */*.class"},
        {"javascript", "deno run "+name+".js"},
        {"typescript", "deno run "+name+".ts"},
        {"python", "python3 "+name+".py"},
        {"rust", "rustc "+name+".rs\n./"+name},
        {"ruby", ""},
        {"sql", ""},
        {"dart", "dart compile exe "+name+".dart -o "+name+"\n./"+name}
    };
    if(std::regex_match(name, std::regex("[a-zA-Z]([a-zA-Z0-9]|-|_)*"))){
        std::cout << "Creating directory \""+name+"\"\n";
        if(std::filesystem::create_directories(name)){
            std::cout << "Directory created succesfully\n";
			/*
            std::filesystem::create_directories(name+"/src");
            std::filesystem::create_directories(name+"/build/");
            */
            std::ofstream project(name+"/src/"+name+"."+extension);
            project << temp[lang];
            project.close();
            std::ofstream readme(name+"/README.md");
            readme << "Package: " << name;
            readme << "\nCreated with Kenneth's Project Manager\n";
            readme.close();
            std::ofstream bash_script(name+"/src/compile.sh");
            bash_script << "#!/bin/bash\nclear\necho \"Compiling\"\necho \"---------------\"\n";
            bash_script << bash[lang] << std::endl;
            bash_script.close();
            if(!std::filesystem::exists(name+".wiz_settings")){
                std::ofstream settings(name+"/.wiz_settings");
                settings << extension;
                settings.close();
            }
            std::cout << "Do you want to open the project in VS Code? [Y/n]: ";
            if(validateChoice()){
                std::string code = "code " + name+"/";
                system(code.c_str());
            }
            return;
        }
        std::cout << "Directory already exists\n";
    }
    else
        std::cout << "Invalid project name. \n";
}

void createComponent(std::string component){
    std::string ext {""};
    std::ifstream extension(".wiz_settings");
    getline(extension, ext);
    extension.close();
    if(std::filesystem::exists(component+"."+ext) == 0){
        std::ofstream componentFile(component+"."+ext);
        std::cout << "\""+component+"\" created succesfully.\n";
        componentFile.close();
    }
    else{
        std::cout << "File \""+component+"."+ext+"\" already exists.\n";
    }
}

void help(){
    std::cout << "Usage: wizard [option]\n";
    std::cout << "Available commands: \n";
    std::cout << "\tconjure\tCreate a new package for a project\n";
    std::cout << "\tspell\tCreate new component file\n";
    std::cout << "\thelp\tDisplay list of commands and options\n";
    std::cout << "\tlist\tLists supported programing languages\n";
    std::cout << "\tcharm\tInitializes directory as wizard project\n";
    //std::cout << "\tsettings\n";
    //std::cout << "\ttree\tProject tree\n";
}

int main(int argc, char * argv[]){
    std::map<std::string, std::string> all_langs {
        {"python", "py"}, {"cpp", "cpp"}, {"ruby", "rb"}, {"java", "java"},
        {"c", "c"}, {"dart", "dart"}, {"javascript", "js"}, {"typescript", "ts"}, 
        {"rust", "rs"}, {"sql", "sql"}
    };
    std::string name, lang;
    switch (argc)
    {
    case 1:
        {
            help();
        }
        break;
    case 2:
        {
            std::string command = argv[1];
            if(command.compare("conjure") == 0){
                std::cout << "Welcome to the Kenneth Wizard App\n";
                name.assign(getProjectName());
                if(!name.empty()){
                    lang.assign(getProjectLang(&all_langs));
                    if(!lang.empty()){
                        std::cout << "Package name: " << name << "\nLanguage: " << lang;
                        std::cout << "\nCreate package \"" << name << "\"? [Y/n]: ";
                        if(validateChoice()){
                            createProject(lang, name, all_langs[lang]);
                        }
                    }
                }
            }
            else if(command.compare("help") == 0){
                help();
            }
            else if (command.compare("list") == 0){
                std::cout << "Supported languages: \n";
                for(auto lang:all_langs){
                    std::cout << "\t* " << lang.first << std::endl;
                }
            }
            else if(command.compare("spell") == 0){
                std::cout << "Create component\n";
                if(std::filesystem::exists(".wiz_settings")){
                    std::cout << "Component name: ";
                    getline(std::cin, name);
                    trimmer(&name);
                    if(validateName(&name))
                        createComponent(name);
                    else
                        std::cout << "Invalid component name. \n";
                }
                else{
                    std::cout << "You are not in a Wizard project directory\n";
                }
            }
            else{
                std::cout << "wizard: unrecognized option '" << command << "'\n";
                std::cout << "Try 'wizard help' for more information and the list of available commands\n";
            }
        }
        break;
    case 3: 
        {
            std::string command = argv[1], argument = argv[2];
            if(command.compare("conjure") == 0){
                if(argument.compare("help") == 0){
                    std::cout << "wizard: option 'conjure'\n";
                    std::cout << "Usage: wizard conjure [name [programming language]]\n";
                }
                else {
                    name.assign(argument);
                    if(validateName(&name)){
                        lang.assign(getProjectLang(&all_langs));
                        if(!lang.empty()){
                            std::cout << "Package name: " << name << "\nLanguage: " << lang;
                            std::cout << "\nCreate package \"" << name << "\"? Y/n: ";
                            if(validateChoice()){
                                createProject(lang, name, all_langs[lang]);
                            }
                        }
                    }
                    else
                        std::cout << "Invalid project name. \n";
                }
            }
            else if(command.compare("spell") == 0){
                if(argument.compare("help") == 0){
                    std::cout << "wizard: option 'spell'\n";
                    std::cout << "Usage: wizard spell [name]\n";
                }
                else{
                    std::cout << "Create component\n";
                    if(std::filesystem::exists(".wiz_settings")){
                        if(validateName(&argument))
                            createComponent(argument);
                        else
                            std::cout << "Invalid component name. \n";
                    }
                    else{
                        std::cout << "You are not in a Wizard project directory\n";
                    }
                }
            }
            else if(command.compare("help") == 0 || command.compare("list") == 0){
                std::cout << "wizard: too many arguments for '"+command+"' option. \n";
            }
            else{
                std::cout << "wizard: unrecognized option '" << command << "'\n";
                std::cout << "Try 'wizard help' for more information and the list of available commands\n";
            }
        }
        break;
    case 4:
        {
            std::string command = argv[1];//, name = argv[2], p_lang = argv[3];
            if(command.compare("conjure")==0){
                name.assign(std::string(argv[2]));
                lang.assign(std::string(argv[3]));
                if(validateName(&name)){
                    if(all_langs.contains(lang)){
                        std::cout << "Package name: " << name << "\nLanguage: " << lang;
                        std::cout << "\nCreate package \"" << name << "\"? [Y/n]: ";
                        if(validateChoice()){
                            createProject(lang, name, all_langs[lang]);
                        }
                    }
                    else{
                        std::cout << "Invalid programming language. \nTry 'wizard list' for the full list of supported languages. \n";
                    }
                }
                else{
                    std::cout << "Invalid project name. \n";
                }
            }
            else if(command.compare("spell")==0 || command.compare("help")==0 || command.compare("list")==0){
                std::cout << "wizard: Too many arguments for '"+command+"' option. \n";
            }
            else{
                std::cout << "wizard: unrecognized option '" << command << "'\n";
                std::cout << "Try 'wizard help' for more information and the list of available commands\n";
            }
        }
        break;
    default:
        std::cout << "Too many arguments, try \"help\" for more info. \n";
        break;
    }
    return 0;
}