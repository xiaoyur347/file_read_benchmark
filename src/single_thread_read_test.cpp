#include "single_thread_read_test.h"
#include <stdlib.h>

CSingleThreadReadTest::CSingleThreadReadTest()
	:m_blocksize(0),
	 m_count(-1),
	 m_bRunning(false)
{
	SetBlockSize(32768);
}

CSingleThreadReadTest::~CSingleThreadReadTest()
{
}

void CSingleThreadReadTest::SetBlockSize(int blocksize)
{
	if (m_blocksize != blocksize)
	{
		m_blocksize = blocksize;
	}
}

void CSingleThreadReadTest::SetCount(int count)
{
	m_count = count;
}

void CSingleThreadReadTest::SetFileList(std::vector<IFile *> & filelist)
{
	m_filelist = filelist;
}

void CSingleThreadReadTest::Run()
{
	void *block = malloc(m_blocksize);
	m_bRunning = true;
	m_bench.Start();
	while (m_bRunning)
	{
		int count = 0;
		for (std::vector<IFile *>::iterator it = m_filelist.begin();
			it != m_filelist.end();
			++it)
		{
			if (m_count > 0 && count >= m_count)
			{
				break;
			}
			count++;
			int ret = (*it)->Read(block, m_blocksize);
			if (ret <= 0)
			{
				m_bRunning = false;
				break;
			}
			else
			{
				m_bench.Increment(ret);
			}
		}
	}
	if (block != NULL)
	{
		free(block);
	}
}

