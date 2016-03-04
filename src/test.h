#ifndef BENCHMARK_TEST_H
#define BENCHMARK_TEST_H

#include <vector>
#include "util/file.h"

#define DEFAULT_BLOCK_SIZE (32768)

class ITest
{
public:
	virtual ~ITest() {}
	virtual const char *GetName() = 0;
	virtual void SetFileList(std::vector<IFile *> & filelist) = 0;
	virtual void SetFileCount(int count) = 0;
	virtual void SetBlockSize(int blocksize) = 0;
	virtual void Run() = 0;
	virtual void Stop() = 0;
};

#endif
