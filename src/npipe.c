#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strsafe.h>
#include <time.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

void simple_rand_str(char *s, size_t sz) {
	static const char cs[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	for (size_t i = 0; i < sz - 1; i++) {
		s[i] = cs[rand() % (sizeof(cs) - 1)];
	}
	s[sz] = '\0';
}

int err() {
	fprintf(stderr, "Error: %d\n", GetLastError());
	return -1;
}

// let's, ah...*not* put this on the stack :)
char g_readBuf[2097152];

int main(int argc, char **argv) {
	srand((unsigned) time(NULL));
	setmode(fileno(stdout), O_BINARY);
	
	char pipeName[256] = {0};
	StringCchCatA(pipeName, sizeof(pipeName), "\\\\.\\pipe\\");
	int status = 0;
	
	// process arguments - see usage details
	if (argc >= 2) {
		if (strcmp(argv[1], "/?") == 0) {
			fputs("Usage:", stderr);
			fputs("npipe [pipeName]", stderr);
			return status;
		}
		StringCchCatA(pipeName, sizeof(pipeName), argv[1]);
	} else {
		simple_rand_str(pipeName + strlen(pipeName), 8);
	}
	
	fprintf(stderr, "Pipe: %s\n", pipeName);
	
	// create the pipe
	HANDLE pipe = CreateNamedPipeA(
		pipeName, // lpName
		PIPE_ACCESS_INBOUND | FILE_FLAG_FIRST_PIPE_INSTANCE, // dwOpenMode
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS, // dwPipeMode
		1, // nMaxInstances
		1024, // nOutBufferSize
		sizeof(g_readBuf), // nInBufferSize
		0, // nDefaultTimeout
		NULL // lpSecurityAttributes
	);
	if (pipe == INVALID_HANDLE_VALUE) {
		status = err();
		goto end;
	}
	
	// wait for connection
	if (!ConnectNamedPipe(pipe, NULL)) {
		status = err();
		goto end;
	}
	
	// copy data to standard out
	while (1) {
		int readSz;
		if (!ReadFile(pipe, g_readBuf, sizeof(g_readBuf), &readSz, NULL)) {
			int code = GetLastError();
			if (code != ERROR_BROKEN_PIPE) status = err();
			break;
		}
		
		fwrite(g_readBuf, 1, readSz, stdout);
	}
	
	// clean up
end:
	DisconnectNamedPipe(pipe);
	CloseHandle(pipe);
	
	return status;
}