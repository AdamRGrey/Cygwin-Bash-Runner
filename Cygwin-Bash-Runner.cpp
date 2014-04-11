// Cygwin-Bash-Runner.cpp : Defines the entry point for the console application.
//
#include "Cygwin-Bash-Runner.h"
int _tmain(int argc, _TCHAR* argv[])
{
	if(argc < 2)
	{
		cout << "not enough args!";
		cin.get();
		return 1;
	}
	string scriptTarget(argv[1]);

	string execStr("d:\\cygwin\\bin\\bash --login -c \"cd \"");
	cout << execStr << "\n";

	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDirectory(FILENAME_MAX, cCurrentPath))return errno;
	
	cout << "current path = " << cCurrentPath << "\n";
	execStr += cCurrentPath;
	cout << execStr << "\n";

	execStr += "\";  \"";
	cout << execStr << "\n";
	execStr += translated(scriptTarget);
	cout << execStr << "\n";
	execStr += "\" ";
	cout << execStr << "\n";

	for(int i = 2; i < argc; i++)
	{
		execStr += argv[i];
		execStr += " ";
		cout << execStr << "\n";
	}
	cout << "executing " << execStr.c_str() << "\n\n";
	system(execStr.c_str());

	cin.get();
	
	return 0;
}

const char* translated(string scriptPath)
{
	//TODO: call cygpath, return output
	return scriptPath.c_str();
}