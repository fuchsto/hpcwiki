#define __GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* It might be convenient to use a define to set
 * the data type used in the benchmark:
 */
#define DTYPE int /* or double */

/* Helper function to obtain a current time stamp
 */
long long timestamp();


int main(int argc, char * argv[])
{
  /*********************************************/
  /* Parse command line arguments              */

  int scenario = 1;
  if (argc > 1) {
    /* First argument is executable name, second
     * argument is scenario. Use atoi to convert
     * string '1' or '2' to integer:
     */
    scenario = atoi(argv[1]);
    if (scenario != 1 && scenario != 2) {
      printf("Invalid scenario, use 1 or 2.\n");
      return EXIT_FAILURE;
    }
  }
  printf("Running scenario %d ...\n", scenario);

  /*********************************************/
  /* Run the benchmark                         */

  if (scenario == 1) {
    /* your implementation of scenario 1 here */
  }
  else if (scenario == 2) {
    /* your implementation of scenario 2 here */
  }

  /*********************************************/
  /* Example of how to measure time intervals: */

  /* Get current time before measured procedure
   * (start timestamp):
   */
  long long ts_start = timestamp();

  /* Run the procedure to measure:
   */
  for (int i = 0; i < 10000000; i++);
  for (int i = 0; i < 10000000; i++);
  for (int i = 0; i < 10000000; i++);
  for (int i = 0; i < 10000000; i++);

  /* Get current time after measured procedure
   * (end timestamp):
   */
  long long ts_end   = timestamp();

  /* Duration is difference of start- and end
   * timestamps:
   */
  long long duration = ts_end - ts_start;

  printf("Time interval test: %lld nanoseconds\n",
         duration);

  /* End of time interval measurement example  */
  /*********************************************/

  return EXIT_SUCCESS;
}

long long timestamp()
{
  /* Note:
   * Don't use gettimeofday() for measuring time
   * in benchmarks.
   */
  struct timespec ts;
  long long timestamp;
  clock_gettime(CLOCK_REALTIME, &ts);
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
}

