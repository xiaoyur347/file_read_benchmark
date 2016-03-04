#ifndef BENCHMARK_SINGLE_THREAD_READ_TEST_H
#define BENCHMARK_SINGLE_THREAD_READ_TEST_H

#include "test.h"
#include "util/size_bench.h"

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
	void SetFileCount(int count);
	void SetBlockSize(int blocksize);
	void Run();
	void Stop();
private:
	std::vector<IFile *> m_filelist;
	int m_filecount;
	int m_blocksize;
	
	bool m_running;
	CSizeBench m_bench;
};

#endif // BENCHMARK_SINGLE_THREAD_READ_TEST_H

