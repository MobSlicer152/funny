// build system in C, because fuck build systems and fuck batch files

#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#define EXE_EXT ".exe"
#else
#include <unistd.h>
#define EXE_EXT ""
#endif

#ifdef _MSC_VER
#define NORETURN __declspec(noreturn)
#else
#define NORETURN _Noreturn
#endif

#define MIN(a, b)     ((a) < (b) ? (a) : (b))
#define MAX(a, b)     ((a) > (b) ? (a) : (b))
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

NORETURN void Fatal(const char* msg, ...);
int Execute(int argc, const char** argv, const char* workingDir, bool inheritEnvironment, const char* environment);
void Assemble(const char* src, const char* dest, const char** flags, size_t flagCount)
{
	const char* assembler = "nasm" EXE_EXT;
}
void CompileKernelC(const char* src, const char* dest, const char** flags, size_t flagCount)
{
	const char* compiler = "clang" EXE_EXT;
}
void CompileToolC(const char* src, const char* dest, const char** flags, size_t flagCount)
{
	const char* compiler = "clang" EXE_EXT;
}

int main(int argc, char* argv[])
{
}

NORETURN void Fatal(const char* msg, ...)
{
	char* buffer;
	int size;
	va_list args;

	va_start(args, msg);
	size = vsnprintf(NULL, 0, msg, args) + 1;

	buffer = calloc(size, sizeof(char));
	if (!buffer)
	{
		fprintf(stderr, "[-] failed to allocate error message buffer\n");
		abort();
	}

	vsnprintf(buffer, size, msg, args);
	va_end(args);

	fprintf(stderr, "[-] fatal error: %s\n", buffer);
	free(buffer);
	abort();
}

int Execute(int argc, const char** argv, const char* workingDir, bool inheritEnvironment, const char* environment)
{
#ifdef _WIN32
	char* commandLine;
	size_t commandLineSize = 0;

	for (int i = 0; i < argc; i++)
	{
		commandLineSize += strlen(argv[i]) + 3;
	}

	commandLine = calloc(commandLineSize, sizeof(char));
	if (!commandLine)
	{
		Fatal("failed to allocate command line for child process");
	}

	size_t offset = 0;
	for (int i = 0; i < argc; i++)
	{
		size_t length = strlen(argv[i]);
		snprintf(commandLine + offset, commandLineSize - offset, "\"%s\" ", argv[i]);
		offset += length + 2;
		commandLine[offset] = ' ';
	}

	commandLine[offset] = 0;

	STARTUPINFOA startupInfo = {0};
	startupInfo.cb = sizeof(STARTUPINFOA);

	PROCESS_INFORMATION processInfo = {0};

	char* realEnvironment;
	if (inheritEnvironment)
	{
		realEnvironment = GetEnvironmentStrings();
	}
	else
	{
		size_t environmentSize;
		for (environmentSize = 0; environmentSize < INT16_MAX; environmentSize++)
		{
			if (environment[environmentSize] == 0 && environment[environmentSize + 1] == 0)
			{
				environmentSize += 2;
				break;
			}
		}

		realEnvironment = calloc(environmentSize, sizeof(char));
		if (!realEnvironment)
		{
			Fatal("failed to allocate %zu byte environment block", environmentSize);
		}

		memcpy(realEnvironment, environment, environmentSize);
	}

	if (!CreateProcessA(
			NULL, commandLine, NULL, NULL, true, NORMAL_PRIORITY_CLASS, realEnvironment, workingDir, &startupInfo, &processInfo))
	{
		Fatal("failed to execute %s: error %d", commandLine, GetLastError());
	}

	WaitForSingleObject(processInfo.hProcess, INFINITE);

	DWORD exitCode = 0;
	GetExitCodeProcess(processInfo.hProcess, &exitCode);

	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);

	if (!inheritEnvironment)
	{
		free(realEnvironment);
	}

	return exitCode;
#else
#endif
}
