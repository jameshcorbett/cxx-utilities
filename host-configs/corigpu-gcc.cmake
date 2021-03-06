set(CONFIG_NAME "corigpu-gcc7" CACHE PATH "") 

# Set up the tpls
set(GEOSX_TPL_ROOT_DIR /global/cscratch1/sd/settgast/thirdPartyLibs/ CACHE PATH "")
set(GEOSX_TPL_DIR ${GEOSX_TPL_ROOT_DIR}/install-${CONFIG_NAME}-release CACHE PATH "")

# C options
set(CMAKE_C_COMPILER /opt/gcc/7.3.0/bin/gcc CACHE PATH "")
#set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -mcpu=powerpc64le -mtune=powerpc64le" CACHE STRING "")

# C++ options
set(CMAKE_CXX_COMPILER /opt/gcc/7.3.0/bin/g++ CACHE PATH "")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -mcpu=powerpc64le -mtune=powerpc64le" CACHE STRING "")
set(CMAKE_CXX_STANDARD 14 CACHE STRING "")

# Fortran options
set(CMAKE_Fortran_COMPILER /opt/gcc/7.3.0/bin/gfortran CACHE PATH "")
#set(CMAKE_Fortran_FLAGS_RELEASE "-O3 -DNDEBUG -qarch=pwr9 -qtune=pwr9" CACHE STRING "")
set(FORTRAN_MANGLE_NO_UNDERSCORE ON CACHE BOOL "")

# OpenMP options
set(ENABLE_OPENMP ON CACHE BOOL "" FORCE)
#set(OpenMP_Fortran_FLAGS "-qsmp=omp" CACHE STRING "")
set(OpenMP_Fortran_LIB_NAMES "" CACHE STRING "")

# MPI options
set(ENABLE_MPI ON CACHE BOOL "")
set(MPI_ROOT /usr/common/software/mvapich2/2.3/gnu/7.3.0 CACHE PATH "")
set(MPI_C_COMPILER         ${MPI_ROOT}/bin/mpicc   CACHE PATH "")
set(MPI_CXX_COMPILER       ${MPI_ROOT}/bin/mpicxx  CACHE PATH "")
set(MPI_Fortran_COMPILER   ${MPI_ROOT}/bin/mpif90  CACHE PATH "")
set(MPIEXEC                lrun CACHE STRING "")
set(MPIEXEC_NUMPROC_FLAG   -n CACHE STRING "")
set(ENABLE_WRAP_ALL_TESTS_WITH_MPIEXEC ON CACHE BOOL "")

# Cuda options
set(ENABLE_CUDA ON CACHE BOOL "")
set(CUDA_TOOLKIT_ROOT_DIR /global/common/cori/software/cuda/10.1 CACHE STRING "")
set(CMAKE_CUDA_HOST_COMPILER ${MPI_CXX_COMPILER} CACHE STRING "")
set(CMAKE_CUDA_COMPILER ${CUDA_TOOLKIT_ROOT_DIR}/bin/nvcc CACHE STRING "")
set(CUDA_ARCH sm_70 CACHE STRING "")
set(CMAKE_CUDA_STANDARD 14 CACHE STRING "")
set(CMAKE_CUDA_FLAGS "-restrict -arch ${CUDA_ARCH} --expt-extended-lambda --expt-relaxed-constexpr -Werror cross-execution-space-call,reorder,deprecated-declarations" CACHE STRING "")
set(BLT_CUDA_FLAGS "${BLT_CUDA_FLAGS} ${CUDA_NVCC_FLAGS}" CACHE STRING "")

# GTEST options
set(ENABLE_GTEST_DEATH_TESTS OFF CACHE BOOL "")
set(gtest_disable_pthreads ON CACHE BOOL "")
