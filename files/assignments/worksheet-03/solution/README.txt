Normally, llrun is the most convenient way to submit an execution,
but there have been issues with it recently.
There are two alternatives:

Use llsubmit to enqueue a job using a command file:

   $ llsubmit jobs/job.serial.run

Alternatively, you can use the autonomous command to execute
a list of sequential jobs in parallel (ideal for running
a series of sequential benchmarks).
Example for running 4 sequential jobs in parallel:

   $ autonomous -A -C test -N 1 -n 4 -S jobs/autonoumous.cmdlst

