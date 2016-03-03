#ifndef BENCHMARK_LIMIT_BENCH_H
#define BENCHMARK_LIMIT_BENCH_H

#include "stop_watch.h"
#include <mutex>
#include <atomic>

class CLimitBench
{
public:
	CLimitBench()
		:m_limit(2048*1024),
		 m_size(0)
	{
	}
	
	~CLimitBench()
	{
	}

	static void Init()
	{
		m_missing = 0;
	}

	void SetLimit(int limit)
	{
		m_limit = limit;
	}
	
	void Start()
	{
		m_size = 0;
		m_watch.Start();
	}

	void Increment(int size)
	{
		m_size += size;
		int diff = 1000000 - m_watch.GetElapsed();
		if (diff < 0)
		{
			if (m_limit > m_size)
			{
				m_missing++;
				int missing = m_missing;
				printf("missing %d, %d\n", missing, m_limit - m_size);
			}
			m_size = 0;
			m_watch.Restart();
		}
		else
		{
			if (m_size >= m_limit)
			{
				usleep(diff);
			}
		}
	}
private:
	CStopWatch m_watch;
	int m_limit;
	int m_size;
	std::mutex m_mutex;
	static std::atomic_int m_missing;
};

#endif //BENCHMARK_SIZE_BENCH_H
