#ifndef BENCHMARK_MULTI_THREAD_READ_TEST_H
#define BENCHMARK_MULTI_THREAD_READ_TEST_H

#include "test.h"
#include "util/size_bench.h"
#include <condition_variable>
#include <mutex>
#include <thread>

class CMultiThreadReadTest : public ITest
{
public:
	CMultiThreadReadTest();
	~CMultiThreadReadTest();
	const char *GetName()
	{
		return "multi";
	}
	void SetFileList(std::vector<IFile *> & filelist);
	void Run();

	void SetBlockSize(int blocksize);
	void SetCount(int count);
private:
	void ThreadRun(IFile *file);
	std::vector<IFile *> m_filelist;
	std::vector<std::thread *> m_threadlist;
	int m_blocksize;
	int m_count;
	bool m_bRunning;
	CSizeBench m_bench;
	std::mutex m_mutex;
	std::condition_variable m_start_cond;
	std::condition_variable m_stop_cond;
	std::atomic_int m_thread_count;
};

#endif // BENCHMARK_MULTI_THREAD_READ_TEST_H

