#include <iostream>
#include <cstdint>

#include <ctime>


static inline long long timestamp()
{
  struct timespec ts;
  long long timestamp;
  clock_gettime(CLOCK_REALTIME, &ts);
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
}


// Harmless and irrelevant, just to make the compiler
// see actual work in the loop:
//
static inline int64_t do_loop_work(int64_t i) {
	return i % 343;
}

// Assume parameter value from something like:
//   std::cin >> runtime_value;
//
int main() {
	long long ts_start, duration;
  long long accum = 0;
	
	// Only difference between loops is break condition
	// as comparison against const vs. non-const:
	const size_t NumIterations_const = 1024 * 1024 * 1024;
  size_t NumIterations_rtime       = 0;


  size_t runtime_val;
  std::cin >> runtime_val;
  NumIterations_rtime += runtime_val * runtime_val * runtime_val;
  

  printf(" == loop A: const break condition\n");
  accum = 0;
  ts_start = timestamp();
	for (size_t i = 0; i < NumIterations_const; i++) {
			accum += do_loop_work(i);
	}
  printf("    accum:  %llu\n",  accum);
  printf("    niter:  %d\n",    NumIterations_const);
  printf(" -> time:   %d ms\n", (timestamp() - ts_start) / 1000);


  printf(" == loop B: runtime-dependent break condition\n");
  accum = 0;
  ts_start = timestamp();
	for (size_t i = 0; i < NumIterations_rtime; i++) {
			accum += do_loop_work(i);
	}
  printf("    accum:  %llu\n",  accum);
  printf("    niter:  %d\n",    NumIterations_rtime);
  printf(" -> time:   %d ms\n", (timestamp() - ts_start) / 1000);

	return accum;
}

