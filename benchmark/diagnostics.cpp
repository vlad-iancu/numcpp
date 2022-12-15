#include <diagnostics.hpp>
#include <chrono>
#include <iostream>
#include <vector>

namespace npp
{
	double mean(std::vector<u64> ary) {
	  double avg = 0;
	  int t = 1;
	  for (double x : ary) 
	  {
		avg += (x - avg) / t;
		++t;
	  }
	  return avg;
	}
	double run(const std::string &name, u64 iterations, std::function<void(u64)> exec)
	{
		double avg = 0;
		u64 t = 1;
		std::chrono::high_resolution_clock clock;
		for(u64 iter = 1; iter <= iterations; iter++)
		{
			std::chrono::high_resolution_clock::time_point start = clock.now();
			exec(iter);
			std::chrono::high_resolution_clock::time_point end = clock.now();
			u64 time_nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			std::cout << name << " - Iteration " << iter << ": " << time_nanos << " nanoseconds" << std::endl;
			avg += (time_nanos - avg) / t;
			t++;
		}
		return avg;
	}
}
