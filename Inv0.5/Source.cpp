#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

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

void insert(std::string key, std::string val) {
    variables_keys[pointer] = key;
    variables_vals[pointer] = val;
    pointer += 1;
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

int main(int argc, char* argv[]) {
    if(argc == 1) {
        std::cout << "Using: Innovation.exe [fileName] [options]" << std::endl;
        exit(0);
    }
    // Create a text string, which is used to output the text file
    string myText;

    if(Exists(argv[1])) {
        //
    } else {
        std::cout << "ERROR: File does not exist" << std::endl;
        exit(0);
    }

    // Read from the text file
    ifstream MyReadFile(argv[1]);

    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, myText)) {
        myText = ltrim(myText);
        if(myText.back() == '.') {
            insert(myText.substr(0, myText.length() - 1), "some");
        }
        if(myText == "vars()") {
            std::cout << getVars() << std::endl;
        }
        //if(inString(myText, "=")) {
        //    if(split(myText, "="))
        //}
        //if (myText.rfind(".-", 0) == 0) {
        //    std::cout << count(split(myText, "-")[1]) << std::endl;
        //}
    }
    // Close the file
    MyReadFile.close();

    return 0;
}
