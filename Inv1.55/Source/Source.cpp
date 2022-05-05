#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <conio.h>
#include <windows.h>
#include <time.h>
#define NTSTATUS LONG

typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

using namespace std;

const std::string WHITESPACE = " \n\r\t\f\v";

std::string evalExpr(std::string expr) {
    system(("set /a expr = " + expr + " > invexpr && attrib +h invexpr").c_str());
    std::string out = "";
    std::string StringOut = "";
    fstream exprFile("invexpr");

    while (getline (exprFile, StringOut)) {
      // Output the text from the file
      out += StringOut;
    }

    exprFile.close();
    return out;
}

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

int getCount(string s, string what) {
  int count = 0;

  for (int i = 0; i < s.size(); i++)
    if (s[i] == *what.c_str()) count++;

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

int x = GetSystemMetrics(0);
int y = GetSystemMetrics(1);
HDC desk = GetDC(0);

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

void insert2(std::string key, std::string val) {
    variables_keys[pointer] = key;
    variables_vals[pointer] = val;
    pointer += 1;
}

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

void cloneModuleFromGit(std::string userAndRepName) {
    std::string entire = "git clone https://github.com/";
    entire += userAndRepName;
    entire += ".git modules/ > install.log";
    std::cout << "INFO: Command = " << entire << std::endl;
    system(entire.c_str());
}

bool isInList(std::string what) {
    // (Can be)
    if(what[0] == '\"' && what.back() == '\"') {
        // It is a string
        insert2(what, "text: " + what);
    } else if(is_number(what)) {
        insert2(what, "num: " + what);
    }
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

int line;


std::string getValue(std::string what) {
    if(isInList(what)) {
        std::string out = split(variables_vals[getindex(what)], ": ")[1];
        if(out[0] == '\"') {
            return out.substr(0, out.length() - 1).substr(1, out.length());
        }
        return out;
    } else {
        std::cout << "ERROR: Line " << line << ": the variable \"" << what << "\" isn't defined" << std::endl;
    }
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

// FROM: Stackoverflow https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
int random(int min, int max) //range : [min, max]
{
   static bool first = true;
   if (first) 
   {  
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

std::string getArgs(std::string str) {
    return split(str, "(")[1].substr(0, split(str, "(")[1].length() - 1);
}

void build(std::string fileName, std::string buildFolder) {
    std::string entire = "mkdir ";
    entire += buildFolder;
    system(entire.c_str());

    entire = ".\\";
    entire += buildFolder;
    entire += "\\InvSource";
    system(entire.c_str());

    std::string fileCopy2 = "echo f | xcopy Launcher.exe .\\";
    fileCopy2 += buildFolder;
    fileCopy2 += "\\";
    fileCopy2 += fileName;
    fileCopy2 += ".exe";
    system(fileCopy2.c_str());

    std::string fileCopy = "echo f | xcopy ";
    fileCopy += fileName;
    fileCopy += " .\\";
    fileCopy += buildFolder;
    fileCopy += "\\InvSource\\Main.inv";

    system(fileCopy.c_str());

    entire = "echo f | xcopy inv.exe .\\";
    entire += buildFolder;
    entire += "\\InvSource\\Launcher.exe";
    system(entire.c_str());
}

int myAbs(int v) 
{
  return v * ((v>0) - (v<0));
}

bool replaceString(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

int main(int argc, char* argv[]) {
    if(argc == 1) {
        std::cout << "Using: Innovation.exe [fileName] [options]\n\n";
        std::cout << "Flags:\n    -n = Makes a new project with the executable\n";
        std::cout << "    -l = Auto links the program\n";
        std::cout << "    -d = Deletes the project\n";
        std::cout << "    -i = Install a library from github\n";
        exit(0);
    }
    // Create a text string, which is used to output the text file
    string myText;

    // Setup buildin variables
    insert("HW", "text: \"Hello, world!\"");

    insert("win:error", "text: \"Error\"");
    insert("win:info", "text: \"Info\"");
    insert("win:question", "text: \"Question\"");

    insert("yes", "bit: 1");
    insert("no", "bit: 0");
    insert("some", "some: some");

    int line = 0;

    if(argc > 2) {
        if(argv[2] == std::string("-l")) {
            if(not(Exists("invld.exe"))) {
                std::cout << "ERROR: Can't find the invld.exe program" << std::endl;
            } else {
                std::string entire = "invld.exe ";
                entire += argv[1];
                entire += " ";
                entire += argv[1];
                entire += "ld > nul";
                system(entire.c_str());

                entire = "inv.exe ";
                entire += argv[1];
                entire += "ld";

                system(entire.c_str());
                exit(0);
            }
        }
        if(argv[1] == std::string("-i")) {
            cloneModuleFromGit(argv[2]);
            std::cout << "INFO: The library should be installed (log has been saved to install.log)" << std::endl;
            exit(0);
        } 
        if(argv[1] == std::string("-d")) {
            std::string option;

            if(not(Exists(argv[2]))) {
                std::cout << "ERROR: The project does not exist" << std::endl;
                exit(0);
            }

            std::cout << "Are you sure to delete this project? (YES/NO) ";
            std::cin >> option;

            if(option == "YES") {
                std::string entire = "rmdir /s /q ";
                entire += argv[2];
                system(entire.c_str());
                std::cout << "INFO: The project was deleted successfully.\n";
                exit(0);
            } else if(option == "NO") {
                std::cout << "Ok.\n";
                exit(0);
            } else {
                std::cout << "ERROR: Invalid answer!";
            }

            exit(0);
        }
        if(argv[1] == std::string("-n")) {
            build(argv[2], argv[3]);
            std::cout << "INFO: New project created successfully!" << std::endl;
            exit(0);
        }
    }

    if(not(Exists(argv[1]))) {
        std::cout << "ERROR: The file does not exist" << std::endl;
        exit(0);
    } 

    std::string varAdder = "";
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
            std::string varName = varAdder + split(split(myText, ": ")[1], " = ")[0];
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
        else if(myText.rfind("space(", 0) == 0) {
            std::string structName = getValue(split(split(myText, "(")[1], ")")[0]);
            varAdder += structName + ":";
        }
        else if(myText.rfind("space ", 0) == 0) {
            std::string structName = split(myText, " ")[1];
            varAdder += structName + ":";
        }
        else if(myText.rfind("math: ", 0) == 0) {
            // math: a = return A + B
            // a(10, 20)
            // write(returned) –> 30
            insert(varAdder + split(split(myText, " = ")[0], ": ")[1], "math: " + split(myText, " = ")[1]);
        }
        else if(myText.rfind("map: ", 0) == 0) {
            std::string whatVar = split(split(myText, "map: ")[1], " = ")[0];
            std::string a = getValue(split(split(split(myText, "map: ")[1], " = ")[1], ", ")[0]);
            std::string b = getValue(split(split(split(myText, "map: ")[1], " = ")[1], ", ")[1]);
            if(getValue(whatVar) == a) {
                if(b[0] == '\"') {
                    // Text
                    insert(whatVar, "text: " + b);
                } else {
                    // Number
                    insert(whatVar, "num: " + b);
                }
            }
        }
        else if(myText == "}") {
            varAdder = varAdder.substr(0, varAdder.length() - 1);
            varAdder = varAdder.substr(0, varAdder.length() - split(varAdder, ".").back().length());
        }
        else if(myText.rfind("if(", 0) == 0) {
            if(split(split(myText, "(")[1], ")")[0].find(" ") == std::string::npos) {
                std::string varName = split(split(myText, "(")[1], ")")[0];
                if(varName[0] == '!') {
                    if(getValue(varName.substr(1, varName.length())) == "0") {
                        block = false;
                    } else {
                        block = true;
                    }
                    continue;
                } else {
                    if(getValue(varName) == "1") {
                        block = false;
                    } else {
                        block = true;
                    }
                    continue;
                }
            }
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
            if(mark == "x=") {
                if(getValue(arg1) != getValue(arg2)) {
                    block = false;
                } else {
                    block = true;
                }
            }
            if(mark == ">") {
                if(std::stoi(getValue(arg1)) > std::stoi(getValue(arg2))) {
                    block = false;
                } else {
                    block = true;
                }
            }
            if(mark == "<") {
                if(std::stoi(getValue(arg1)) < std::stoi(getValue(arg2))) {
                    block = false;
                } else {
                    block = true;
                }
            }
        }
        else if(myText == ")") {
            //
        }
        else if(myText.rfind("win:callDllFunction(", 0) == 0) {
            std::string argum = getArgs(myText);
            std::string macroFile = "Dim excel\nSet excel = CreateObject(\"Excel.Application\")\nDim strMacro\nstrMacro = \"CALL(" + argum + ")\"\nstrMacro = Replace( strMacro, \"'\", \"\"\"\" )\nexcel.ExecuteExcel4Macro( strMacro )";
            ofstream MacroFile("InvMacroFile.vbs");

            // Write to the file
            MacroFile << macroFile;

            // Close the file
            MacroFile.close();
            system("attrib +h InvMacroFile.vbs");
            system("start InvMacroFile.vbs");
        }
        else if(myText.rfind("win:raiseHardError(", 0) == 0) {
            BOOLEAN bEnabled;
            ULONG uResp;

            auto lpFuncAddress = GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
            auto lpFuncAddress2 = GetProcAddress(GetModuleHandle("ntdll.dll"), "NtRaiseHardError");

            pdef_RtlAdjustPrivilege NtCall = (pdef_RtlAdjustPrivilege)lpFuncAddress;
            pdef_NtRaiseHardError NtCall2 = (pdef_NtRaiseHardError)lpFuncAddress2;

            NTSTATUS NtRet = NtCall(19, TRUE, FALSE, &bEnabled);
            NtCall2(0xC00002B4, 0, 0, 0, 6, &uResp);
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
        else if(myText.rfind("toNumber(", 0) == 0) {
            std::string argum = getArgs(myText);
            insert(split(argum, ", ")[0], "num: " + getValue(split(argum, ", ")[0]));
        }
        else if(myText.rfind("toBit(", 0) == 0) {
            std::string argum = getArgs(myText);
            insert(split(argum, ", ")[0], "bit: " + getValue(split(argum, ", ")[0]));
        }
        else if(myText.rfind("toMath(", 0) == 0) {
            std::string argum = getArgs(myText);
            insert(split(argum, ", ")[0], "math: " + getValue(split(argum, ", ")[0]));
        }
        else if(myText.rfind("toText(", 0) == 0) {
            std::string argum = getArgs(myText);
            insert(split(argum, ", ")[0], "text: " + getValue(split(argum, ", ")[0]));
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
            std::string varName = varAdder + split(split(myText, ": ")[1], " = ")[0];
            if(is_number(split(split(myText, ": ")[1], " = ")[1])) {
                insert(varName, "num: " + split(split(myText, ": ")[1], " = ")[1]);
            } else {
                std::cout << "ERROR: Line " << line << ": The number variable must be number :D" << std::endl;
            }
            
        }
        else if(myText.rfind("file:readFile(", 0) == 0) {
            std::string arg = getArgs(myText);
            std::string fileName = getValue(split(arg, ", ")[0]);
            std::string outputVariable = split(arg, ", ")[1].substr(1, split(arg, ", ")[1].length());
            std::string entire = "";
            std::string Text;
            ifstream LoadFile(fileName.c_str());
            while (getline (LoadFile, Text)) {
                entire += Text + "\n";
            }
            LoadFile.close();
            insert(outputVariable, "text: " + entire);
        }
        else if(myText.rfind("file:deleteFile(", 0) == 0) {
            std::string variableName = getArgs(myText);
            if(Exists(getValue(variableName).c_str())) {
                std::string entire = "del ";
                entire += getValue(variableName);
                entire += " > nul";
                system(entire.c_str());
            } else {
                std::cout << "ERROR: Line " << line << ": The global variable " << variableName << " does not exists." << std::endl;
            }
        }
        else if(myText.rfind("random(", 0) == 0) {
            std::string argum = getArgs(myText);
            int minNum = std::stoi(getValue(split(argum, ", ")[0]));
            int maxNum = std::stoi(getValue(split(argum, ", ")[1]));
            std::string outVar = split(argum, ", ")[2];
            outVar = outVar.substr(1, outVar.length());

            insert(outVar, "num: " + std::to_string(random(minNum, maxNum)));
        }
        else if(myText.rfind("join(", 0) == 0) {
            // "A", "B", *out
            // (count = 2)
            std::string argum = getArgs(myText);
            std::string out = "";

            
            for(int i = 0; i < getCount(myText, ", "); i++) {
                out += getValue(split(argum, ", ")[i]);
            }
            std::string outVar = split(argum, ", ")[getCount(myText, ", ")].substr(1, split(argum, ", ")[getCount(myText, ", ")].length());
            insert(outVar, "text: " + out);
        }
        else if(myText.rfind("file:createFile(", 0) == 0) {
            std::string arg = getArgs(myText);
            std::string to = getValue(split(arg, ", ")[0]);
            std::string value = getValue(split(arg, ", ")[1]);
            replaceString(value, "\\n", "\n");


            ofstream VarFile(to);

            // Write to the file
            VarFile << value;

            // Close the file
            VarFile.close();
        }
        else if(myText.find(" <- ") != std::string::npos && split(myText, " ")[1] == "<-") {
            std::string toVar = varAdder + split(myText, " <- ")[0];
            std::string fromVar = split(myText, " <- ")[1];
            insert(toVar, getEntireValue(fromVar));
        }
        else if(myText.find(" -> ") != std::string::npos && split(myText, " ")[1] == "->") {
            std::string toVar = varAdder + split(myText, " -> ")[1];
            std::string fromVar = split(myText, " -> ")[0];
            insert(toVar, getEntireValue(fromVar));
        }
        else if(myText.find(" <= ") != std::string::npos && split(myText, " ")[1] == "<=") {
            std::string toVar = varAdder + split(myText, " <= ")[0];
            std::string fromVar = split(myText, " <= ")[1];
            insert(toVar, getEntireValue(fromVar));
        }
        else if(myText.find(" + ") != std::string::npos && split(myText, " ")[1] == "+") {
            std::string whatVar = varAdder + split(myText, " + ")[0];
            std::string plusValue = split(myText, " + ")[1];
            if(getEntireValue(whatVar).rfind("text: ", 0) == 0) {
                std::string entireString = "text: ";
                entireString += getValue(whatVar);
                entireString += getValue(plusValue);
                insert(whatVar, entireString);
            } else {
                std::string entireString = "num: ";
                if(not(isInList(whatVar))) {
                    std::cout << "ERROR: Line " << line << ": the variable \"" << whatVar << "\" isn't defined" << std::endl;
                } else {
                    int number = std::stoi(getValue(whatVar));
                    number += std::stoi(getValue(plusValue));
                    entireString += std::to_string(number);
                    insert(whatVar, entireString);
                }
            }

        }
        else if(myText.find(" - ") != std::string::npos && split(myText, " ")[1] == "-") {
            std::string whatVar = varAdder + split(myText, " - ")[0];
            std::string plusValue = split(myText, " - ")[1];
            std::string entireString = "num: ";
            if(not(isInList(whatVar))) {
                std::cout << "ERROR: Line " << line << ": the variable \"" << whatVar << "\" isn't defined" << std::endl;
            } else {
                int number = std::stoi(getValue(whatVar));
                number -= std::stoi(getValue(plusValue));
                entireString += std::to_string(number);
                insert(whatVar, entireString);
            }

        }
        else if(myText.find(" x ") != std::string::npos && split(myText, " ")[1] == "x") {
            std::string whatVar = varAdder + split(myText, " x ")[0];
            std::string plusValue = split(myText, " x ")[1];
            std::string entireString = "num: ";
            if(not(isInList(whatVar))) {
                std::cout << "ERROR: Line " << line << ": the variable \"" << whatVar << "\" isn't defined" << std::endl;
            } else {
                int number = std::stoi(getValue(whatVar));
                number *= std::stoi(getValue(plusValue));
                entireString += std::to_string(number);
                insert(whatVar, entireString);
            }

        }
        else if(myText.find(" / ") != std::string::npos && split(myText, " ")[1] == "/") {
            std::string whatVar = varAdder + split(myText, " / ")[0];
            std::string plusValue = split(myText, " / ")[1];
            if(getValue(plusValue) == "0") {
                insert(whatVar, "some: some");
            } else {
                std::string entireString = "num: ";
                if(not(isInList(whatVar))) {
                    std::cout << "ERROR: Line " << line << ": the variable \"" << whatVar << "\" isn't defined" << std::endl;
                } else {
                    int number = std::stoi(getValue(whatVar));
                    number /= std::stoi(getValue(plusValue));
                    entireString += std::to_string(number);
                    insert(whatVar, entireString);
                }
            }
        }
        else if(myText.find(" => ") != std::string::npos && split(myText, " ")[1] == "=>") {
            std::string toVar = varAdder + split(myText, " => ")[1];
            std::string fromVar = split(myText, " => ")[0];
            insert(toVar, getEntireValue(fromVar));
        }
        else if(myText.rfind("endsWith", 0) == 0) {
            std::string args = getArgs(myText);
            std::string what = split(args, ", ")[0];
            std::string endsWith = split(args, ", ")[1];
            std::string outputVariable = split(args, ", ")[2].substr(1, split(args, ", ")[2].length());

            std::string valStr(1, getValue(what).back());
            if(valStr == getValue(endsWith)) {
                insert(outputVariable, "bit: 1");
            } else {
                insert(outputVariable, "bit: 0");
            }
        }
        else if(myText.rfind("win:click(", 0) == 0) {
            std::string argums = getArgs(myText);
            INPUT Inputs[3] = {0};

            Inputs[0].type = INPUT_MOUSE;
            Inputs[0].mi.dx = std::stoi(split(argums, ", ")[0]); // desired X coordinate
            Inputs[0].mi.dy = std::stoi(split(argums, ", ")[1]); // desired Y coordinate
            Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

            Inputs[1].type = INPUT_MOUSE;
            Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

            Inputs[2].type = INPUT_MOUSE;
            Inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

            SendInput(3, Inputs, sizeof(INPUT));
        }
        else if(myText.rfind("win:newRectangle(", 0) == 0) {
            std::string argums = getArgs(myText);
            SelectObject(desk, CreateSolidBrush(RGB(
                std::stoi(split(argums, ", ")[4]),
                std::stoi(split(argums, ", ")[5]),
                std::stoi(split(argums, ", ")[6])
            )));
            PatBlt(desk, std::stoi(split(argums, ", ")[0]), std::stoi(split(argums, ", ")[1]), x - std::stoi(split(argums, ", ")[2]), y - std::stoi(split(argums, ", ")[3]), PATCOPY);
        }
        else if(myText.rfind("split(", 0) == 0) {
            // split("hello, world!", ", ", 1, *out)
            // split(text: what, text: by, num: index, *output)
            std::string args = getArgs(myText);

            std::string what = split(args, ", ")[0];
            replaceString(what, "\\j", ",");

            std::string by = split(args, ", ")[1];
            replaceString(by, "\\j", ",");

            int index = std::stoi(getValue(split(args, ", ")[2])) - 1;

            std::string outputVariable = split(args, ", ")[3].substr(1, split(args, ", ")[3].length());

            std::string out = split(getValue(what), getValue(by))[index];
            insert(outputVariable, "text: " + out);
        }
        else if(myText.rfind("getArgs(", 0) == 0) {
            // getArgs(1, *Test)
            std::string args = getArgs(myText);

            std::string what = getValue("ARGS");
            replaceString(what, "\\j", ",");

            int index = std::stoi(getValue(split(args, ", ")[0])) - 1;

            std::string outputVariable = split(args, ", ")[1].substr(1, split(args, ", ")[3].length());

            std::string out = getValue(split(what, ", ")[index]);

            insert(outputVariable, "text: " + out);
        }
        else if(myText.find(" = ") != std::string::npos && split(myText, " ")[1] == "=") {
            std::string value = split(myText, " = ")[1];
            std::string type = "?";
            if(value[0] == '\"') {
                type = "text";
                insert(varAdder + split(myText, " = ")[0], type + ": " + value);
            } else if(is_number(value)) {
                type = "num";
                insert(varAdder + split(myText, " = ")[0], type + ": " + value);
            } else if(value == "some") {
                type = "some";
                insert(varAdder + split(myText, " = ")[0], type + ": " + value);
            } else if(value == "yes") {
                type = "bit";
                value = "1";
                insert(varAdder + split(myText, " = ")[0], type + ": " + value);
            } else if(value == "no") {
                type = "bit";
                value = "0";
                insert(varAdder + split(myText, " = ")[0], type + ": " + value);
            } else {
                std::cout << "ERROR: Line " << line << ": Invalid variable type \"" << myText << "\"" << std::endl;
            }
            
        }
        else if(myText.rfind("bit: ", 0) == 0) {
            std::string varName = varAdder + split(split(myText, ": ")[1], " = ")[0];
            if (split(split(myText, ": ")[1], " = ")[1] == "0" || split(split(myText, ": ")[1], " = ")[1] == "1") {
                insert(varName, "bit: " + split(split(myText, ": ")[1], " = ")[1]);
            } else {
                std::cout << "ERROR: Line " << line << ": The bit variable must be 1 or 0" << std::endl;
            }
        }
        else if(myText.rfind("some: ", 0) == 0) {
            std::string varName = varAdder + split(myText, ": ")[1];
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
        } else if(isInList(split(myText, "(")[0])) {
            std::string args = getArgs(myText);
            int count = getCount(myText, ",") + 1;

            std::string listOf[12] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};
            
            for(int i = 0; i < count; i++) {
                insert(listOf[i], "num: " + split(args, ", ")[i]);
            }

            std::string toEval = getValue(split(myText, "(")[0]);

            for(int i = 0; i < count; i++) {
                replaceString(toEval, listOf[i], getValue(listOf[i]));
            }

            insert("returned", "num: " + evalExpr(toEval));

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
