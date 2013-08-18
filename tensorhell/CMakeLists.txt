cmake_minimum_required(VERSION 2.8)

project(tensorheaven_test)

find_path(EIGEN_INCLUDE_DIR "eigen" PATHS $ENV{EIGEN_INCLUDE_DIR} NO_DEFAULT_PATH)
find_path(BOOST_INCLUDE_DIR "boost" PATHS $ENV{BOOST_INCLUDE_DIR} NO_DEFAULT_PATH)

# the root directory of the project (the one containing the tenh/ dir) is the only necessary include dir
include_directories(${tensorheaven_test_SOURCE_DIR} ${EIGEN_INCLUDE_DIR} ${BOOST_INCLUDE_DIR})

# TODO: build options for other compilers
set(CMAKE_CXX_FLAGS "-Wall -Werror") # -Werror can give problems under clang due to -Wtautological-compare see pragma in tensor2symmetric.hpp to disable it as needed.
set(CMAKE_CXX_FLAGS_DEBUG "-g -O3") # this uses -O3 so that the tests run reasonably quickly, even though the inlining may interfere with step-debugging.
set(CMAKE_CXX_FLAGS_DEBUG_NO_OPTIMIZE "-g -O0") # this uses -O0, which is useful for step-debugging sometimes.
set(CMAKE_CXX_FLAGS_RELEASE "-g -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_ASMEXAM "-g0 -O3 -save-temps -DNDEBUG")

# temp tests
add_executable(asm_exam asm_exam.cpp asm_exam_separate_functions.cpp)
add_executable(conceptual_inheritance_prototype conceptual_inheritance_prototype.cpp)
add_executable(conceptual_prototype conceptual_prototype.cpp)
add_executable(contraction_prototype contraction_prototype.cpp)
add_executable(eigen_interop_prototype eigen_interop_prototype.cpp)
add_executable(list list.cpp)
add_executable(strongly_typed_vector_prototype strongly_typed_vector_prototype.cpp)
add_executable(tuple tuple.cpp)
add_executable(typelist typelist.cpp)
add_executable(typetuple typetuple.cpp)

# test-framework-using tests
set(test_SRCS
    lvd/lvd_callstack.cpp
    lvd/lvd_commandlineparser.cpp
    lvd/lvd_spawner.cpp
    lvd/lvd_testsystem.cpp
    standard/randomize.cpp
    standard/test_array.cpp
    standard/test_euclideanembedding.cpp
    standard/test_euclideanembeddinginverse.cpp
    standard/test_expressiontemplates.cpp
    standard/test_interop_eigen_euclideanlyembedded.cpp
    standard/test_interop_eigen_euclideanlyembedded0.cpp
    standard/test_interop_eigen_euclideanlyembedded1.cpp
    standard/test_interop_eigen_euclideanlyembedded2.cpp
    standard/test_interop_eigen_euclideanlyembedded3.cpp
    standard/test_interop_eigen_euclideanlyembedded4.cpp
    standard/test_interop_eigen_euclideanlyembedded5.cpp
    standard/test_interop_eigen_euclideanlyembedded6.cpp
    standard/test_interop_eigen_euclideanlyembedded7.cpp
    standard/test_interop_eigen_inversion.cpp
    standard/test_interop_eigen_inversion0.cpp
    standard/test_interop_eigen_inversion1.cpp
    standard/test_interop_eigen_inversion2.cpp
    standard/test_interop_eigen_inversion3.cpp
    standard/test_interop_eigen_inversion4.cpp
    standard/test_interop_eigen_inversion5.cpp
    standard/test_interop_eigen_inversion6.cpp
    standard/test_interop_eigen_inversion7.cpp
    standard/test_interop_eigen_ldlt.cpp
    standard/test_list.cpp
    standard/test_tensor2.cpp
    standard/test_tensor2diagonal.cpp
    standard/test_typelist.cpp
    standard/test_vector.cpp)
add_executable(test standard/test.cpp ${test_SRCS})
include_directories(${tensorheaven_test_SOURCE_DIR}/.. ${tensorheaven_test_SOURCE_DIR}/lvd ${tensorheaven_test_SOURCE_DIR}/standard)
