#include <iostream>
#include <string>
#include <map>
#include <filesystem>
#include <fstream>
#include <regex>
#include "functions.hpp"
#include "help.hpp"
#include "validations.hpp"


void createProject(std::string name, std::string extension){
    std::map<std::string, std::string> temp{
        {"py", "print(\"Hello World!\")\n"}, 
        {"cpp", "#include <iostream>\n\nint main(int argc, char * argv[]){\n\tstd::cout << \"Hello, world!\\n\";\n\treturn 0;\n}\n"},
        {"java", "public class "+name+"{\n\tpublic static void main(String[] args){\n\t\tSystem.out.println(\"Hello, world!\");\n\t}\n}\n"},
        {"js", "//Run with 'deno run "+name+".js\n\nconsole.log(\"Hello World!\")"},
        {"ts", "//Run with 'deno run "+name+".ts\n\nconsole.log(\"Hello, world!\")"}, 
        //{"rs", "fn main() {\n\tprintln!(\"Hello, world!\");\n}"},
        //{"sql", "//SQL Project"},
        //{"rb", ""},
        {"c", "#include <stdio.h>\n\nint main(int argc, char * argv[]){\n\tprintf(\"Hello, world!\\n\");\n\treturn 0;\n}"},
        //{"dart", "//Compile with dart compile exe "+name+".dart\nvoid main(List<String> args) {\n\tprint(\"Hello, world!\");\n}"},
    };
    std::map<std::string, std::string> bash{
        {"c", "gcc "+name+".c -o ../build/"+name},
        {"cpp", "g++ -std=c++2a "+name+".cpp -o ../build/"+name},
        {"java", "javac *.java\n#jar -cfe ../build/"+name+".jar "+name+" *.class #*/*.class"},
        {"js", "deno run "+name+".js"},
        {"ts", "deno run "+name+".ts"},
        {"py", "python3 "+name+".py"},
        //{"rs", "rustc "+name+".rs\n./"+name},
        //{"rb", ""},
        //{"sql", ""},
        //{"dart", "dart compile exe "+name+".dart -o "+name+"\n./"+name}
    };
    if(std::regex_match(name, std::regex("[a-zA-Z]([a-zA-Z0-9]|-|_)*"))){
        std::cout << "Creating directory \""+name+"\"\n";
        if(std::filesystem::create_directories(name)){
            std::cout << "Directory created succesfully\n";
            std::filesystem::create_directories(name+"/src");
            std::filesystem::create_directories(name+"/build/");
            std::ofstream project(name+"/src/"+name+"."+extension);
            project << temp[extension];
            project.close();
            std::ofstream readme(name+"/README.md");
            readme << "Package: " << name;
            readme << "\nCreated with Kenneth's Project Manager\n";
            readme.close();
            std::ofstream bash_script(name+"/src/compile.sh");
            bash_script << "#!/bin/bash\nclear\necho \"Compiling\"\necho \"---------------\"\n";
            bash_script << bash[extension] << std::endl;
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
    std::ifstream extension("../.wiz_settings");
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

int main(int argc, char * argv[]){
    std::map<std::string, std::string> extensions {
        {"python", "py"}, {"cpp", "cpp"}, /*{"ruby", "rb"},*/ {"java", "java"},
        {"c", "c"}, /*{"dart", "dart"}, */{"javascript", "js"}, {"typescript", "ts"}, 
        /*{"rust", "rs"}, {"sql", "sql"}*/
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
                    lang.assign(getProjectLang(&extensions));
                    if(!lang.empty()){
                        std::cout << "Package name: " << name << "\nLanguage: " << lang;
                        std::cout << "\nCreate package \"" << name << "\"? [Y/n]: ";
                        if(validateChoice()){
                            createProject(name, extensions[lang]);
                        }
                    }
                }
            }
            else if(command.compare("help") == 0){
                help();
            }
            else if (command.compare("list") == 0){
                std::cout << "Supported languages: \n";
                for(auto lang:extensions){
                    std::cout << "\t* " << lang.first << std::endl;
                }
            }
            else if(command.compare("spell") == 0){
                std::cout << "Create component\n";
                if(std::filesystem::exists("../.wiz_settings")){
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
                        lang.assign(getProjectLang(&extensions));
                        if(!lang.empty()){
                            std::cout << "Package name: " << name << "\nLanguage: " << lang;
                            std::cout << "\nCreate package \"" << name << "\"? Y/n: ";
                            if(validateChoice()){
                                createProject(name, extensions[lang]);
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
                    if(extensions.contains(lang)){
                        std::cout << "Package name: " << name << "\nLanguage: " << lang;
                        std::cout << "\nCreate package \"" << name << "\"? [Y/n]: ";
                        if(validateChoice()){
                            createProject(name, extensions[lang]);
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