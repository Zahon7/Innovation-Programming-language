#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <conio.h>
#include <windows.h>

using namespace std;

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

// for string delimiter
vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

#include <sys/stat.h>
int Exists(const char *name)
{
  struct stat   buffer;
  return (stat (name, &buffer) == 0);
}

int count(string s) {
  int count = 0;

  for (int i = 0; i < s.size(); i++)
    if (s[i] == '.') count++;

  return count;
}

bool findInList(char* list[], string what, int lenght) {
    for(int i = 0; i < lenght; i++) {
        if(list[i] == what) {
            return true;
        }
    }
    return false;
}

std::string variables_keys[100] = {};
std::string variables_vals[100] = {};
int pointer = 0;


int getindex(std::string what) {
    for(int i = 0; i < 101; i++) {
        if(variables_keys[i] == what)
            return i;
    }
    return -1;
}

bool isInList(std::string what) {
    for(int i = 0; i < 101; i++) {
        if(what == variables_keys[i]) {
            return true;
        }
    }
    return false;
}

void insert(std::string key, std::string val) {
    if(isInList(key)) {
        variables_vals[getindex(key)] = val;
    } else {
        variables_keys[pointer] = key;
        variables_vals[pointer] = val;
        pointer += 1;
    }
}


std::string getValue(std::string what) {
    std::string out = split(variables_vals[getindex(what)], ": ")[1];
    if(out[0] == '\"') {
        return out.substr(0, out.length() - 1).substr(1, out.length());
    }
    return out;
}

std::string getEntireValue(std::string what) {
    return variables_vals[getindex(what)];
}

std::string getVars() {
    std::string Out = "";
    for(int i = 0; i < pointer + 1; i++) {
        Out += variables_keys[i] + " = " + variables_vals[i] + "\n";
    }
    Out = Out.substr(0, Out.length() - 5);
    return Out;
}

/*
some
text: Hello
num: 10
bit: 0
*/

bool is_number(std::string s)
{
    for(int i = 0; i < s.length(); i++) {
        if(isdigit(s[i])) {
            //
        } else {
            return false;
        }
    }
    return true;
}

std::string getArgs(std::string str) {
    return split(str, "(")[1].substr(0, split(str, "(")[1].length() - 1);
}

void build(std::string fileName) {
    system("mkdir Build");
    system("mkdir .\\Build\\InvSource");

    std::string fileCopy2 = "echo f | xcopy Launcher.exe .\\Build\\";
    fileCopy2 += fileName;
    fileCopy2 += ".exe";
    system(fileCopy2.c_str());

    std::string fileCopy = "echo f | xcopy ";
    fileCopy += fileName;
    fileCopy += " .\\Build\\InvSource\\Main.inv";

    system(fileCopy.c_str());
    system("echo f | xcopy inv.exe .\\Build\\InvSource\\Launcher.exe");
}

