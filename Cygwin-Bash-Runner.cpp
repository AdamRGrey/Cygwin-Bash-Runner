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

	string execStr("d:\\cygwin\\bin\\bash --login -c \"cd \"");

	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDirectory(FILENAME_MAX, cCurrentPath))return errno;

	execStr += translated(cCurrentPath);

	execStr += "\";  '";
	execStr += translated(scriptTarget);
	execStr += "' ";

	for(int i = 2; i < argc; i++)
	{
		execStr += argv[i];
		execStr += " ";
	}
	cout << "executing " << execStr.c_str() << "\n\n";
	system(execStr.c_str());
	cout << "end of system call \n";

	cin.get();

	return 0;
}
#define BUFSIZE 4096
const string cygpath("D:\\cygwin\\bin\\cygpath.exe");
const string translated(string scriptPath)
{
	//TODO: call cygpath, return output

	//default security options. basically just inherit my own security
	SECURITY_ATTRIBUTES sec;
	sec.nLength = sizeof(SECURITY_ATTRIBUTES);//TODO: mess around with intentionally setting the wrong value to nLength :P
	sec.bInheritHandle = TRUE; 
	sec.lpSecurityDescriptor = NULL; 


	HANDLE inPipeRead = NULL;
	HANDLE inPipeWrite = NULL;
	HANDLE outPipeRead = NULL;
	HANDLE outPipeWrite = NULL;

	if(!CreatePipe(&outPipeRead, &outPipeWrite, &sec, 0))
		throw exception("couldn't create outpipe");
	if(!CreatePipe(&inPipeRead, &inPipeWrite, &sec, 0))
		throw exception("couldn't create inpipe");

	if ( ! SetHandleInformation(outPipeRead, HANDLE_FLAG_INHERIT, 0) )
		throw exception("outpipe couldn't disallow inheritance");
	if ( ! SetHandleInformation(inPipeWrite, HANDLE_FLAG_INHERIT, 0) )
		throw exception("inpipe couldn't disallow inheritance");

	STARTUPINFO startInfo; 
	ZeroMemory( &startInfo, sizeof(STARTUPINFO) );
	startInfo.cb = sizeof(STARTUPINFO);
	startInfo.hStdError = outPipeWrite;
	startInfo.hStdOutput = outPipeWrite;
	startInfo.hStdInput = inPipeRead;
	startInfo.dwFlags |= STARTF_USESTDHANDLES;

	PROCESS_INFORMATION procInf; 
	ZeroMemory( &procInf, sizeof(PROCESS_INFORMATION) );
	string myString(cygpath.c_str());
	myString += " ";
	myString += scriptPath;
	if(!CreateProcess(NULL, TEXT(const_cast<char*>(myString.c_str())), NULL, NULL, TRUE, 0, NULL, NULL, &startInfo, &procInf))
		throw exception("couldn't create process");

	DWORD exitCode;
	DWORD bytesRead; 
	CHAR chBuf[BUFSIZE]; 
	string outputText("");
	BOOL readSuccess = FALSE;

	while (true) 
	{ 
		ZeroMemory(chBuf, BUFSIZE);
		
		GetExitCodeProcess(procInf.hProcess, &exitCode);
		
		readSuccess = ReadFile( outPipeRead, chBuf, BUFSIZE, &bytesRead, NULL);
		
		outputText += chBuf;
		

		if(!readSuccess || bytesRead==0 || exitCode != STILL_ACTIVE) break; 
	} 

	
	while(true)
	{
		auto whyAreThereNewlines = outputText.find('\n');
		if(whyAreThereNewlines == string::npos)
			break;
		
		outputText.replace(whyAreThereNewlines, 1, " ");
	}
	CloseHandle(procInf.hProcess);
	CloseHandle(procInf.hThread);
	return reescape(outputText);
}
///<summary>
/// make backslash doublebackslash, since we'll be passing it to bash.exe, which will strip one level.
///</summary>
string reescape(string singlyEscaped)
{
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
