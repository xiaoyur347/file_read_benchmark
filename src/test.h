#ifndef BENCHMARK_TEST_H
#define BENCHMARK_TEST_H

#include <vector>
#include "util/file.h"

class ITest
{
public:
	virtual ~ITest() {}
	virtual const char *GetName() = 0;
	virtual void SetFileList(std::vector<IFile *> & filelist) = 0;
	virtual void Run() = 0;
};

#endif
