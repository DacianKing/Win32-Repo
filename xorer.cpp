#define WINVER 0x0500

#include <stdio.h>
#include <windows.h>

bool options = 0; //if 1 decrypt


int main(int argc, char *argv[]) {
if(argc < 3) {
	printf("Usage : %s <originalfile> <xorkey>",argv[0]);
	return 0;
}


	DWORD trashvar;
	DWORD written;

	char xorkey[strlen(argv[2])] = { 0 };
	lstrcat(xorkey,argv[2]);

	char *ext = strrchr(argv[1],'.');
	if(!lstrcmp(ext,".xor")) {
		++options;
	}

	HANDLE hFile = CreateFile(argv[1],GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if(GetLastError() != 0) {
		printf("CreateFile() to original failed with error code %d",GetLastError());
		CloseHandle(hFile);
		return 0;
}
	DWORD size = GetFileSize(hFile,&trashvar);

	char *buffer = (char*)malloc(size);

	if(buffer == 0) {
		printf("malloc() call failed\n");
		CloseHandle(hFile);
		return 0;
}


	BOOL success = ReadFile(hFile,buffer,size,&written,NULL);
	//printf("%s\n\n\n",buffer);

	if(!success) {
		printf("ReadFile() call failed with code %d\n",GetLastError());
		CloseHandle(hFile);
		return 0;
}

	if(written != size) {
		printf("File read is not the same as the size of the file\n");
		CloseHandle(hFile);
		return 0;
}

int xorindex = 0;
int xorstrlen = strlen(xorkey);

	for(unsigned int i =0; i < size; ++i) {
	buffer[i]^= xorkey[xorindex];
	if(xorindex == xorstrlen - 1) {
		xorindex = 0;
	}
	xorindex++;
}

//printf("%s",buffer);

	char filename[255] = { 0 };

	lstrcat(filename,argv[1]);
	if(!options) {
	lstrcat(filename,".xor");
}

if(options) {
	int len = strlen(filename);
	char extension[] = "txt";
	int extindex = 3;

	for(int i = len; i > (len - 4); i--) {
		filename[i] = extension[extindex];
		extindex--;
	}
}

CloseHandle(hFile);

	HANDLE newFile = CreateFile(filename,GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(GetLastError() != 0) {
		if(GetLastError() != 183) { //fix this shit
		printf("CreateFile() call 2 failed with %d",GetLastError());
		CloseHandle(newFile);
		return 0;
	}
	}

	BOOL successwrite = WriteFile(newFile,buffer,size,NULL,NULL);
	if(successwrite == 0) {
		printf("WriteFile() bool failed\n");
		CloseHandle(newFile);
		return 0;
	}

CloseHandle(newFile);
}
