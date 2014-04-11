// Cygwin-Bash-Runner.cpp : Defines the entry point for the console application.
//
#include "Cygwin-Bash-Runner.h"

using std::endl;
int _tmain(int argc, _TCHAR* argv[])
{
	if(argc < 2)
	{
		cout << "not enough args!";
		cin.get();
		return 1;
	}
	string scriptTarget(argv[1]);

	string execStr("d:\\cygwin\\bin\\bash --login -c \"cd \\\"");

	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDirectory(FILENAME_MAX, cCurrentPath))return errno;
	
	execStr += translated(cCurrentPath);

	execStr += "\\\";  \\\"";
	execStr += translated(scriptTarget);
	execStr += "\\\" ";

	for(int i = 2; i < argc; i++)
	{
		execStr += argv[i];
		execStr += " ";
	}
	cout << "executing " << execStr.c_str() << "\n\n";
	system(execStr.c_str());

	cin.get();
	
	return 0;
}

const string translated(string scriptPath)
{
	//TODO: call cygpath, return output
	return reescape(scriptPath);
}

string reescape(string singlyEscaped)
{
	//backslash pass
	unsigned int minimumIndex = 0;
	while(minimumIndex < singlyEscaped.length())
	{
		minimumIndex = singlyEscaped.find('\\', minimumIndex);
		if(minimumIndex == string::npos)
			break;
		singlyEscaped = singlyEscaped.insert(minimumIndex, "\\");
		minimumIndex+=2;
	}
	return singlyEscaped;
}