int main(int argc, char* argv[]) {
    if(argc == 1) {
        std::cout << "Using: Innovation.exe [fileName] [options]\n\n";
        std::cout << "Flags:\n    -e = Builds the file to exe\n";
        exit(0);
    }
    // Create a text string, which is used to output the text file
    string myText;

    // Setup buildin variables
    insert("HW", "text: \"Hello, world!\"");

    insert("win:ERROR", "text: \"Error\"");
    insert("win:INFO", "text: \"Info\"");
    insert("win:QUESTION", "text: \"Question\"");

    insert("YES", "bit: 1");
    insert("NO", "bit: 0");
    insert("SOME", "some");

    int line = 0;

    if(argc > 2) {
        if(argv[2] == std::string("-e")) {
            build(argv[1]);
            std::cout << "INFO: The exe was successfully built!";
            exit(0);
        }
    }

    if(not(Exists(argv[1]))) {
        std::cout << "ERROR: The file does not exist" << std::endl;
        exit(0);
    } 

    // Read from the text file
    ifstream MyReadFile(argv[1]);
    bool block = false;

    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, myText)) {
        myText = ltrim(myText);
        if(block) {
            if(myText == ")") {
                block = false;
            } else {
                continue;
            }
        }
        if(myText.back() == '.' && not(myText.rfind("\"", 0) == 0)) {
            insert(myText.substr(0, myText.length() - 1), "some: some");
        }
        else if(myText.rfind("text: ", 0) == 0) {
            std::string varName = split(split(myText, ": ")[1], " = ")[0];
            if(split(split(myText, ": ")[1], " = ")[1].rfind("\"", 0) == 0 && myText.back() == '"') {
                insert(varName, "text: " + split(split(myText, ": ")[1], " = ")[1]);
            } else {
                std::cout << "ERROR: Line " << line << ": The text variable must start and end with \"" << std::endl;
            }
        }
        else if(myText.rfind("get(*", 0) == 0) {
            std::string arg = split(myText, "*")[1].substr(0, split(myText, "*")[1].length() - 1);
            std::string out;
            if(isInList(arg)) {
                std::cin >> out;
                variables_vals[getindex(arg)] = "text: " + out;
            } else {
                std::cout << "ERROR: Line " << line << ": the variable \"" << arg << "\" isn't defined" << std::endl;
            }
        }
        else if(myText.rfind("get(", 0) == 0 && not(myText.rfind("get(*", 0) == 0)) {
            std::cout << "ERROR: Line " << line << ": Function get needs a output argument (*)" << std::endl;
        }
        else if(myText.rfind("if(", 0) == 0) {
            std::string arg1 = split(myText, " ")[0].substr(3, split(myText, " ")[0].length());
            std::string mark = split(myText, " ")[1];
            std::string arg2 = split(myText, " ")[2].substr(0, split(myText, " ")[2].length() - 1);
            if(mark == "=") {
                if(getValue(arg1) == getValue(arg2)) {
                    block = false;
                } else {
                    block = true;
                }
            }
        }
        else if(myText == ")") {
            //
        }
        else if(myText.rfind("win:boxInfo(", 0) == 0) {
            std::string argum = getArgs(myText);
            if(not(isInList(split(argum, ", ")[0]))) {
                std::cout << "ERROR: Line " << line << ": the variable \"" << split(argum, ", ")[0] << "\" isn't defined" << std::endl;
            } else {
                MessageBox(0, getValue(split(argum, ", ")[0]).c_str(), getValue(split(argum, ", ")[1]).c_str(), MB_OK);
            }
        }
        else if(myText.rfind("win:boxError(", 0) == 0) {
            std::string argum = getArgs(myText);
            if(not(isInList(split(argum, ", ")[0]))) {
                std::cout << "ERROR: Line " << line << ": the variable \"" << split(argum, ", ")[0] << "\" isn't defined" << std::endl;
            } else {
                MessageBox(0, getValue(split(argum, ", ")[0]).c_str(), getValue(split(argum, ", ")[1]).c_str(), MB_OK | MB_ICONSTOP);
            }
        }
        else if(myText.rfind("\"") && myText.back() == '.') {
            insert(myText.substr(0, myText.length() - 1), "text: " + myText.substr(0, myText.length() - 1));
        }
        else if(myText.rfind("win:boxContinue(", 0) == 0) {
            std::string argum = getArgs(myText);
            if(not(isInList(split(argum, ", ")[0]))) {
                std::cout << "ERROR: Line " << line << ": the variable \"" << split(argum, ", ")[0] << "\" isn't defined" << std::endl;
            } else {
                if(MessageBox(0, getValue(split(argum, ", ")[0]).c_str(), getValue(split(argum, ", ")[1]).c_str(), MB_YESNO) == IDYES) {
                    //
                } else {
                    exit(0);
                }
            }
        }
        else if(myText.rfind("win:execute(", 0) == 0) {
            std::string argum = getArgs(myText);
            system(getValue(split(argum, ", ")[0]).c_str());
        }
        else if(myText == "wait()") {
            std::cout << "Press any key to continue...";
            getch();
        }
        else if(myText.rfind("num: ", 0) == 0) {
            std::string varName = split(split(myText, ": ")[1], " = ")[0];
            if(is_number(split(split(myText, ": ")[1], " = ")[1])) {
                insert(varName, "num: " + split(split(myText, ": ")[1], " = ")[1]);
            } else {
                std::cout << "ERROR: Line " << line << ": The number variable must be number :D" << std::endl;
            }
            
        }
        else if(myText.find(" <- ") != std::string::npos && split(myText, " ")[1] == "<-") {
            std::string toVar = split(myText, " <- ")[0];
            std::string fromVar = split(myText, " <- ")[1];
            insert(toVar, getEntireValue(fromVar));
        }
        else if(myText.find(" = ") != std::string::npos && split(myText, " ")[1] == "=") {
            std::string value = split(myText, " = ")[1];
            std::string type = "?";
            if(value[0] == '\"') {
                type = "text";
                insert(split(myText, " = ")[0], type + ": " + value);
            } else if(is_number(value)) {
                type = "num";
                insert(split(myText, " = ")[0], type + ": " + value);
            } else if(value == "some") {
                type = "some";
                insert(split(myText, " = ")[0], type + ": " + value);
            } else if(value == "yes") {
                type = "bit";
                value = "1";
                insert(split(myText, " = ")[0], type + ": " + value);
            } else if(value == "no") {
                type = "bit";
                value = "0";
                insert(split(myText, " = ")[0], type + ": " + value);
            } else {
                std::cout << "ERROR: Line " << line << ": Invalid variable type \"" << myText << "\"" << std::endl;
            }
            
        }
        else if(myText.rfind("bit: ", 0) == 0) {
            std::string varName = split(split(myText, ": ")[1], " = ")[0];
            if (split(split(myText, ": ")[1], " = ")[1] == "0" || split(split(myText, ": ")[1], " = ")[1] == "1") {
                insert(varName, "bit: " + split(split(myText, ": ")[1], " = ")[1]);
            } else {
                std::cout << "ERROR: Line " << line << ": The bit variable must be 1 or 0" << std::endl;
            }
        }
        else if(myText.rfind("some: ", 0) == 0) {
            std::string varName = split(myText, ": ")[1];
            insert(varName, "some: some");
        }
        else if(myText.rfind("write(", 0) == 0) {
            std::string out = split(myText, "(")[1].substr(0, split(myText, "(")[1].length() - 1);
            if(isInList(out)) {
                std::cout << getValue(out) << std::endl;
            } else {
                std::cout << "ERROR: Line " << line << ": the variable \"" << out << "\" isn't defined" << std::endl;
            }
        }
        else if(myText.rfind("put(", 0) == 0) {
            std::string out = split(myText, "(")[1].substr(0, split(myText, "(")[1].length() - 1);
            if(isInList(out)) {
                std::cout << getValue(out);
            } else {
                std::cout << "ERROR: Line " << line << ": the variable \"" << out << "\" isn't defined" << std::endl;
            }
        }
        else if(myText == "vars()") {
            std::cout << getVars() << std::endl;
        }
        else if(myText == "" || myText.rfind("/", 0) == 0) {
            // Good
        } else {
            std::cout << "ERROR: Line " << line << ": Invalid syntax \"" << myText << "\"" << std::endl;
            exit(0);
        }
        //if(inString(myText, "=")) {
        //    if(split(myText, "="))
        //}
        //if (myText.rfind(".-", 0) == 0) {
        //    std::cout << count(split(myText, "-")[1]) << std::endl;
        //}
        line += 1;
    }
    // Close the file
    MyReadFile.close();

    return 0;
}
