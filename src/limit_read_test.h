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
	void SetFileCount(int count);
	void SetBlockSize(int blocksize);
	void SetLimitSize(int limitsize);
	void Run();
	void Stop();
private:
	void ThreadRun(IFile *file);
	void WaitStart();
	void NotifyStart();
	void WaitStop();
	void NotifyStop();
	
	std::vector<IFile *> m_filelist;
	int m_filecount;
	int m_blocksize;
	
	std::vector<std::thread *> m_threadlist;
	int m_limitsize;
	bool m_running;
	CSizeBench m_bench;
	std::mutex m_mutex;
	std::condition_variable m_start_cond;
	std::condition_variable m_stop_cond;
	std::atomic_int m_thread_count;
};

#endif // BENCHMARK_LIMIT_READ_TEST_H

