#ifndef BENCHMARK_LIMIT_READ_TEST_H
#define BENCHMARK_LIMIT_READ_TEST_H

#include "test.h"
#include "util/size_bench.h"
#include <condition_variable>
#include <mutex>
#include <thread>

class CLimitReadTest : public ITest
{
public:
	CLimitReadTest();
	~CLimitReadTest();
	const char *GetName()
	{
		return "limit";
	}
	void SetFileList(std::vector<IFile *> & filelist);
	void Run();

	void SetBlockSize(int blocksize);
	void SetLimitSize(int limitsize);
	void SetCount(int count);
private:
	void ThreadRun(IFile *file);
	std::vector<IFile *> m_filelist;
	std::vector<std::thread *> m_threadlist;
	int m_blocksize;
	int m_limitsize;
	int m_count;
	bool m_bRunning;
	CSizeBench m_bench;
	std::mutex m_mutex;
	std::condition_variable m_start_cond;
	std::condition_variable m_stop_cond;
	std::atomic_int m_thread_count;
};

#endif // BENCHMARK_LIMIT_READ_TEST_H

