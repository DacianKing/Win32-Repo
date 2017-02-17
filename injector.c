#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>

#define msgboxaddr 0x7511314e
#define getprocaddress 0x774bab25
#define loadlibraryAs 0x7529b9e8
#define createprocessa 0x752c3dab


DWORD GetProcId(char* procname);

DWORD __stdcall RemoteThread(HMODULE hlib, LPCSTR procname) //thread that will be spawned in our target process
{
  char msg[] = "hooked";
  DWORD slepvar = 5000;

  int (*msgbox)() = (void*)msgboxaddr;
  void (*slep)(DWORD) = (void*)0x774bd03e;
  msgbox(0,procname,msg,0);
  slep(slepvar);

}

void main() {

  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetProcId("kek.exe")); //get handle to process

  LPVOID pRemoteThread = VirtualAllocEx(hProcess, NULL, 500, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE); //allocate memory for remote thread


  WriteProcessMemory(hProcess, pRemoteThread, (LPVOID)RemoteThread,500, 0); //write remote thread to memory

  DWORD oldprot;
  int test = VirtualProtectEx(hProcess,(void*)0x774bab25,5,0x20,&oldprot); //remove protection to address

 BYTE jmp = 0xE9; //relative jmp
 DWORD jmpaddr = (DWORD)getprocaddress - ((DWORD)pRemoteThread - 5); //address of jump


 WriteProcessMemory(hProcess,(void*)0x774bab25,&jmp,1,0); //write byte to jmp

 printf("code cave address at 0x%x\n",pRemoteThread);
 printf("%d\n",((DWORD)pRemoteThread + (DWORD)getprocaddress + 5));
 DWORD written;

  //int newtest = WriteProcessMemory(hProcess,(void*)(getprocaddress),(LPVOID)&nophook,5,&written);

  int newtest = WriteProcessMemory(hProcess,(void*)(getprocaddress + 1),(LPVOID)&jmpaddr,4,&written); //write address
  printf("%d",written);



  CloseHandle(hProcess);

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
