#include <stdio.h>
#include <omp.h>


int main()
{
  int i, t_id;

  printf("-- Unordered:\n");
                       private(t_id)
  {
    t_id = omp_get_thread_num();
    printf("    thread %d of %d\n", t_id, omp_get_num_threads());
  }

  printf("-- Ordered:\n");
  /* Note:
   * Loop counters are private by default (every iteration
   * has a private copy of i set to the current loop index),
   * shared ownership would provoke race.
   */
  #pragma omp parallel for ordered \
                           private(t_id)
  for (i = 0; i < omp_get_num_threads(); ++i) {
    /* Every thread loads its id in arbitrary order
     * in parallel:
     */
    t_id = omp_get_thread_num();
    /* But output of the messages is ordered:
     */
    #pragma omp ordered
    printf("    thread %d of %d\n", t_id, omp_get_num_threads());

    /* Note:
     * Ordered statements need to be nested in an ordered
     * parallel region (here: parallel for >ordered<).
     */
  }

  return 0;
}
