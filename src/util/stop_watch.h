#ifndef BENCHMARK_STOP_WATCH_H
#define BENCHMARK_STOP_WATCH_H

#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>

class CStopWatch
{
public:
	typedef uint64_t Timestamp;
	typedef int64_t TimeDiff;
	
	CStopWatch()
		:m_start(0),
		 m_elapsed(0),
		 m_running(false)
	{
	}
	~CStopWatch()
	{
	}
	void Start()
	{
		if (!m_running)
		{
			m_start = Now();
			m_running = true;
		}
	}
	void Stop()
	{
		if (m_running)
		{
			Timestamp current = Now();
			m_elapsed += current - m_start;
			m_running = false;
		}
	}
	void Reset()
	{
		m_elapsed = 0;
		m_running = false;
	}
	void Restart()
	{
		m_elapsed = 0;
		m_start = Now();
		m_running = true;
	}
	TimeDiff GetElapsed()
	{
		if (m_running)
		{
			Timestamp current = Now();
			return m_elapsed + (current - m_start);
		}
		else
		{
			return m_elapsed;
		}
	}
	int GetElapsedSeconds()
	{
		return int(GetElapsed()/1000000);
	}
private:
	Timestamp TimeVal2Timestamp(struct timeval &tv)
	{
		return tv.tv_sec * 1000000 + tv.tv_usec;
	}
	Timestamp Now()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * 1000000 + tv.tv_usec;
	}
	Timestamp m_start;
	TimeDiff m_elapsed;
	bool m_running;
};

#endif //BENCHMARK_STOP_WATCH_H
