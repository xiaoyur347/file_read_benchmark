#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include "util/functional.h"
#include "multi_thread_read_test.h"
#include "util/size_bench.h"
#include <stdlib.h>
#include <thread>

CMultiThreadReadTest::CMultiThreadReadTest()
	:m_filecount(1),
	 m_blocksize(DEFAULT_BLOCK_SIZE),
	 m_running(false)
{
	m_thread_count = 0;
}

CMultiThreadReadTest::~CMultiThreadReadTest()
{
}

void CMultiThreadReadTest::SetBlockSize(int blocksize)
{
	m_blocksize = blocksize;
}

void CMultiThreadReadTest::SetFileCount(int count)
{
	m_filecount = count;
}

void CMultiThreadReadTest::SetFileList(std::vector<IFile *> & filelist)
{
	m_filelist = filelist;
	m_threadlist.resize(filelist.size());
}

void CMultiThreadReadTest::WaitStart()
{
	m_thread_count++;
	
	std::unique_lock<std::mutex> lock(m_mutex);
	m_start_cond.wait(lock);
}

void CMultiThreadReadTest::NotifyStart()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_start_cond.notify_all();
}

void CMultiThreadReadTest::WaitStop()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_stop_cond.wait(lock);
}

void CMultiThreadReadTest::NotifyStop()
{
	m_running = false;
	
	std::unique_lock<std::mutex> lock(m_mutex);
	m_stop_cond.notify_all();
}

void CMultiThreadReadTest::ThreadRun(IFile *file)
{
	void *block = malloc(m_blocksize);
	int64_t total_size = 0;
	WaitStart();
	while (m_running)
	{
		int ret = file->Read(block, m_blocksize);
		if (ret <= 0)
		{
			NotifyStop();
			break;
		}
		else
		{
			m_bench.Increment(ret);
			total_size += ret;
			sched_yield();
		}
	}
	if (block != NULL)
	{
		free(block);
	}
	printf("[%u]%" PRId64 "\n", (unsigned)pthread_self(), total_size);
}

void CMultiThreadReadTest::Run()
{
	m_running = true;
	if (m_filelist.size() > (unsigned)m_filecount)
	{
		m_filelist.resize((unsigned)m_filecount);
	}
	int count = (int)m_filelist.size();
	for (int i = 0; i < count; i++)
	{
		std::thread *t = new std::thread(std::bind(&CMultiThreadReadTest::ThreadRun,
			this, m_filelist[i]));
		m_threadlist[i] = t;
	}
	
	while (m_thread_count < count)
	{
		sched_yield();
	}
	m_bench.Start();

	NotifyStart();
	WaitStop();

	/* cleanup */
	for (unsigned i = 0; i < count; i++)
	{
		m_threadlist[i]->join();
		delete m_threadlist[i];
	}
	m_threadlist.clear();
}

void CMultiThreadReadTest::Stop()
{
	m_running = false;
	m_stop_cond.notify_all();
}

