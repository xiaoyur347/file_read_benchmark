#include "util/functional.h"
#include "multi_thread_read_test.h"
#include "util/size_bench.h"
#include <stdlib.h>
#include <thread>

CMultiThreadReadTest::CMultiThreadReadTest()
	:m_blocksize(0),
	 m_count(1)
{
	SetBlockSize(32768);
	m_thread_count = 0;
}

CMultiThreadReadTest::~CMultiThreadReadTest()
{
}

void CMultiThreadReadTest::SetBlockSize(int blocksize)
{
	if (m_blocksize != blocksize)
	{
		m_blocksize = blocksize;
	}
}

void CMultiThreadReadTest::SetCount(int count)
{
	m_count = count;
}

void CMultiThreadReadTest::SetFileList(std::vector<IFile *> & filelist)
{
	m_filelist = filelist;
	m_threadlist.resize(filelist.size());
}

void CMultiThreadReadTest::ThreadRun(IFile *file)
{
	void *block = malloc(m_blocksize);
	m_thread_count++;
	int count = m_thread_count;
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_start_cond.wait(lock);
	}
	while (m_bRunning)
	{
		int ret = file->Read(block, m_blocksize);
		if (ret <= 0)
		{
			m_bRunning = false;
			
			{
				//notify
				std::unique_lock<std::mutex> lock(m_mutex);
				m_stop_cond.notify_all();
			}
			break;
		}
		else
		{
			m_bench.Increment(ret);
		}
	}
	if (block != NULL)
	{
		free(block);
	}
}

void CMultiThreadReadTest::Run()
{
	m_bRunning = true;
	unsigned less = (unsigned)m_count < m_filelist.size() ? (unsigned)m_count : m_filelist.size();
	for (unsigned i = 0; i < less; i++)
	{
		IFile *file = m_filelist[i];
		std::thread *t = new std::thread(std::bind(&CMultiThreadReadTest::ThreadRun,
			this, file));
		m_threadlist[i] = t;
	}
	
	while (m_thread_count < less)
	{
		sched_yield();
	}
	m_bench.Start();

	//notify
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_start_cond.notify_all();
	}

	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_stop_cond.wait(lock);
	}
	for (unsigned i = 0; i < less; i++)
	{
		m_threadlist[i]->join();
	}
}

