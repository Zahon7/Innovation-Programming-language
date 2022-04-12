#include <windows.h>
#include <iostream>
using namespace std;

#include <sys/stat.h>
int Exists(const char *name)
{
  struct stat   buffer;
  return (stat (name, &buffer) == 0);
}

int main(int argc, char* argv[]) {
	if(not(Exists(".\\InvSource"))) {
		MessageBox(0, "Can't find the InvSource / application data folder (Stopping)", "Error", MB_OK | MB_ICONSTOP);
		exit(0);
	}
	if(not(Exists(".\\InvSource\\Launcher.exe"))) {
		MessageBox(0, "The InvSource\\Launcher.exe wasn't found. Try to reinstalling the program", "Error", MB_OK | MB_ICONSTOP);
		exit(0);
	}
	if(not(Exists(".\\InvSource\\Main.inv"))) {
		MessageBox(0, "The InvSource\\Main.inv wasn't found. Try to reinstalling the program", "Error", MB_OK | MB_ICONSTOP);
		exit(0);
	}
	system("start .\\InvSource\\Launcher.exe .\\InvSource\\Main.inv");
	return 0;
}