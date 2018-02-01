#!/bin/sh

# !!! Only effective if this script is loaded via
# !!!   `source set-build-env.sh`

# ==========================================================================

# See: https://www.lrz.de/services/software/utilities/modules/

# ==========================================================================

# Enable modules
#
source /etc/profile.d/modules.sh

# Set locale
#
LANG=en_US.UTF-8
LC_ALL=en_US

# Load modules
# ==========================================================================

# GCC >= 4.6 must be available for C++11 - features of ICC
# See: https://software.intel.com/en-us/articles/c0x-features-supported-by-intel-c-compiler

# OPTION 2: Intel-MPI via Intel compiler suite
# --------------------------------------------------------------------------
module unload gcc
module unload intel
module unload cmake
module unload mpi.ibm
module unload mpi.intel
module unload mpi.ompi
module unload mkl
module unload hwloc
module unload scalapack
module unload plasma
module load gcc/4.9
module load intel/15.0
module load cmake/3.4
module load mpi.intel
module load mkl/11.2
module load hwloc
module load scalapack
# module load plasma

# module list should print:
# 1) admin/1.0         5) lrz/default       9) mpi.intel/5.0
# 2) tempdir/1.0       6) gcc/4.9(default) 10) mkl/11.2
# 3) lrztools/1.0      7) intel/15.0       11) scalapack/mkl
# 4) poe/1.4           8) cmake/3.4


# Load library/tool modules
# (after chosing compiler suite)
# --------------------------------------------------------------------------
# PAPI module is broken, use own compilation of PAPI:
module unload papi
export PAPI_HOME=~/opt/papi

# Set default compiler
# ==========================================================================
export CC=icc
export CXX=icc
alias cc="cc"
alias cxx="icc"

