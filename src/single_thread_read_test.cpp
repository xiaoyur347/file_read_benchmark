#include "single_thread_read_test.h"
#include "util/size_bench.h"
#include <stdlib.h>

CSingleThreadReadTest::CSingleThreadReadTest()
	:m_blocksize(0),
	 m_block(NULL),
	 m_count(-1)
{
	SetBlockSize(32768);
}

CSingleThreadReadTest::~CSingleThreadReadTest()
{
	if (m_block != NULL)
	{
		free(m_block);
		m_block = NULL;
	}
}

void CSingleThreadReadTest::SetBlockSize(int blocksize)
{
	if (m_blocksize != blocksize)
	{
		m_blocksize = blocksize;
		if (m_block != NULL)
		{
			free(m_block);
		}
		m_block = malloc(m_blocksize);
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
	bool success = true;
	CSizeBench bench;
	while (success)
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
			int ret = (*it)->Read(m_block, m_blocksize);
			if (ret <= 0)
			{
				success = false;
				break;
			}
			else
			{
				bench.Increment(ret);
			}
		}
	}
}

