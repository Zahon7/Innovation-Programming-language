#include <windows.h>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

string * fromToArray(string *array, int from, int to) {
	string outArray[100] = {};
	int pointer = 0;
	for(int i = from; i < to; i++) {
		outArray[pointer] = array[i];
		pointer++;
	}
	return outArray;
}

int Exists(const char *name)
{
  struct stat   buffer;
  return (stat (name, &buffer) == 0);
}

bool replaceString(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

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

int Count(std::string String, const char* What) {
	int out = 0;
	for(int i = 0; i < String.length(); i++) {
		if(String[i] == *What) {
			out += 1;
		}
	}
	return out;
}

int main(int argc, char* argv[]) {
	if(argc < 3) {
        std::cout << "Using: InvLd.exe [inputFileName] [outputFileName]\n";
        exit(0);
    }
    if(not(Exists(argv[1]))) {
    	std::cout << "ERROR: The file does not exist" << std::endl;
    	exit(0);
    }
    if(not(Exists(argv[2]))) {
    	std::ofstream { argv[2] };
    }

	// Read from the text file
	ifstream ReadFile(argv[1]);
	std::string textLine;
	std::string outLines = "";

	// Use a while loop together with the getline() function to read the file line by line
	while (getline (ReadFile, textLine)) {
	  // Output the text from the file
	  if(textLine == "") {
	  	continue;
	  }
	  if(textLine.rfind("/", 0) == 0) {
	  	continue;
	  }
	  if(textLine.rfind("modules:", 0) == 0) {
	  	std::string args = split(split(textLine, "(")[1], ")")[0];
	  	outLines += "ARGS = \"" + args + "\"\n";
	  	std::string fileName = split(textLine, "(")[0] + ".inv";
	  	std::replace( fileName.begin(), fileName.end(), ':', '/');

	  	std::string textLine2;

	  	ifstream ReadFile2(fileName);
	  	while (getline (ReadFile2, textLine2)) {
	  		outLines += textLine2 + "\n";
	  	}
	  	ReadFile2.close();
	  } else {
		  outLines += textLine;
		  outLines += "\n";
	  }
	}

	// Close the file
	ReadFile.close();

	// Create and open a text file
	ofstream OutFile(argv[2]);

	// Write to the file
	OutFile << outLines;

	// Close the file
	OutFile.close();

	std::cout << "INFO: File successfuly linked!" << std::endl;
	return 0;
}