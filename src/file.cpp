#include "file.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

CUnbufferFile::CUnbufferFile()
	:m_fd(-1)
{
}

CUnbufferFile::~CUnbufferFile()
{
	CUnbufferFile::Close();
}

bool CUnbufferFile::Open(const char *path)
{
	if (m_fd != -1)
	{
		Close();
	}
	int ret = open(path, O_RDONLY, S_IRUSR);
	if (ret < 0)
	{
		return false;
	}
	m_fd = ret;
	return true;
}

bool CUnbufferFile::Close()
{
	if (m_fd != -1)
	{
		close(m_fd);
		m_fd = -1;
	}
	return true;
}

int CUnbufferFile::Read(void *ptr, int size)
{
	if (m_fd != -1)
	{
		return read(m_fd, ptr, size);
	}

	return -1;
}

CBufferFile::CBufferFile()
	:m_fd(NULL)
{
}

CBufferFile::~CBufferFile()
{
	CBufferFile::Close();
}

bool CBufferFile::Open(const char *path)
{
	if (m_fd != NULL)
	{
		Close();
	}
	FILE *p = fopen(path, "r");
	if (p == NULL)
	{
		return false;
	}
	m_fd = p;
	return true;
}

bool CBufferFile::Close()
{
	if (m_fd != NULL)
	{
		fclose(m_fd);
		m_fd = NULL;
	}
	return true;
}

int CBufferFile::Read(void *ptr, int size)
{
	if (m_fd != NULL)
	{
		return fread(ptr, 1, size, m_fd);
	}

	return -1;
}

