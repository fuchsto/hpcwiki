#!/bin/bash

# Usage: 
#
#   $ source ./setenv.sh
#

# Currently Loaded Modulefiles:
#  1) admin/1.0     4) intel/15.0    7) mpi.ibm/1.4  10) hwloc/1.10
#  2) tempdir/1.0   5) mkl/11.2      8) lrz/default
#  3) lrztools/1.0  6) poe/1.4       9) papi/5.3

module unload papi
module unload mkl
module unload intel
module unload hwloc

module load papi/5.3
module load mkl/11.2
module load intel/15.0
module load hwloc/1.10

export MKL_SERIAL=yes

