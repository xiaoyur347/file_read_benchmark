#ifndef BENCHMARK_SIZE_BENCH_H
#define BENCHMARK_SIZE_BENCH_H

#include "stop_watch.h"
#include <mutex>
#include <atomic>

class CSizeBench
{
public:
	CSizeBench()
	{
		m_total_size = 0;
		m_second_size = 0;
		m_total_watch.Start();
		m_second_watch.Start();
	}
	~CSizeBench()
	{
	}

	void Increment(int size)
	{
		m_total_size += size;
		m_second_size += size;

		if (m_second_watch.GetElapsed() > 1000000)
		{
			m_mutex.lock();
			if (m_second_watch.GetElapsed() > 1000000)
			{
				printf("%.3f MB, avg %.3f MB/s\n", 
					double(m_second_size) / (1024 * 1024),
					double(m_total_size) / (1024 * 1024) / m_total_watch.GetElapsed() * 1000000);
				m_second_size = 0;
				m_second_watch.Restart();
			}
			m_mutex.unlock();
		}
	}
private:
	CStopWatch m_total_watch;
	CStopWatch m_second_watch;
	std::atomic_llong m_total_size;
	std::atomic_llong m_second_size;
	std::mutex m_mutex;
};

#endif //BENCHMARK_SIZE_BENCH_H
