cmake_minimum_required(VERSION 2.8)

project(tensorheaven_test)

find_path(EIGEN_INCLUDE_DIR "eigen" PATHS $ENV{EIGEN_INCLUDE_DIR} NO_DEFAULT_PATH)

# the root directory of the project (the one containing the tenh/ dir) is the only necessary include dir
include_directories(${tensorheaven_test_SOURCE_DIR} ${EIGEN_INCLUDE_DIR})

# TODO: build options for other compilers
set(CMAKE_CXX_FLAGS "-Wall") # took out -Werror for now because of dumb -Wtautological-compare in clang not recognizing it's in a template
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-g -O2")
set(CMAKE_CXX_FLAGS_ASMEXAM "-g0 -O2 -save-temps")

# temp tests
add_executable(asm_exam asm_exam.cpp)
add_executable(contraction_prototype contraction_prototype.cpp)
add_executable(eigen_interop_prototype eigen_interop_prototype.cpp)
add_executable(multiindex multiindex.cpp)
add_executable(list list.cpp)
add_executable(strongly_typed_vector_prototype strongly_typed_vector_prototype.cpp)
add_executable(tuple tuple.cpp)
add_executable(typetuple typetuple.cpp)

# test-framework-using tests
set(test_SRCS
    lvd/lvd_callstack.cpp
    lvd/lvd_commandlineparser.cpp
    lvd/lvd_spawner.cpp
    lvd/lvd_testsystem.cpp
    standard/test_array.cpp
    standard/test_euclideanembedding.cpp
    standard/test_expressiontemplates.cpp
    standard/test_list.cpp
    standard/test_tensor2.cpp
    standard/test_tensor2diagonal.cpp
    standard/test_typelist.cpp
    standard/test_vector.cpp)
add_executable(test standard/test.cpp ${test_SRCS})
include_directories(${tensorheaven_test_SOURCE_DIR}/.. ${tensorheaven_test_SOURCE_DIR}/lvd ${tensorheaven_test_SOURCE_DIR}/standard)
