#include "single_thread_read_test.h"
#include "multi_thread_read_test.h"
#include "limit_read_test.h"
#include "read_file.h"
#include "util/file.h"
#include <stdlib.h>
#include <signal.h>

void AddUnbufferFile(const std::string &strLine, void *arg)
{
	std::vector<IFile *> * pvec = (std::vector<IFile *> *)arg;
	IFile *file = new CUnbufferFile();
	file->Open(strLine.c_str());
	pvec->push_back(file);
}

void AddBufferFile(const std::string &strLine, void *arg)
{
	std::vector<IFile *> * pvec = (std::vector<IFile *> *)arg;
	IFile *file = new CBufferFile();
	file->Open(strLine.c_str());
	pvec->push_back(file);
}

static void InitFileList(std::vector<IFile *> &filelist, const char *resource)
{
	ReadFileFunc(resource,AddUnbufferFile,&filelist);
	//ReadFileFunc(resource,AddBufferFile,&filelist);
}

//CMultiThreadReadTest test;
CLimitReadTest test;

void exit_signal_handler(int signal_no)
{
	test.Stop();
}

int main(int argc, char *argv[])
{
	system("echo 3 > /proc/sys/vm/drop_caches");

	signal(SIGINT, exit_signal_handler);
	
	std::vector<IFile *> filelist;
	InitFileList(filelist, argv[1]);
	test.SetFileList(filelist);
	if (argc > 2)
	{
		test.SetFileCount(atoi(argv[2]));
	}
	test.Run();
	return 0;
}

