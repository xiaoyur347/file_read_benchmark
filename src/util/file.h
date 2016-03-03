#ifndef BENCHMARK_FILE_H
#define BENCHMARK_FILE_H

#include <unistd.h>
#include <stdio.h>

class IFile
{
public:
	virtual ~IFile() {}
	virtual bool Open(const char *path) = 0;
	virtual bool Close() = 0;
	virtual int Read(void *ptr, int size) = 0;
};

class CUnbufferFile : public IFile
{
public:
	CUnbufferFile();
	~CUnbufferFile();
	bool Open(const char *path);
	bool Close();
	int Read(void *ptr, int size);

private:
	int m_fd;
};

class CBufferFile : public IFile
{
public:
	CBufferFile();
	~CBufferFile();
	bool Open(const char *path);
	bool Close();
	int Read(void *ptr, int size);

private:
	FILE *m_fd;
};

#endif
