#include "single_thread_read_test.h"
#include "read_file.h"
#include "util/file.h"
#include <stdlib.h>

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
	//ReadFileFunc(resource,AddUnbufferFile,&filelist);
	ReadFileFunc(resource,AddBufferFile,&filelist);
}

int main(int argc, char *argv[])
{
	CSingleThreadReadTest test;
	std::vector<IFile *> filelist;
	InitFileList(filelist, argv[1]);
	test.SetFileList(filelist);
	if (argc > 2)
	{
		test.SetCount(atoi(argv[2]));
	}
	test.Run();
	return 0;
}

