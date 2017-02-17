#define WINVER 0x0500

#include <windows.h>
#include <Shobjidl.h>
#include <stdio.h>
#include <conio.h>

int main() {
char str[] = "String to be overwritten";

  HMODULE hlib = LoadLibrary("kernel32.dll");


while(1) {
  printf("here\n");
char *p = (void*)0x774bab25;
char val1 = p[0];
char val2 = p[1];
char val3 = p[2];
char val4 = p[3];
char val5 = p[4];


printf("%.1x %.1x %.1x %.1x %.1x\n",val1,val2,val3,val4,val5);
DWORD addr = (DWORD)GetProcAddress(hlib,"GetProcAddress");
  Sleep(5000);

}
}
