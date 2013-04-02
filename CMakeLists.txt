cmake_minimum_required(VERSION 2.8)

project(tensorheaven)

find_path(EIGEN_INCLUDE_DIR "eigen" PATHS $ENV{EIGEN_INCLUDE_DIR} NO_DEFAULT_PATH)

# the root directory of the project (the one containing the tenh/ dir) is the only necessary include dir
include_directories(${tensorheaven_SOURCE_DIR} ${EIGEN_INCLUDE_DIR})

# TODO: build options for other compilers
set(COMPILER_DEBUG_FLAGS "-g -Wall -Werror")
set(COMPILER_RELEASE_FLAGS "-O3 -g -DNDEBUG -Wall -Werror")

add_executable(contraction_prototype test/contraction_prototype.cpp)
add_executable(eigen_interop_prototype test/eigen_interop_prototype.cpp)
add_executable(compoundindex test/compoundindex.cpp)
add_executable(list test/list.cpp)
add_executable(strongly_typed_vector_prototype test/strongly_typed_vector_prototype.cpp)
add_executable(tuple test/tuple.cpp)
add_executable(typetuple test/typetuple.cpp)
