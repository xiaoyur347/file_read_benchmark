#include "single_thread_read_test.h"
#include <stdlib.h>

CSingleThreadReadTest::CSingleThreadReadTest()
	:m_filecount(-1),
	 m_blocksize(DEFAULT_BLOCK_SIZE),
	 m_running(false)
{
}

CSingleThreadReadTest::~CSingleThreadReadTest()
{
}

void CSingleThreadReadTest::SetBlockSize(int blocksize)
{
	m_blocksize = blocksize;
}

void CSingleThreadReadTest::SetFileCount(int count)
{
	m_filecount = count;
}

void CSingleThreadReadTest::SetFileList(std::vector<IFile *> & filelist)
{
	m_filelist = filelist;
}

void CSingleThreadReadTest::Run()
{
	void *block = malloc(m_blocksize);
	m_running = true;
	m_bench.Start();
	while (m_running)
	{
		int count = 0;
		for (std::vector<IFile *>::iterator it = m_filelist.begin();
			it != m_filelist.end();
			++it)
		{
			if (m_filecount > 0 && count >= m_filecount)
			{
				break;
			}
			count++;
			int ret = (*it)->Read(block, m_blocksize);
			if (ret <= 0)
			{
				m_running = false;
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

void CSingleThreadReadTest::Stop()
{
	m_running = false;
}

