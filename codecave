//Codecave injector made by zer0/zer0memory/Virtualcoder . 

/* NOTE!!! 32bit process can't interfere with 64bit process or vice versa! */
/* You must be using the same architecture as your target process uses!*/
  
   //no need to typedef function and pass parameter. also can (void) return type. 
// ex: int(*msgbox)() = 0x7511314e;
//can also show strings if defined in the injected thread
#define _CRT_SECURE_NO_WARNINGS //let's us use "unsafe" functions in the crt
  
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
  
typedef int(__stdcall *__MessageBoxA)(HWND, LPCSTR, LPCSTR, UINT);
  
class cavedata // store all of our remote data to one object
{
public:
char chMessage[256]; //here we store it's message!
char chTitle[256]; //here we will store our messagebox's title
  
DWORD paMessageBoxA; //pa = Procedure address in memory
  
};
  
DWORD GetProcId(char* procname)
{
PROCESSENTRY32 pe;
HANDLE hSnap;
  
pe.dwSize = sizeof(PROCESSENTRY32);
hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
if (Process32First(hSnap, &pe)) {
do {
if (strcmp(pe.szExeFile, procname) == 0)
break;
} while (Process32Next(hSnap, &pe));
}
return pe.th32ProcessID;
}
  
DWORD __stdcall RemoteThread(cavedata *cData) //thread that will be spawned in our target process
{
__MessageBoxA MsgBox = (__MessageBoxA)cData->paMessageBoxA; //initialize our function
MsgBox(NULL, cData->chMessage, cData->chTitle, MB_ICONINFORMATION); //call it
return EXIT_SUCCESS;
}
  
int main()
{
cavedata CaveData; //declare cave data object
ZeroMemory(&CaveData, sizeof(cavedata)); // fill it with zeros
strcpy(CaveData.chMessage, "Hi, I was called from remote process!"); //set the variables inside it
strcpy(CaveData.chTitle, "Hello from code cave!");
  
HINSTANCE hUserModule = LoadLibrary("user32.dll"); //load the user32.dll
CaveData.paMessageBoxA = (DWORD)GetProcAddress(hUserModule, "MessageBoxA"); //find MessageBox procedure from it
// and pass it to paMessageBox.
FreeLibrary(hUserModule);
  
//open our target process for writing
HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetProcId("calc.exe"));
//allocate memory for our procedure in the remote process' address space
LPVOID pRemoteThread = VirtualAllocEx(hProcess, NULL, sizeof(cavedata), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
//write our thread to the target process
WriteProcessMemory(hProcess, pRemoteThread, (LPVOID)RemoteThread, sizeof(cavedata), 0);
//allocate memory for our cavedata object in the remote process
cavedata *pData = (cavedata*)VirtualAllocEx(hProcess, NULL, sizeof(cavedata), MEM_COMMIT, PAGE_READWRITE);
//Write it to the target process
WriteProcessMemory(hProcess, pData, &CaveData, sizeof(cavedata), NULL);
//spawn/create our procedure/thread in the remote process
HANDLE hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)pRemoteThread, pData, 0, 0);
//close thread handle
CloseHandle(hThread);
//free the now un-used memory from the remote process' heap
VirtualFreeEx(hProcess, pRemoteThread, sizeof(cavedata), MEM_RELEASE);
//close process' handle
CloseHandle(hProcess);
/*...*/
getchar();
return 0;
}
