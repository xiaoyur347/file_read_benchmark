#include "single_thread_read_test.h"
#include "read_file.h"
#include "file.h"

void AddUnbufferFile(const std::string &strLine, void *arg)
{
	std::vector<IFile *> * pvec = (std::vector<IFile *> *)arg;
	IFile *file = new CUnbufferFile();
	file->Open(strLine.c_str());
	pvec->push_back(file);
}

static void InitFileList(std::vector<IFile *> &filelist, const char *resource)
{
	ReadFileFunc(resource,AddUnbufferFile,&filelist);
}

int main(int argc, char *argv[])
{
	CSingleThreadReadTest test;
	std::vector<IFile *> filelist;
	InitFileList(filelist, argv[1]);
	test.SetFileList(filelist);
	test.Run();
	return 0;
}

