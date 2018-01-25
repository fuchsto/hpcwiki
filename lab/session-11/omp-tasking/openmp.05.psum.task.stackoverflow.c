
#include <ctime>
#include <vector>
#include <iostream>
#include <cstdio>

typedef int32_t input_t;
typedef int64_t value_t;
typedef int64_t index_t;

static inline long long timestamp()
{
  struct timespec ts;
  long long timestamp;
  clock_gettime(CLOCK_REALTIME, &ts);
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
}



using namespace std;

const int n          = 100000000;
const int baseLength = 100;

int f(int ii) {
  return ii;
}

int recursiveSumBody(int * begin, int * end){

  size_t length  = end - begin;
  size_t mid     = length/2;
  int    sum     = 0;

  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // !!                                                             !!
  // !!          THIS CODE ILLUSTRATES ONE OF MANY                  !!
  // !!          HORRIBLE IDEAS I FOUND IN THE WEB:                 !!
  // !!      https://stackoverflow.com/questions/18719257           !!
  // !!                                                             !!
  // !!      DO NOT USE THIS FOR REFERENCE IN YOUR WORK!            !!
  // !!      MAKE SURE TO UNDERSTAND WHY AND IN HOW MANY            !!
  // !!      WAYS THIS IMPLEMENTATION IS MISGUIDED!                 !!
  // !!                                                             !!
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  if ( length < baseLength ) {
    for(size_t ii = 1; ii < length; ii++ ){
        begin[ii] += begin[ii-1];
    }
  } else {
#pragma omp task shared(sum)
    {
      sum = recursiveSumBody(begin    ,begin+mid);
    }
#pragma omp task
    {
      recursiveSumBody(begin+mid,end      );
    }
#pragma omp taskwait

#pragma omp parallel for
    for(size_t ii = mid; ii < length; ii++) {
      begin[ii] += sum;
    }

  }
  return begin[length-1];
}

void recursiveSum(int * begin, int * end){

# pragma omp parallel
  {
#   pragma omp single
    {
      recursiveSumBody(begin,end);
    }    
  }
}


int main()
{
  long long ts_start, duration_p, duration_s;

  vector<int> a_par(n,0);
  vector<int> a_seq(n,0);

# pragma omp parallel for schedule(static)
  for(int ii=0; ii < n; ii++) {          
    a_par[ii] = f(ii);
    a_seq[ii] = f(ii);
  }  

  printf(" == sequential run ...\n");
  ts_start = timestamp();
  for (int64_t i = 1; i < n; i++) {
    a_seq[i] = a_seq[i-1] + a_seq[i];
  }
  duration_s = timestamp() - ts_start;

  printf(" == parallel run ...\n");
  ts_start = timestamp();
  recursiveSum(a_par.data(), a_par.data() + n);
  duration_p = timestamp() - ts_start;

  printf("------------------------------\n");
  printf("sequential: %.4f ms\n", 1.0e-6 * duration_s);
  printf("parallel:   %.4f ms\n", 1.0e-6 * duration_p);

  return 0;
}
