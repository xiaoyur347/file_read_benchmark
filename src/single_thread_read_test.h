#ifndef BENCHMARK_SINGLE_THREAD_READ_TEST_H
#define BENCHMARK_SINGLE_THREAD_READ_TEST_H

#include "test.h"

class CSingleThreadReadTest : public ITest
{
public:
	CSingleThreadReadTest();
	~CSingleThreadReadTest();
	const char *GetName()
	{
		return "single";
	}
	void SetFileList(std::vector<IFile *> & filelist);
	void Run();

	void SetBlockSize(int blocksize);
private:
	std::vector<IFile *> m_filelist;
	int m_blocksize;
	void *m_block;
};

#endif // BENCHMARK_SINGLE_THREAD_READ_TEST_H

