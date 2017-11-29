#@ job_name = pc-ws1617-s04-mpi
#@ job_type = MPICH
#@ node = 3
#@ tasks_per_node = 28
#@ class = test
#@ initialdir = $(home)/s04-mpi
#@ output = $(home)/s04-mpi/results/result.out
#@ error = $(home)/s04-mpi/results/result.err
#@ wall_clock_limit = 00:10:00
#@ energy_policy_tag = NONE
#@ queue

. /etc/profile
. /etc/profile.d/modules.sh

# setup of environment

module unload mpi.ibm
module load mpi.intel

mpiexec -n 84 ./pi_parallel
