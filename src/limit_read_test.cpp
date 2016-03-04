#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include "util/functional.h"
#include "limit_read_test.h"
#include "util/size_bench.h"
#include "util/limit_bench.h"
#include <stdlib.h>
#include <thread>

CLimitReadTest::CLimitReadTest()
	:m_filecount(1),
	 m_blocksize(DEFAULT_BLOCK_SIZE),
	 m_limitsize(2048*1024),
	 m_running(false)
{
	m_thread_count = 0;
}

CLimitReadTest::~CLimitReadTest()
{
}

void CLimitReadTest::SetBlockSize(int blocksize)
{
	m_blocksize = blocksize;
}

void CLimitReadTest::SetLimitSize(int limitsize)
{
	m_limitsize = limitsize;
}

void CLimitReadTest::SetFileCount(int count)
{
	m_filecount = count;
}

void CLimitReadTest::SetFileList(std::vector<IFile *> & filelist)
{
	m_filelist = filelist;
	m_threadlist.resize(filelist.size());
}

void CLimitReadTest::WaitStart()
{
	m_thread_count++;
	
	std::unique_lock<std::mutex> lock(m_mutex);
	m_start_cond.wait(lock);
}

void CLimitReadTest::NotifyStart()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_start_cond.notify_all();
}

void CLimitReadTest::WaitStop()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_stop_cond.wait(lock);
}

void CLimitReadTest::NotifyStop()
{
	m_running = false;
	
	std::unique_lock<std::mutex> lock(m_mutex);
	m_stop_cond.notify_all();
}

void CLimitReadTest::ThreadRun(IFile *file)
{
	/* init */
	void *block = malloc(m_blocksize);
	int64_t total_size = 0;
	CLimitBench bench;
	bench.SetLimit(m_limitsize);
	
	/* wait */
	WaitStart();
	bench.Start();
	
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
			bench.Increment(ret);
			total_size += ret;
		}
	}
	if (block != NULL)
	{
		free(block);
	}
	printf("[%u]%" PRId64 "\n", (unsigned)pthread_self(), total_size);
}

void CLimitReadTest::Run()
{
	m_running = true;
	if (m_filelist.size() > (unsigned)m_filecount)
	{
		m_filelist.resize((unsigned)m_filecount);
	}
	int count = (int)m_filelist.size();
	for (int i = 0; i < count; i++)
	{
		std::thread *t = new std::thread(std::bind(&CLimitReadTest::ThreadRun,
			this, m_filelist[i]));
		m_threadlist[i] = t;
	}
	
	while (m_thread_count < count)
	{
		sched_yield();
	}
	m_bench.Start();
	CLimitBench::Init();
	
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

void CLimitReadTest::Stop()
{
	m_running = false;
	m_stop_cond.notify_all();
}

