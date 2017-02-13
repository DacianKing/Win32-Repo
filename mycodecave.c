#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>

#define msgboxaddr 0x7511314e
#define getprocaddress 0x7529ab25
#define loadlibraryAs 0x7529b9e8

DWORD __stdcall RemoteThread() //thread that will be spawned in our target process
{
  HMODULE (*loadlib)() = (void*)loadlibraryAs;
  FARPROC(*getaddr)() = (void*)getprocaddress;
  //HMODULE hlib = loadlib(dlltoload);

 char dllname[] = "MessageBoxA";
 char libname[] = "user32.dll";

 HMODULE library = loadlib(libname);
 int(*msgbox)() = (DWORD)getaddr(library,dllname);

 msgbox(0,dllname,libname,MB_OK);

}

DWORD GetProcId(char* procname)
{
PROCESSENTRY32 pe;
HANDLE hSnap;

pe.dwSize = sizeof(PROCESSENTRY32);
hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
if (Process32First(hSnap, &pe)) {
do {
if (strcmp(pe.szExeFile, procname) == 0)
break;
} while (Process32Next(hSnap, &pe));
}
return pe.th32ProcessID;
}

int main() {

  HMODULE hlib = LoadLibrary("user32.dll");

    printf("%d\n",(DWORD)GetProcAddress(hlib,"MessageBoxA") % 10);


  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetProcId("firefox.exe"));
  //allocate memory for our procedure in the remote process' address space
  LPVOID pRemoteThread = VirtualAllocEx(hProcess, NULL, 500, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
  //write our thread to the target process
  WriteProcessMemory(hProcess, pRemoteThread, (LPVOID)RemoteThread,500, 0);
  //allocate memory for our cavedata object in the remote process
  //struct cavedata *pData = (struct cavedata*)VirtualAllocEx(hProcess, NULL, sizeof(struct cavedata), MEM_COMMIT, PAGE_READWRITE);
  //Write it to the target process
  //WriteProcessMemory(hProcess, pData, &CaveData, sizeof(struct cavedata), NULL);
  //spawn/create our procedure/thread in the remote process
  HANDLE hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)pRemoteThread, NULL, 0, 0);
  WaitForSingleObject(hThread,0);
//  VirtualFreeEx(hProcess,pRemoteThread,500,MEM_RELEASE);
  CloseHandle(hProcess);

}
